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
import os
import re
import shutil
import string

from .common import *
from .msg_printer import msg_printer


def pkg_create(build_arch, build_type, version, key):

    with msg_printer("Preparing deploy directory"):
        rm_path(f'bin/{build_arch}/Deploy')
        os.makedirs(f'bin/{build_arch}/Deploy')

        shutil.copy2('pkg/pingnoo.install', f'bin/{build_arch}/Deploy')

    with msg_printer("Getting git info"):
        git_year = re.sub(r'\W', '', execute("git log -1 --format=%cd --date=format:%Y")[1])
        git_month = re.sub(r'\W', '', execute("git log -1 --format=%cd --date=format:%m")[1])
        git_day = re.sub(r'\W', '', execute("git log -1 --format=%cd --date=format:%d")[1])
        git_hash = re.sub(r'\W', '', execute("git log -1 --format=%h")[1])
        git_branch = re.sub(r'\W', '', execute("git branch --show-current")[1])
        git_uncommitted = 0

    aur_source_location = 'https://www.nedryspft.com/downloads/${pkgname}/source/${pkgname}-${pkgver}.tar.gz'

    source_filename = f'pingnoo.tar.gz'

    source_location = f'/tmp/{source_filename}'

    with msg_printer("Creating source tarball"):
        execute(f'git-archive-all {source_location}', "Failed to create source archive")

    with msg_printer("Generating tarball hash"):
        tarball_hash = execute(f'md5sum {source_location}', "Failed to get hash of source archive").split(' ')[0]

    dependencies = set()
    libraries = set()
    packages = set()
    so_regex = re.compile(r"\s*(?P<soname>.*)\s=>")
    build_type = build_type.capitalize()

    search_files = {f'bin/{build_arch}/{build_type}/Pingnoo'}

    # create list of all shared libraries that the application uses
    for filepath in glob.iglob(f'bin/{build_arch}/{build_type}/**/*.so', recursive=True):
        if os.path.isdir(filepath):
            continue

        search_files.add(filepath)

    for filepath in search_files:
        with msg_printer(f"Determining dependencies of {os.path.basename(filepath)}"):
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
    pkg_regex = re.compile(r".*/(?P<pkg>\S*)\s.*\[installed(:.*)?]")
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

        build_version = version.split('-')

        # use PKGBUILD.in template to create PKGBUILD file
        pkgbuild_file_content = pkgbuild_template.substitute(
            GIT_YEAR=git_year, GIT_MONTH=git_month, GIT_DAY=git_day, GIT_HASH=git_hash, GIT_BRANCH=git_branch,
            GIT_UNCOMMITTED=git_uncommitted, sourcelocation=f'file://{source_location}', arch=build_arch,
            md5sum=tarball_hash, version=build_version[0], dependencies="\'{0}\'".format("\' \'".join(packages)))

        aur_pkgbuild_file_content = pkgbuild_template.substitute(
            GIT_YEAR=git_year, GIT_MONTH=git_month, GIT_DAY=git_day, GIT_HASH=git_hash, GIT_BRANCH=git_branch,
            GIT_UNCOMMITTED=git_uncommitted, sourcelocation=aur_source_location, arch=build_arch, md5sum=tarball_hash,
            version=build_version[0], dependencies="\'{0}\'".format("\' \'".join(packages)))

        with open(f'bin/{build_arch}/Deploy/PKGBUILD', 'w') as pkgbuild_file:
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
        execute(
            f'PKGDEST={deployment_dir} bash -c "cd bin/{build_arch}/Deploy && sudo -u nobody makepkg {key_param}"', "Failed to build!")

    with msg_printer("Creating AUR deployment"):
        os.makedirs(f'{deployment_dir}/aur')
        shutil.copy2(source_location, f'{deployment_dir}/aur/pingnoo-{git_year}.{git_month}.{git_day}.tar.gz')
        shutil.copy2('pkg/pingnoo.install', f'{deployment_dir}/aur/')

        with open(f'{deployment_dir}/aur/PKGBUILD', 'w') as pkgbuild_file:
            pkgbuild_file.write(aur_pkgbuild_file_content)

        execute(f'cd {deployment_dir}/aur && makepkg --printsrcinfo > .SRCINFO', "Failed to create .SRCINFO!")


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

    pkg_create(args.arch, args.type, args.version, args.key)


if __name__ == "__main__":
    main()
