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

# this script is used to generate the docker image(s) for a platform, it can be called with the name of the folder
# to generate, or called with the --all parameter with {os}-{release} and then the script will attempt tp build the
# platforms images.
#
# i.e call with "raspbian-buster" as the image name, it will try to build:
#
#   raspbian-buster
#   raspbian-buster-base
#   raspbian-buster-builder
#
# in that order.  Any folder that does not exist will be skipped.

import argparse
import importlib
import os
import subprocess
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


try:
    parser = argparse.ArgumentParser(description='docker build image generator')

    parser.add_argument("image", nargs=None, help="the image to build (the folder that contains the Dockerfile")
    parser.add_argument('--all',
                        action='store_true',
                        help='build all images, first without a suffix, then with -base and finally -builder')
    parser.add_argument('--user', type=str, nargs='?', help='the user to run the build as', default=None)
    parser.add_argument('--group', type=str, nargs='?', help='the group to run the build as', default=None)
    parser.add_argument('--registry',
                        type=str,
                        nargs='?',
                        help='the group to run the build as',
                        default='registry.fizzyade.com')

    args = parser.parse_args()

    user_string = ""
    group_string = ""

    if args.user is not None:
        user_id = execute(f"id -u {args.user}", "error: error finding the user id for user  \'{args.user}\'")
 
        user_id = user_id.strip()
        
        user_string = f'--build-arg DOCKER_USER={args.user} ' \
                      f'--build-arg DOCKER_USER_ID={user_id} '
        
        user_id = user_id.strip()
    
    if args.group is not None:
        group_id = execute(f"id -g {args.user}", "error: error finding the group id for user \'{args.user}\'.")
        
        group_id = group_id.strip()
        
        group_string = f'--build-arg DOCKER_GROUP={args.group} ' \
                       f'--build-arg DOCKER_GROUP_ID={group_id} '
        
    images = [args.image]

    if args.all:
        images.extend([f'{args.image}-base', f'{args.image}-builder'])

    base_folder = os.getcwd()

    for current_image in images:
        if not os.path.exists(current_image):
            continue

        if not os.path.exists(current_image+"/Dockerfile"):
            continue

        if os.path.exists(current_image+"/init.py"):
            init_module = importlib.import_module(current_image+".init", package=None)

            init_module.initialise_docker()

        docker_command = f'(cd {current_image} && docker build -f Dockerfile ' \
                         f'{user_string}{group_string}' \
                         f'-t {args.registry}/{current_image} .)'

        execute(docker_command, "error: error building docker image.", False)

        docker_command = f'docker push {args.registry}/{current_image}'

        execute(docker_command, "error: error pushing the docker image to \'{args.registry'}\'.", False)

except ExecuteException as error:
    print(error)
