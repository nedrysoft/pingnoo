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

# docker build -f Dockerfile -t registry.fizzyade.com/arch-builder .
# docker save registry.fizzyade.com/arch-builder | gzip > arch-builder.tar.gz
# docker import arch-builder.tar.gz
# docker push registry.fizzyade.com/arch-builder

FROM archlinux

LABEL maintainer="hello@nedrysoft.com"

RUN pacman --noconfirm -Syu
RUN pacman --noconfirm -Fy
RUN pacman --noconfirm -S cmake \
    wget \
    git \
    base-devel \
    qt5-base \
    qt5-tools \
    qt5-quickcontrols \
    dbus \
    python3 \
    python-pip

RUN pip install git-archive-all

ARG DOCKER_USER=teamcity
ARG DOCKER_GROUP=teamcity
ARG DOCKER_USER_ID=0
ARG DOCKER_GROUP_ID=0

COPY makepkg.conf /etc/makepkg.conf



