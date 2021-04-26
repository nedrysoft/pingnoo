#!/bin/env python3

# Copyright (C) 2019-21 Adrian Carpenter, et al
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import argparse
import glob
import hashlib
import os
import re
import shutil
import string

from .common import *
from .msg_printer import msg_printer, MsgPrinterException


def _find_prereqs():
    """ Finds programs - I dunno if all machines have these out of the box, so explicitly checking """
    for app in ('dpkg', 'dpkg-deb', 'dpkg-sig'):
        with msg_printer(f"Checking for {app}"):
            if which(app) is None:
                raise MsgPrinterException(f"Could not find '{app}' executable!")


def deb_create(build_arch, build_type, version, output_file, key, extra_packages):
    _find_prereqs()

    # remove previous dpkg build tree if it exists
    with msg_printer("Creating directory structure"):
        rm_path(f'bin/{build_arch}/Deploy/dpkg')
        for dir_name in ('usr/local/bin/pingnoo', 'DEBIAN', 'usr/share/icons/hicolor/512x512/apps',
                         'usr/share/applications', 'usr/share/doc/pingnoo', 'etc/ld.so.conf.d'):
            os.makedirs(os.path.join(f'bin/{build_arch}/Deploy/dpkg/', dir_name))

        # copy data + binaries into the deb tree
        shutil.copy2(f'bin/{build_arch}/{build_type}/Pingnoo', f'bin/{build_arch}/Deploy/dpkg/usr/local/bin/pingnoo')
        shutil.copy2('src/app/images/appicon-512x512-.png',
                     f'bin/{build_arch}/Deploy/dpkg/usr/share/icons/hicolor/512x512/apps/pingnoo.png')

        shutil.copytree(f'bin/{build_arch}/{build_type}/Components',
                        f'bin/{build_arch}/Deploy/dpkg/usr/local/bin/pingnoo/Components', symlinks=True)

        for file in glob.glob(f'bin/{build_arch}/{build_type}/*.so'):
            shutil.copy2(file, f'bin/{build_arch}/Deploy/dpkg/usr/local/bin/pingnoo')

        shutil.copy2('dpkg/postinst', f'bin/{build_arch}/Deploy/dpkg/DEBIAN')
        shutil.copy2('dpkg/pingnoo.conf', f'bin/{build_arch}/Deploy/dpkg/etc/ld.so.conf.d')
        shutil.copy2('dpkg/copyright', f'bin/{build_arch}/Deploy/dpkg/usr/share/doc/pingnoo')

    with msg_printer("Generating desktop file"):
        build_parts = version.split('-', 1)

        if len(build_parts) != 2:
            linux_build_version = "1.0.0"
        else:
            linux_build_version = build_parts[0][2:]

        with open("installer/Pingnoo.desktop.in", 'r') as desktop_file:
            desktop_template = string.Template(desktop_file.read())

            # use control.in template to create the deb control file
        desktop_file_content = desktop_template.substitute(
            executable="/usr/local/bin/pingnoo/Pingnoo",
            icon="/usr/share/icons/hicolor/512x512/apps/pingnoo.png",
            version=linux_build_version)

        with open( f'bin/{build_arch}/Deploy/dpkg/usr/share/applications/Pingnoo.desktop', 'w') as desktop_file:
            desktop_file.write(desktop_file_content)

    dependencies = set()
    hashes = dict()
    libraries = set()
    packages = set()
    so_regex = re.compile(r"\s*(?P<soname>.*)\s=>")

    # create list of all shared libraries that the application uses (and at the same time create hashes)
    for filepath in glob.iglob(f'bin/{build_arch}/Deploy/dpkg/usr/local/bin/pingnoo/**/*', recursive=True):
        if os.path.isdir(filepath):
            continue
        with msg_printer(f"Determining dependencies of {os.path.basename(filepath)}"):
            # Get MD5 checksum
            hashes[filepath] = hashlib.md5(open(filepath, 'rb').read()).hexdigest()

            # Find .so dependencies
            deps = execute(f'ldd {filepath}', "Failed to run ldd")
            for line in deps.split('\n'):
                result = so_regex.match(line.rstrip())
                if not result:
                    continue
                dependencies.add(result.group("soname"))
            libraries.add(os.path.basename(filepath))

    # Remove our own libraries
    dependencies = dependencies-libraries

    # Using the shared library list, find the package that provides the shared library and add to
    # the list of dependencies
    dpkg_regex = re.compile(r"(?P<pkg>.*):(?P<arch>.*):\s(?P<lib>.*)")
    for dependency in dependencies:
        with msg_printer(f"Determining package providing for {dependency}"):
            dpkg = execute(f'dpkg -S {dependency}', f"Failed to determine package that provides {dependency}")
            result = dpkg_regex.match(dpkg)
            if result:
                packages.add(result.group("pkg"))
            else:
                raise ValueError(f"Could not provider for {dependency}: regex failed parsing:\n{dpkg}")

    # If extra packages are provided, then check each package to make sure it exists and as not already in the
    # package dependencies

    if extra_packages:
        available_extra_packages = []

        extra_packages = args.extra_packages.split(",")

        for package in extra_packages:
            package = package.strip()

            if package in packages:
                continue

            try:
                apt_command = f'apt-cache --quiet=0 show {package}'

                result = subprocess.run(apt_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)

                if result.returncode:
                    continue

                result = result.stderr.decode('UTF-8')

                match_string = f'N: Can\'t select versions from package \'{package}\' as it is purely virtual'

                if result.find(match_string)!=-1:
                    continue

                available_extra_packages.append(package)

            except Exception as exception:
                continue

        packages.extend(available_extra_packages)

    with msg_printer("Creating control files"):
        with open("dpkg/control.in", 'r') as control_file:
            control_template = string.Template(control_file.read())

        if len(available_extra_packages):
            extra_packages = ",".join(available_extra_packages)
        else:
            extra_packages = ""

        # use control.in template to create the deb control file
        control_file_content = control_template.substitute(
            version=version,
            dependencies=",".join(packages)

        with open(f'bin/{build_arch}/Deploy/dpkg/DEBIAN/control', 'w') as control_file:
            control_file.write(control_file_content)

        # add the hashes to the deb hash file
        with open(f'bin/{build_arch}/Deploy/dpkg/DEBIAN/md5sums', 'w') as md5sums_file:
            # Need to strip off the staging area
            leading_path = f'bin/{build_arch}/Deploy/dpkg'
            for file_name, file_hash in hashes.items():
                md5sums_file.write(f'{file_hash}  {file_name[len(leading_path):]}\n')

    package_root = f'bin/{build_arch}/Deploy/dpkg/'

    # remove any previous deployment artifacts
    rm_path('deployment')
    os.makedirs('deployment')

    # create the deb file
    with msg_printer("Bundling package"):
        execute(f'dpkg-deb --build {package_root} "{output_file}"', "Failed to build!")

    if key:
        with msg_printer("Signing package"):
            execute(f'dpkg-sig -k {key} -s origin \"{output_file}\"', "Signing failed")


def main():
    parser = argparse.ArgumentParser(description='dpkg build script')

    parser.add_argument('--arch',
                        choices=['x86_64'],
                        type=str,
                        default='x86_64',
                        nargs='?',
                        help='architecture type to deploy')

    parser.add_argument('--type',
                        choices=['Release', 'Debug'],
                        type=str,
                        default='Release',
                        nargs='?',
                        help='architecture type to deploy')

    parser.add_argument('--output',
                        type=str,
                        default='./deployment/pingnoo.deb',
                        nargs='?',
                        help='the output deb file')

    parser.add_argument('--extra-packages',
                        type=str,
                        default=None,
                        nargs='?',
                        help='any extra packages that are required')

    parser.add_argument('--key',
                        type=str,
                        default=None,
                        nargs='?',
                        help='sign with the gpg key that matches this id')

    parser.add_argument('--version', type=str, nargs='?', help='version')

    args = parser.parse_args()

    deb_create(args.arch, args.type, args.version, args.output, args.key, args.extra_packages)


if __name__ == "__main__":
    main()
