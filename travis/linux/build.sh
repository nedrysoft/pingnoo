#!/bin/sh

# Copyright (C) 2019 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 19/02/2021.
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

sudo apt-get update -qq
sudo apt-get install build-essential
sudo apt-get install qt5-default
sudo apt-get install qtdeclarative5-dev
sudo apt-get install qttools5-dev
sudo apt-get install mesa-common-dev
sudo apt-get install libdbus-1-dev
export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/qt5
mkdir build
cd build
cmake ../
make -j 4