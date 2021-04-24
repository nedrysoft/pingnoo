#!/usr/bin/env python3

#
# Copyright (C) 2021 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 24/04/2021.
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

import re
import subprocess
import os
import sys


class ExecuteException(Exception):
    pass


def execute(command, fail_msg=None, capture_output=True):
    if capture_output:
        output_stream = subprocess.PIPE
        error_stream = subprocess.PIPE
    else:
        output_stream = sys.stdout
        error_stream = sys.stderr

    output = subprocess.run(command,
                            shell=True,
                            stdout=output_stream,
                            stderr=error_stream,
                            check=False)

    if capture_output:
        output_text = output.stdout.decode('utf-8') + output.stderr.decode('utf-8')
    else:
        output_text = ""

    if fail_msg and output.returncode:
        raise ExecuteException(fail_msg + "\r\n\r\n" + output_text)

    return output_text


def initialise_docker():
    try:
        arch = execute("arch", "error: unable to find architecture.")

        if not re.match("^arm.*", arch):
            print("error: this image can only be created on a raspberry pi.")
            sys.exit(1)

        folder = os.path.basename(os.getcwd()).split("-")

        if len(folder) < 2:
            print("error: the folder is not named correctly.")
            sys.exit(1)

        distro = folder[0]
        release = folder[1]

        command = f"/usr/sbin/debootstrap --arch=armhf --no-check-gpg --variant='minbase' " \
                  f"{release} 'rootfs' https://archive.raspbian.org/{distro}/"

        execute(command, "error: root fs could not be downloaded.", False)

        command = "tar --remove-files --numeric-owner -cJvf 'rootfs.tar.xz' -C 'rootfs' ."

        execute(command, "error: root fs could not be compressed.", False)

        sys.exit(0)
    except ExecuteException as exception:
        print(exception)
        sys.exit(1)


if __name__ == "__main__":
    initialise_docker()
