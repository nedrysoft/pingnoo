#!/usr/bin/env sh

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

# prerequisites for building

apt update -y

apt install software-properties-common -y

add-apt-repository ppa:git-core/ppa -y
add-apt-repository ppa:ubuntu-toolchain-r/test
add-apt-repository ppa:deadsnakes/ppa

apt update -y

apt remove --auto-remove python3.5 -y
apt install curl -y
apt install gcc-6 -y
apt install g++-6 -y
apt install unzip -y
apt install vim -y
apt install apt-get -y
apt install mesa-common-dev -y
apt install libdbus-1-dev -y
apt install libxcb-xinerama0-dev -y
apt install git -y
apt install python3.9 -y
apt install python3.9-dev -y
apt install python3.9-venv -y

update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.9 2

# download and install cmake

cd ~
wget https://github.com/Kitware/CMake/releases/download/v3.20.0-rc2/cmake-3.20.0-rc2-linux-x86_64.sh
chmod +x cmake-3.20.0-rc2-linux-x86_64.sh
./cmake-3.20.0-rc2-linux-x86_64.sh --skip-license --prefix=/usr/local

wget http://172.29.13.4/linux/qt5.15.2.tar.gz
cd ~
mkdir Qt
cd Qt
tar -xf ../qt5.15.2.tar.gz 2>/dev/null

# get the teamcity build agent

apt-get install openjdk-8-jdk -y

cd /tmp
wget https://$1/update/buildAgentFull.zip
mkdir /opt/teamcity-agent
cd /opt/teamcity-agent
unzip /tmp/buildAgentFull.zip

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

systemctl enable teamcity-agent
service teamcity-agent start

# if the second parameter is test then checkout the code and attempt to build

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


