#!/usr/bin/env bash

#
# Copyright (C) 2021 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 12/04/2021.
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

# install qt

aqt install 5.15.2 linux desktop --external 7z -O /qt

# build gcc 10.1

cd /tmp
wget https://ftpmirror.gnu.org/gcc/gcc-10.1.0/gcc-10.1.0.tar.xz
tar xf gcc-10.1.0.tar.xz
cd gcc-10.1.0
./contrib/download_prerequisites

cd /tmp
mkdir build
cd build
../gcc-10.1.0/configure -v --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --prefix=/usr/local/gcc-10.1.0 --enable-checking=release --enable-languages=c,c++ --disable-multilib --program-suffix=-10.1

make -j $(nproc)
make install-strip

echo "export export PATH=/usr/local/gcc-10.1.0/bin:\$PATH" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=/usr/local/gcc-10.1.0/lib64:\$LD_LIBRARY_PATH" >> ~/.bashrc

# build git from source

cd /tmp
wget https://github.com/git/git/archive/refs/tags/v2.31.0.tar.gz
tar -zxf v2.31.0.tar.gz
cd git-2.31.0
make configure
./configure --prefix=/usr
make -j $(nproc) all
make doc info install install-doc install-html install-info

# build python from source

cd /tmp
wget https://www.python.org/ftp/python/3.9.1/Python-3.9.1.tgz
tar -xf Python-3.9.1.tgz
cd Python-3.9.1
./configure --enable-optimizations
make -j $(nproc)
make install

# install cmake

cd /tmp
wget https://github.com/Kitware/CMake/releases/download/v3.20.0-rc2/cmake-3.20.0-rc2-linux-x86_64.sh
chmod +x cmake-3.20.0-rc2-linux-x86_64.sh
./cmake-3.20.0-rc2-linux-x86_64.sh --skip-license --prefix=/usr/local

# cleanup

rm -rf /tmp/*



