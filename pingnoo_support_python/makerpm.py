#!/bin/env python3

# Copyright (C) 2019-21 Adrian Carpenter, et al
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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
""" RPM package wrapper """

import argparse
import os
import pathlib
import re
import shutil
import string
import sys
import tarfile
from functools import lru_cache

from .common import *
from .msg_printer import msg_printer, MsgPrinterException

# Currently unused?
@lru_cache()
def find_provider(library):
    with msg_printer(f"Finding what RPM provides '{library}'"):
        return execute(f"rpm -q --queryformat '%{{NAME}}' --whatprovides {library}", fail_msg='Could not find RPM')


def _find_prereqs():
    for app, rpm in [('git', 'git'),
                     ('rpmbuild', 'rpm-build'),
                     ('rpmdev-setuptree', 'rpmdevtools')]:
        with msg_printer(f"Checking for {app}"):
            if which(app) is None:
                raise MsgPrinterException(f"Could not find '{app}' executable; try installing the '{rpm}' package")


def rpm_create(build_arch, build_type, version, release, key):
    version = re.sub(r'-.*',"",version)
    _find_prereqs()
    if build_arch == "x86":
        build_arch = "i686"  # Fedora/RPM terminology
                             # FIXME: Never got 32-bit working because I didn't have Qt5 built for it

    # Find release info, if any
    rpm_dist = execute("rpm --eval %{?dist}")[1].strip()

    if False:  # TODO: Eventually would like parallel build paths enabled... rpm_dist:
        os.environ['HOME'] = os.path.join(os.getcwd(), f"rpmbuild-{rpm_dist[1:]}-{build_arch}")
        os.chdir(os.environ['HOME'])
    else:
        os.environ['HOME'] = os.getcwd()

    with msg_printer("Removing existing rpmbuild subtree"):
        execute("rm -rf rpmbuild", "Could not remove rpmbuild temporary area")

    with msg_printer("Creating rpmbuild subtree"):
        execute("rpmdev-setuptree", "Could not create rpmbuild temporary area")

    with msg_printer("Creating specfile"):
        with open("rpm/pingnoo.spec.in") as in_file:
            out_data = string.Template(in_file.read()).substitute(version=version,
                                                                  release=release,
                                                                  build_arch=build_arch,
                                                                  cmake_build_type=build_type)
            with open("rpmbuild/SPECS/pingnoo.spec", 'w') as out_file:
                out_file.write(out_data)

    with msg_printer("Creating tarball"):
        # Can't easily exclude... shutil.make_archive(f"pingnoo-{version}", 'tar')

        def tar_filter(tar_info):
            if tar_info.name == f"pingnoo-{version}":  # Top-level
                return tar_info
            for good_str in ('/.git', '/cmake', '/dpkg', '/src', 'CMake'):
                if good_str in tar_info.name:
                    return tar_info
            return None

        with tarfile.open(f"pingnoo-{version}.tar", 'w') as tarball:
            tarball.add('.', arcname=f"pingnoo-{version}", filter=tar_filter)
        shutil.move(f"pingnoo-{version}.tar", "rpmbuild/SOURCES")

    with msg_printer(f"Calling rpmbuild (logged to rpmbuild-{rpm_dist[1:]}-{build_arch}.log)"):
        execute(f"rpmbuild -bb --target {build_arch} rpmbuild/SPECS/pingnoo.spec",
                fail_msg="rpmbuild failed",
                out_log=f"rpmbuild-{rpm_dist[1:]}-{build_arch}.log")

    pathlib.Path(f'bin/{build_arch}/Deploy/rpm/').mkdir(parents=True, exist_ok=True)
    final_name = f'pingnoo-{version}-{release}{rpm_dist}.{build_arch}.rpm'
    shutil.copy2(f'rpmbuild/RPMS/{build_arch}/{final_name}',
                 f'bin/{build_arch}/Deploy/rpm/')

    if key:
        execute(f'rpm --define \"_gpg_name {key}\" '\
                '--define \"_signature gpg\" '\
                '--define \"%_gpg_path /root/.gnupg\" '\
                '--define \"%_gpgbin /usr/bin/gpg\" '\
                '--addsign \"bin/{build_arch}/Deploy/rpm/{final_name}\"')

    return final_name


def main():
    """ Executable wrapper """
    parser = argparse.ArgumentParser(description='rpm build script')
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
    parser.add_argument('--release', type=str, nargs='?', help='release')

    args = parser.parse_args()

    rpm_create(args.arch, args.type, args.version, args.release, args.key)
    sys.exit(0)


if __name__ == "__main__":
    main()
