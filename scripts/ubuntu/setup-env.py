#!/usr/bin/env python3

#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 25/02/2021.
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

import os
import platform
import shutil
import subprocess
import sys

""" A context handler to print things to the console nicely """
import platform
import sys
from contextlib import contextmanager

try:
    from colorama import Fore, Style, init

    init(autoreset=True)

except ModuleNotFoundError:
    # Make some blank wrappers
    class Style:
        BRIGHT = ''
        RESET = ''


    class Fore:
        GREEN = ''
        RED = ''
        CYAN = ''
        RESET = ''

class MsgPrinterException(Exception):
    pass


GOOD_SYMBOL = 'Y' if platform.system() == "Windows" else '✓'
BAD_SYMBOL = 'N' if platform.system() == "Windows" else '✘'

def msg_printer(start_msg=None):
    """ Context to use to print messages. Will exit if receives MsgPrinterException """
    try:
        if start_msg:
            sys.stdout.write(Style.BRIGHT + f'> {start_msg} ')
            sys.stdout.flush()
            yield None
            sys.stdout.write(Style.BRIGHT + '[' + Fore.GREEN + GOOD_SYMBOL + Fore.RESET + ']\r\n')
            sys.stdout.flush()
        else:
            yield None
    except MsgPrinterException as err:
        if start_msg:
            sys.stdout.write(Style.BRIGHT + '[' + Fore.RED + BAD_SYMBOL + Fore.RESET + ']\r\n')
        print('\r\n' + Fore.RED + 'ERROR: ' + Fore.RESET + str(err), flush=True)
        sys.exit(1)

class ExecuteException(MsgPrinterException):
    """ Custom class to separate msgprinter vs. this code """

def execute(command, fail_msg=None, out_log=None):
    """ Execute a command in subprocess and throw exceptions if given fail_msg """
    output = subprocess.run(command,
                            shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            check=False)
    output_text = output.stdout.decode('utf-8') + output.stderr.decode('utf-8')

    if out_log:
        with open(out_log, 'w') as out_log_file:
            out_log_file.write(output_text)

    if fail_msg and output.returncode:  # Non-zero is error  FIXME: Is this true on Windows?
        raise ExecuteException(fail_msg + "\r\n\r\n" + output_text)

    if fail_msg is None:  # Return backwards-compat results of return code + text
        return output.returncode, output_text

    return output_text


teamCityServer = 'https://172.29.13.4:8111'

with msg_printer('Preparing apt...'):
    execute('apt update -y',
            fail_msg='unable to update apt. (1)')

    execute('apt install software-properties-common -y',
            fail_msg='unable to install software-properties-common')

    execute('add-apt-repository ppa:git-core/ppa -y',
            fail_msg='unable to add ppa:git-core/ppa')

    execute('add-apt-repository ppa:ubuntu-toolchain-r/test -y',
            fail_msg='unable to add ppa:ubuntu-toolchain-r/test')

    execute('add-apt-repository ppa:deadsnakes/ppa -y',
            fail_msg='unable to add ppa:deadsnakes/ppa')

    execute('apt update -y',
            fail_msg='unable to update apt. (2)')

with msg_printer('Installing packages...'):
    execute('apt install curl -y',
            fail_msg='unable to install curl.')

    execute('apt install gcc-6 -y',
            fail_msg='unable to install gcc-6.')

    execute('apt install g++-6 -y',
            fail_msg='unable to install g++-6.')

    execute('apt install unzip -y',
            fail_msg='unable to install unzip.')

    execute('apt install wget -y',
            fail_msg='unable to install wget.')

    execute('apt install vim -y',
            fail_msg='unable to install vim.')

    execute('apt install unzip -y',
            fail_msg='unable to install unzip.')

    execute('apt install mesa-common-dev -y',
            fail_msg='unable to install mesa-common-dev.')

    execute('apt install libdbus-1-dev -y',
            fail_msg='unable to install libdbus-1-dev.')

    execute('apt install libxcb-xinerama0-dev -y',
            fail_msg='unable to install libxcb-xinerama0-dev.')

    execute('apt install git -y',
            fail_msg='unable to install git.')

    execute('apt install python3.9 -y',
            fail_msg='unable to install python3.9.')

    execute('apt install python3.9-dev -y',
            fail_msg='unable to install python3.9-dev.')

    execute('apt install python3.9-venv -y',
            fail_msg='unable to install python3.9-venv.')

with msg_printer('Setting python 3.9 as default...'):
    execute('update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.9 2',
            fail_msg='unable to install python 3.9 as default.')

with msg_printer('Installing CMake 3.20.0...'):
    execute('cd ~;'
            'wget https://github.com/Kitware/CMake/releases/download/v3.20.0-rc2/cmake-3.20.0-rc2-linux-x86_64.sh;'
            './cmake-3.20.0-rc2-linux-x86_64.sh --skip-license --prefix=/usr/local',
            fail_msg='unable to install CMake.')

with msg_printer('Downloading Qt 5.15.2...'):
    execute('cd ~;'
            'wget http://172.29.13.4/linux/qt5.15.2.tar.gz;'
            'mkdir Qt;'
            'cd Qt;'
            'tar -xf ../qt5.15.2.tar.gz 2>/dev/null',
            fail_msg='unable to install Qt.')

with msg_printer('Installing TeamCity Build Agent...'):
    execute('apt-get install openjdk-8-jdk -y;'           
            'cd /tmp;'
            f'wget {teamCityServer}/update/buildAgentFull.zip;'
            'mkdir /opt/teamcity-agent;'
            'cd /opt/teamcity-agent;'
            'unzip /tmp/buildAgentFull.zip',
            ail_msg='unable to install TeamCity build agent.')

# TODO: add the script to /etc/systemd

with msg_printer('Starting TeamCity Build Agent...'):
    execute('systemctl enable teamcity-agent;'
            'service teamcity-agent start',
            ail_msg='unable to start TeamCity build agent service.')

# this script installs the required packages to build pingnoo on an ubuntu 16.04 LTS container.
# it is intended for internal use only when setting up build containers for CI

# http://download.proxmox.com/images/system/ contains all available containers, the proxmox GUI doesn't show a lot
# of these so you can manually download and install.

# FUSE requires the following lines to be added to the containers configuration
# in addition you need to enable FUSE on the container in the settings

# /etc/pve/lxc/<$container_id>.conf
# ----
# lxc.autodev: 1
# lxc.hook.autodev: sh -c "mknod -m 0666 ${LXC_ROOTFS_MOUNT}/dev/fuse c 10 229"
# ----

# /etc/systemd/system/teamcity-agent.service
# ---

#[Unit]
#Description=TeamCity Agent
#After=network.target
#
#[Install]
#WantedBy=multi-user.target
#
#[Service]
#ExecStart=/opt/teamcity-agent/bin/agent.sh start
#Type=forking
#RemainAfterExit=yes
#SyslogIdentifier=teamcity_agent
#PrivateTmp=yes
#PIDFile=/opt/teamcity-agent/logs/buildAgent.pid
#ExecStop=/opt/teamcity-agent/bin/agent.sh stop
#RestartSec=5
#Restart=on-failure

#---



# if the second parameter is test then checkout the code and attempt to build
"""
if [ $2 -eq "test" ]; then
    # clone the code
    cd ~

    git clone https://github.com/nedrysoft/pingnoo.git
    cd pingnoo
    git submodule update --init --recursive

    # run a test build to check that all the required tools & libraries are installed

    mkdir build
    cd build
    CC=gcc-6 CXX=g++-6 /usr/local/bin/cmake .. -DCMAKE_PREFIX_PATH=/root/Qt/5.15.2/gcc_64
    make
fi
"""

