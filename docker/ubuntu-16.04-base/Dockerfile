#
# Copyright (C) 2021 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 14/04/2021.
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

# docker build -f Dockerfile -t registry.fizzyade.com/ubuntu-16.04-base .
# docker save registry.fizzyade.com/ubuntu-16.04-base | gzip > ubuntu-16.04-base.tar.gz
# docker import ubuntu-16.04-base.tar.gz
# docker push registry.fizzyade.com/ubuntu-16.04-base

FROM ubuntu:16.04

LABEL maintainer="hello@nedrysoft.com"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y && \
    apt-get install -y apt-utils software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y \
    curl \
    python3 \
    p7zip \
    p7zip-full && \
    curl https://bootstrap.pypa.io/pip/3.5/get-pip.py -o get-pip.py && \
    python3 get-pip.py && \
    python3 -m pip install aqtinstall && \
    apt-get install -y \
    gcc-6 \
    g++-6 \
    curl \
    build-essential \
    wget \
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
    dpkg-sig \
    autoconf \
    dh-autoreconf \
    libcurl4-gnutls-dev \
    libexpat1-dev \
    gettext \
    libz-dev \
    libssl-dev \
    asciidoc \
    xmlto \
    docbook2x \
    install-info \
    zlib1g-dev \
    libncurses5-dev \
    libgdbm-dev \
    libnss3-dev \
    libssl-dev \
    libsqlite3-dev \
    libreadline-dev \
    libffi-dev \
    libbz2-dev \
    ruby \
    ruby-dev \
    fuse \
    appstream
