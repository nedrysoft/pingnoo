#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 27/03/2020.
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

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/pingnoo.cmake)

add_subdirectory(Core)
add_subdirectory(HostIPGeoIPProvider)
add_subdirectory(ICMPAPIPingEngine)
add_subdirectory(ICMPPingEngine)
add_subdirectory(PingCommandPingEngine)
add_subdirectory(PublicIPHostMasker)
add_subdirectory(RegExHostMasker)
add_subdirectory(RouteAnalyser)
add_subdirectory(RouteEngine)

set(NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER ON)

