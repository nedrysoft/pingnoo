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

# this script installs the required packages to build pingnoo on a fedora container.
# it is intended for internal use only when setting up build containers for CI

# http://download.proxmox.com/images/system/ contains all available containers, the proxmox GUI doesn't show a lot
# of these so you can manually download and install.

# prerequisites for building

dnf -y group install "C Development Tools and Libraries"
dnf -y install cmake
dnf -y install git
dnf -y install python-qt5-rpm-macros
dnf -y install qt5-qtbase-devel     # fc 33
dnf -y install qt5-devel            # fc 32
dnf -y install qt5-linguist
dnf -y install rpm-build
dnf -y install rpmdevtools
dnf -y install qt5-qtquickcontrols2-devel
dnf -y install hostname
dnf -y install dbus-devel
dnf -y install vim

# get the teamcity build agent

dnf -y install wget
dnf -y install unzip
dnf -y install java-11-openjdk

ln -s /usr/lib64/qt5/bin/lrelease /usr/bin/lrelease

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
    cmake ..
    make
fi
