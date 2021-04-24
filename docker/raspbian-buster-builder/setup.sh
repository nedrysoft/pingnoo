#!/usr/bin/env bash

#
# Copyright (C) 2021 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 23/04/2021.
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

set -e

# build gcc 10 from source

cd /tmp
wget https://ftp.gnu.org/gnu/gcc/gcc-10.1.0/gcc-10.1.0.tar.xz
tar -xf gcc-10.1.0.tar.xz
cd gcc-10.1.0
./contrib/download_prerequisites
mkdir build
cd build
../configure --enable-languages=c,c++,fortran --with-cpu=cortex-a72 \
  --with-fpu=neon-fp-armv8 --with-float=hard --build=arm-linux-gnueabihf \
  --host=arm-linux-gnueabihf --target=arm-linux-gnueabihf  \
  --prefix=/usr/local/gcc-10.1.0 -program-suffix=-10.1
make -j$(($(nproc)-1))
make install-strip

# build cmake from source

cd /tmp
wget https://github.com/Kitware/CMake/releases/download/v3.20.1/cmake-3.20.1.tar.gz
tar -xf cmake-3.20.1.tar.gz
cd cmake-3.20.1
./bootstrap
make -j$(($(nproc)-1))
make install

# build git from source

cd /tmp
wget https://github.com/git/git/archive/refs/tags/v2.31.0.tar.gz
tar -zxf v2.31.0.tar.gz
cd git-2.31.0
make configure
./configure --prefix=/usr
make -j$(($(nproc)-1)) all
make doc info install install-doc install-html install-info

# build python from source

cd /tmp
wget https://www.python.org/ftp/python/3.9.1/Python-3.9.1.tgz
tar -xf Python-3.9.1.tgz
cd Python-3.9.1
./configure --enable-optimizations
make -j$(($(nproc)-1))
make install

pip3 install colorama

# cleanup

rm -rf /tmp/*

