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
import shutil
import string
import sys
import tarfile
from functools import cached_property, lru_cache

from .common import *
from .msg_printer import msg_printer, MsgPrinterException


# A lot of this ended up unused but can be referenced
class PackageManagerFinder:
    """ Finds dnf or yum in path """
    MGRS = ('dnf', 'yum')

    def __init__(self):
        self._exe = None

    @cached_property
    def exe(self):
        """ The RPM repository manager this system uses, e.g. dnf """
        for exe in self.MGRS:
            attempt = which(exe)
            if attempt:
                self._exe = attempt
                return self._exe
        raise RuntimeError("Could not find package manager! Tried: {}".format(", ".join(self.MGRS)))


@lru_cache
def find_provider(library):
    with msg_printer(f"Finding what RPM provides '{library}'"):
        return execute(f"rpm -q --queryformat '%{{NAME}}' --whatprovides {library}", fail_msg='Could not find RPM')


def _find_prereqs():
    for app, rpm in [('rpmbuild', 'rpm-build'),
                     ('rpmdev-setuptree', 'rpmdevtools')]:
        with msg_printer(f"Checking for {app}"):
            if which(app) is None:
                raise MsgPrinterException(f"Could not find '{app}' executable; try installing the '{rpm}' package")


def rpm_create(build_arch, build_type, version, release):
    _find_prereqs()
    if build_arch == "x86":
        build_arch = "i686"  # Fedora/RPM terminology
                             # FIXME: Never got 32-bit working because I didn't have Qt5 built for it

    with msg_printer("Removing existing rpmbuild subtree"):
        execute("rm -rf rpmbuild", "Could not remove rpmbuild temporary area")

    with msg_printer("Creating rpmbuild subtree"):
        os.environ['HOME'] = os.getcwd()
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
        tarball = tarfile.open(f"pingnoo-{version}.tar", 'w', )

        def tar_filter(tar_info):
            if tar_info.name == f"pingnoo-{version}":  # Top-level
                return tar_info
            for good_str in ('/.git', '/cmake', '/dpkg', '/src', 'CMake'):
                if good_str in tar_info.name:
                    return tar_info
            return None

        tarball.add('.', arcname=f"pingnoo-{version}", filter=tar_filter)
        shutil.move(f"pingnoo-{version}.tar", "rpmbuild/SOURCES")

    with msg_printer("Calling rpmbuild (logged to rpmbuild.log)"):
        execute(f"rpmbuild -bb --target {build_arch} rpmbuild/SPECS/pingnoo.spec",
                fail_msg="rpmbuild failed",
                out_log="rpmbuild.log")

    pathlib.Path(f'bin/{build_arch}/Deploy/rpm/').mkdir(parents=True, exist_ok=True)
    shutil.copy2(f'rpmbuild/RPMS/{build_arch}/pingnoo-{version}-{release}.{build_arch}.rpm',
                 f'bin/{build_arch}/Deploy/rpm/')

    return False  # 0 is good for POSIX below and expected in deploy.py


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
    parser.add_argument('--version', type=str, nargs='?', help='version')
    parser.add_argument('--release', type=str, nargs='?', help='release')

    args = parser.parse_args()

    sys.exit(rpm_create(args.arch, args.type, args.version, args.release))


if __name__ == "__main__":
    main()
