#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 22/01/2021.
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

if(NOT (UNIX AND NOT APPLE))
    return()
endif()

pingnoo_start_component()

pingnoo_set_component_optional(ON)

pingnoo_add_sources(
        PingCommandPingComponent.cpp
        PingCommandPingComponent.h
        PingCommandPingEngine.cpp
        PingCommandPingEngine.h
        PingCommandPingEngineFactory.cpp
        PingCommandPingEngineFactory.h
        PingCommandPingEngineSpec.h
        PingCommandPingTarget.cpp
        PingCommandPingTarget.h
)

pingnoo_set_description("Ping command ping engine component")

pingnoo_use_component(Core)

pingnoo_use_qt_libraries(Core)

pingnoo_use_shared_library(ComponentSystem)

pingnoo_set_component_metadata("Ping Engines" "Provides a ping engine using the operating system ping executable")

pingnoo_end_component()
