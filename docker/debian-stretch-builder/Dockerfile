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

# docker build -f Dockerfile -t registry.fizzyade.com/debian-stretch-builder .
# docker save registry.fizzyade.com/debian-buster-builder | gzip > debian-stretch-builder.tar.gz
# docker import debian-stretch-builder.tar.gz
# docker push registry.fizzyade.com/debian-stretch-builder

FROM registry.fizzyade.com/debian-stretch-base

LABEL maintainer="hello@nedrysoft.com"

ENV DEBIAN_FRONTEND noninteractive

COPY setup.sh /tmp

RUN /tmp/setup.sh
