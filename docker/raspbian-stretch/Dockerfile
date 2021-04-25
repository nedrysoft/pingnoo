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

# docker build -f Dockerfile -t registry.fizzyade.com/raspbian-stretch .
# docker save raspbian.fizzyade.com/debian-stretch | gzip > raspbian-stretch.tar.gz
# docker import raspbian-stretch.tar.gz
# docker push registry.fizzyade.com/raspbian-stretch

FROM scratch

LABEL maintainer="hello@nedrysoft.com"

ADD rootfs.tar.xz /

ENV DEBIAN_FRONTEND noninteractive

