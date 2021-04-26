#!/usr/bin/env bash

#
# Copyright (C) 2021 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 25/04/2021.
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

# build ruby from source

cd /tmp
wget https://cache.ruby-lang.org/pub/ruby/2.7/ruby-2.7.3.tar.gz
tar -xf ruby-2.7.3.tar.gz
cd ruby-2.7.3
./configure
make -j$(($(nproc)-1))
make install

echo "gem: --no-ri --no-rdoc" >/etc/gemrc

gem install fpm -v 1.4.0
gem install fpm-cookery -v 0.29.0
gem install buildtasks -v 0.0.1
gem install bundler -v 1.10.0

# build gcc 8 from source

cd /tmp
wget https://ftp.gnu.org/gnu/gcc/gcc-8.3.0/gcc-8.3.0.tar.xz
tar -xf gcc-8.3.0.tar.xz
cd gcc-8.3.0
./contrib/download_prerequisites
mkdir build
cd build
../configure --enable-languages=c,c++,fortran \
  --build=x86_64-linux-gnu \
  --host=x86_64-linux-gnu --target=x86_64-linux-gnu \
  --prefix=/usr/local/gcc-8.3.0 -program-suffix=-8.3
make -j$(($(nproc)-1))
make install-strip

# build git from source

cd /tmp
wget https://github.com/git/git/archive/refs/tags/v2.31.0.tar.gz
tar -zxf v2.31.0.tar.gz
cd git-2.31.0
make configure
./configure --prefix=/usr
make all doc info
make install install-doc install-html install-info

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



