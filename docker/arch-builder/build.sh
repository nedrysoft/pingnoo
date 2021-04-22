#!/usr/bin/env bash

#
# Copyright (C) 2021 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 22/04/2021.
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

DOCKER_REGISTRY=registry.fizzyade.com
DOCKER_IMAGE=${PWD##*/}

DOCKER_USER=${DOCKER_USER:-teamcity}
DOCKER_GROUP=${DOCKER_GROUP:-teamcity}

DOCKER_USER_ID=$(id $DOCKER_USER -u)
DOCKER_GROUP_ID=$(id $DOCKER_USER -g)

if [ -z "$DOCKER_USER_ID" ]
then
  echo "error: could not find the user $DOCKER_USER."
  exit 1
fi

if [ -z "$DOCKER_GROUP_ID" ]
then
  echo "error: could not find the group $DOCKER_USER."
  exit 1
fi

docker build \
       -f Dockerfile \
       --build-arg DOCKER_USER=$DOCKER_USER \
       --build-arg DOCKER_GROUP=$DOCKER_GROUP \
       --build-arg DOCKER_USER_ID=$DOCKER_USER_ID \
       --build-arg DOCKER_GROUP_ID=$DOCKER_GROUP_ID \
       -t $DOCKER_REGISTRY/$DOCKER_IMAGE \
       .

docker push $DOCKER_REGISTRY/$DOCKER_IMAGE