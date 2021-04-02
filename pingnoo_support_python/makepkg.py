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

    with msg_printer("Preparing deploy directory"):
        rm_path(f'bin/{buildArch}/Deploy')
        os.makedirs(f'bin/{buildArch}/Deploy')

        shutil.copy2('pkg/pingnoo.install', f'bin/{buildArch}/Deploy')

    dependencies = set()
    hashes = dict()
    libraries = set()
    packages = set()
    so_regex = re.compile(r"\s*(?P<soname>.*)\s=>")

    search_files = {f'bin/{buildArch}/Release/Pingnoo'}

    # create list of all shared libraries that the application uses
    for filepath in glob.iglob(f'bin/{buildArch}/Release/**/*.so', recursive=True):
        if os.path.isdir(filepath):
            continue

        search_files.add(filepath)

    for filepath in search_files:
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
    pkg_regex = re.compile(r".*\/(?P<pkg>\S*)\s.*\[installed\]")
    for dependency in dependencies:
        dependency = os.path.basename(dependency)
        with msg_printer(f"Determining package providing for {dependency}"):
            pkg = execute(f'pacman -F {dependency}', f"Failed to determine package that provides {dependency}")
            result = pkg_regex.match(pkg)
            if result:
                packages.add(result.group("pkg"))
            else:
                raise ValueError(f"Could not provider for {dependency}: regex failed parsing:\n{pkg}")

    with msg_printer("Creating PKGBUILD file"):
        with open("pkg/PKGBUILD.in", 'r') as pkgbuild_file:
            pkgbuild_template = string.Template(pkgbuild_file.read())

        # use PKGBUILD.in template to create PKGBUILD file
        pkgbuild_file_content = pkgbuild_template.substitute(version=version, dependencies="\'{0}\'".format("\' \'".join(packages)))

        with open(f'bin/{buildArch}/Deploy/PKGBUILD', 'w') as pkgbuild_file:
            pkgbuild_file.write(pkgbuild_file_content)

    # remove any previous deployment artifacts
    rm_path('deployment')
    os.makedirs('deployment')

    deployment_dir = os.getcwd()+"/deployment"

    key_param = ""

    if key:
        key_param = f'--sign --key {key}'

    # create the pkg file
    with msg_printer("Building package"):
        execute(f'PKGDEST={deployment_dir} bash -c "cd bin/{buildArch}/Deploy && makepkg ${key_param}"', "Failed to build!")

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
