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

# docker build -f Dockerfile -t registry.fizzyade.com/fedora-32-builder .
# docker save registry.fizzyade.com/fedora-32-builder | gzip > fedora-32-builder.tar.gz
# docker import fedora-32-builder.tar.gz
# docker push registry.fizzyade.com/fedora-32-builder

FROM fedora:32

LABEL maintainer="hello@nedrysoft.com"

RUN dnf -y group install "C Development Tools and Libraries"
RUN dnf -y install \
    cmake \
    git \
    python-qt5-rpm-macros \
    qt5-linguist \
    rpm-build \
    rpmdevtools \
    qt5-qtquickcontrols2-devel \
    dbus-devel \
    vim \
    wget \
    unzip \
    rpm-sign \
    ruby \
    ruby-devel \
    qt5-devel

RUN ln -s /usr/lib64/qt5/bin/lrelease /bin/lrelease


