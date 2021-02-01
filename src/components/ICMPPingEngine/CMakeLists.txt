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

pingnoo_start_component()

pingnoo_set_component_optional(ON)

pingnoo_add_sources(
    ICMPPingComponent.cpp
    ICMPPingComponent.h
    ICMPPingEngine.cpp
    ICMPPingEngine.h
    ICMPPingEngineFactory.cpp
    ICMPPingEngineFactory.h
    ICMPPingEngineSpec.h
    ICMPPingItem.cpp
    ICMPPingItem.h
    ICMPPingTarget.cpp
    ICMPPingTarget.h
    ICMPPingTimeout.cpp
    ICMPPingTimeout.h
    ICMPPingTransmitter.cpp
    ICMPPingTransmitter.h
    ICMPPingReceiverWorker.cpp
    ICMPPingReceiverWorker.h
    Utils.h
)

pingnoo_set_description("ICMP ping engine component")

pingnoo_use_qt_libraries(Core Network)

pingnoo_use_component(Core)

pingnoo_use_shared_library(ComponentSystem)
pingnoo_use_shared_library(ICMPPacket)
pingnoo_use_shared_library(ICMPSocket)

pingnoo_set_component_metadata("Ping Engines" "Provides an socket based ICMP ping engine")

pingnoo_end_component()
