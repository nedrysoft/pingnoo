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

# docker build -f Dockerfile -t registry.fizzyade.com/raspbian-stretch-base .
# docker save registry.fizzyade.com/raspbian-stretch-base | gzip > raspbian-stretch-base.tar.gz
# docker import raspbian-stretch-base.tar.gz
# docker push registry.fizzyade.com/raspbian-stretch-base

FROM registry.fizzyade.com/raspbian-stretch

LABEL maintainer="hello@nedrysoft.com"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y
RUN apt-get install -y --no-install-recommends \
    software-properties-common  \
    wget \
    curl \
    build-essential \
    unzip \
    vim \
    mesa-common-dev \
    libdbus-1-dev \
    libxcb-xinerama0-dev \
    qttools5-dev-tools \
    qtdeclarative5-dev \
    qml-module-qtquick-controls \
    qt5-default \
    qttools5-dev \
    default-jdk \
    dpkg-sig \
    ruby \
    autoconf \
    ruby-dev \
    libssl-dev \
    zlib1g-dev \
    tcl \
    gettext \
    asciidoc \
    docbook2x \
    install-info \
    libsqlite0 \
    libsqlite0-dev \
    libreadline-dev \
    libbz2-dev \
    sqlite3 \
    tk-dev \
    libgdbm-dev \
    libncurses5-dev \
    uuid-dev \
    lzma-dev \
    liblzma-dev \
    xmlto \
    libcurl4-openssl-dev
