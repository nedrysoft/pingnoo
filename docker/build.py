#!/usr/bin/env python3

#
# Copyright (C) 2021 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 22/04/2021.
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
import os
import subprocess

class ExecuteException(Exception):
    pass

def execute(command, fail_msg=None):
    """ Execute a command in subprocess and throw exceptions if given fail_msg """
    output = subprocess.run(command,
                            shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            check=False)
    output_text = output.stdout.decode('utf-8') + output.stderr.decode('utf-8')

    if fail_msg and output.returncode:
        raise ExecuteException(fail_msg + "\r\n\r\n" + output_text)
    if fail_msg is None:  # Return backwards-compat results of return code + text
        return output.returncode, output_text
    return output_text

try:
    parser = argparse.ArgumentParser(description='docker build image generator')

    parser.add_argument("image", nargs=None, help="the image to build (the folder that contains the Dockerfile")
    parser.add_argument('--user', type=str, nargs='?', help='the user to run the build as', default='root')
    parser.add_argument('--group', type=str, nargs='?', help='the group to run the build as', default='root')
    parser.add_argument('--registry', type=str, nargs='?', help='the group to run the build as', default='registry.fizzyade.com')

    args = parser.parse_args()

    if not os.path.exists(args.image):
        print(f"error: the image {args.image} does not exist.")
        exit(1)

    if not os.path.exists(args.image+"/Dockerfile"):
        print(f"error: the image {args.image} does not contain a Dockerfile.")
        exit(1)

    user_id=execute(f"id -u {args.user}", "error: error finding the user id for user  \'{args.user}\'")

    group_id=execute(f"id -g {args.user}", "error: error finding the group id for user \'{args.user}\'.")

    user_id = user_id.strip()
    group_id = group_id.strip()

    docker_command = 'docker build -f Dockerfile ' \
                     f'--build-arg DOCKER_USER={args.user} ' \
                     f'--build-arg DOCKER_GROUP={args.group} ' \
                     f'--build-arg DOCKER_USER_ID={user_id} ' \
                     f'--build-arg DOCKER_GROUP_ID={group_id} ' \
                     f'-t {args.registry}/{args.image}'

    execute(docker_command, "error: error building docker image.")

    docker_command = f'docker push {args.registry}/{args.image}'

    execute(docker_command, "error: error pushing the docker image to \'{args.registry'}\'.")

except ExecuteException as error:
    print(error)
