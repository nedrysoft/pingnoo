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

def pkgCreate(buildArch, buildType, version, outputFile, key):
    # remove previous pkg build tree if it exists
    with msg_printer("Creating directory structure"):
        rm_path(f'bin/{buildArch}/Deploy/pkg')
        for dir_name in ('usr/local/bin/pingnoo', 'usr/share/icons/hicolor/512x512/apps',
                         'usr/share/applications', 'usr/share/doc/pingnoo', 'etc/ld.so.conf.d'):
            os.makedirs(os.path.join(f'bin/{buildArch}/Deploy/pkg/', dir_name))

        # copy data + binaries into the pkg tree
        shutil.copy2(f'bin/{buildArch}/{buildType}/Pingnoo', f'bin/{buildArch}/Deploy/pkg/usr/local/bin/pingnoo')
        shutil.copy2('src/app/images/appicon-512x512-.png',
                     f'bin/{buildArch}/Deploy/pkg/usr/share/icons/hicolor/512x512/apps/pingnoo.png')

        shutil.copytree(f'bin/{buildArch}/{buildType}/Components',
                        f'bin/{buildArch}/Deploy/pkg/usr/local/bin/pingnoo/Components', symlinks=True)

        for file in glob.glob(f'bin/{buildArch}/{buildType}/*.so'):
            shutil.copy2(file, f'bin/{buildArch}/Deploy/pkg/usr/local/bin/pingnoo')

        shutil.copy2('pkg/pingnoo.install', f'bin/{buildArch}/Deploy/pkg')
        shutil.copy2('pkg/pingnoo.conf', f'bin/{buildArch}/Deploy/pkg/etc/ld.so.conf.d')
        shutil.copy2('pkg/Pingnoo.desktop', f'bin/{buildArch}/Deploy/pkg/usr/share/applications')

    dependencies = set()
    hashes = dict()
    libraries = set()
    packages = set()
    so_regex = re.compile(r"\s*(?P<soname>.*)\s=>")
    """
    # create list of all shared libraries that the application uses (and at the same time create hashes)
    for filepath in glob.iglob(f'bin/{buildArch}/Deploy/pkg/usr/local/bin/pingnoo/**/*', recursive=True):
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
    """
    # Remove our own libraries
    dependencies = dependencies-libraries

    # Using the shared library list, find the package that provides the shared library and add to
    # the list of dependencies
    pkg_regex = re.compile(r".*\/(?P<pkg>\S*)\s.*\[installed\]")
    for dependency in dependencies:
        dependency = os.path.basename(dependency)
        with msg_printer(f"Determining package providing for {dependency}"):
            pkg = execute(f'pacman -F {dependency}', f"Failed to determine package that provides {dependency}")
            result = pkg_regex.match(pkg)
            if result:
                print(result.group("pkg"))
                packages.add(result.group("pkg"))
            else:
                raise ValueError(f"Could not provider for {dependency}: regex failed parsing:\n{pkg}")

    with msg_printer("Creating control files"):
        with open("pkg/PKGBUILD.in", 'r') as control_file:
            control_template = string.Template(control_file.read())

        # use control.in template to create the deb control file
        control_file_content = control_template.substitute(version=version, dependencies="\'{0}\'".format("\' \'".join(packages)))

        with open(f'bin/{buildArch}/Deploy/pkg/PKGBUILD', 'w') as control_file:
            control_file.write(control_file_content)

    package_root = f'bin/{buildArch}/Deploy/pkg/'

    # remove any previous deployment artifacts
    rm_path('deployment')
    os.makedirs('deployment')

    # create the pkg file
    with msg_printer("Bundling package"):
        os.chdir(package_root)
        execute(f'makepkg', "Failed to build!")


    #if key:
    #    with msg_printer("Signing package"):
    #        execute(f'dpkg-sig -k {key} -s origin \"{outputFile}\"', "Signing failed")


def main():
    parser = argparse.ArgumentParser(description='pkg build script')

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
                        help='the output pkg file')

    parser.add_argument('--key',
                        type=str,
                        default=None,
                        nargs='?',
                        help='sign with the gpg key that matches this id')

    parser.add_argument('--version', type=str, nargs='?', help='version')

    args = parser.parse_args()

    pkgCreate(args.arch, args.type, args.version, args.output, args.key)

if __name__ == "__main__":
    main()
