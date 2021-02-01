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

#if(NOT WIN32)
    return()
#endif()

pingnoo_start_component()

pingnoo_set_component_optional(ON)

pingnoo_add_sources(
    ICMPAPIPingComponent.cpp
    ICMPAPIPingComponent.h
    ICMPAPIPingEngine.cpp
    ICMPAPIPingEngine.h
    ICMPAPIPingEngineFactory.cpp
    ICMPAPIPingEngineFactory.h
    ICMPAPIPingEngineSpec.h
    ICMPAPIPingItem.cpp
    ICMPAPIPingItem.h
    ICMPAPIPingTarget.cpp
    ICMPAPIPingTarget.h
    ICMPAPIPingTransmitter.cpp
    ICMPAPIPingTransmitter.h
)

pingnoo_set_description("ICMPAPI ping engine component")

pingnoo_use_qt_libraries(Core Network)

pingnoo_use_component(Core)

pingnoo_use_shared_library(ComponentSystem)
pingnoo_use_shared_library(ICMPPacket)
pingnoo_use_shared_library(ICMPSocket)

pingnoo_set_component_metadata("Ping Engines" "Provides an socket based ICMP ping engine")

pingnoo_end_component()
