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

# this script pulls the latest images from my private registry and then saves it to a file.  Used specifically
# for the raspberry pi images which take a considerable amount of time to build

docker pull registry.fizzyade.com/raspbian-buster
docker pull registry.fizzyade.com/raspbian-buster-base
docker pull registry.fizzyade.com/raspbian-buster-builder
docker pull registry.fizzyade.com/raspbian-buster-agent

docker pull registry.fizzyade.com/raspbian-stretch
docker pull registry.fizzyade.com/raspbian-stretch-base
docker pull registry.fizzyade.com/raspbian-stretch-builder
docker pull registry.fizzyade.com/raspbian-stretch-agent

docker save registry.fizzyade.com/raspbian-buster | gzip > raspbian-buster.tar.gz
docker save registry.fizzyade.com/raspbian-buster-base | gzip > raspbian-buster-base.tar.gz
docker save registry.fizzyade.com/raspbian-buster-builder | gzip > raspbian-buster-builder.tar.gz
docker save registry.fizzyade.com/raspbian-buster-agent | gzip > raspbian-buster-agent.tar.gz

docker save registry.fizzyade.com/raspbian-stretch | gzip > raspbian-stretch.tar.gz
docker save registry.fizzyade.com/raspbian-stretch-base | gzip > raspbian-stretch-base.tar.gz
docker save registry.fizzyade.com/raspbian-stretch-builder | gzip > raspbian-stretch-builder.tar.gz
docker save registry.fizzyade.com/raspbian-stretch-agent | gzip > raspbian-stretch-agent.tar.gz