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

# docker build -f Dockerfile -t nedrysoft/debian-buster-base .
# docker save nedrysoft/debian-buster-base | gzip > debian-buster-base.tar.gz
# docker import debian-buster-base.tar.gz

FROM debian:buster
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install -y && \
    apt-get install software-properties-common -y && \
    apt update -y && \
    apt install -y curl \
    build-essential \
    unzip \
    vim \
    mesa-common-dev \
    libdbus-1-dev \
    libxcb-xinerama0-dev \
    git \
    qttools5-dev-tools \
    qtdeclarative5-dev \
    qml-module-qtquick-controls \
    qt5-default \
    qttools5-dev \
    default-jdk \
    dpkg-sig \
    wget \
    dh-autoreconf \
    libcurl4-gnutls-dev \
    libexpat1-dev \
    gettext libz-dev \
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
    libffi-dev curl \
    libbz2-dev \
    ruby \
    ruby-dev && \
    echo "gem: --no-ri --no-rdoc" >/etc/gemrc && \
    gem install fpm -v 1.4.0 && \
    gem install fpm-cookery -v 0.29.0 && \
    gem install buildtasks -v 0.0.1 && \
    gem install bundler -v 1.10.0

