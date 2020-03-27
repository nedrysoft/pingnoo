#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
# An open source ping path analyser
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
    Cache.cpp
    Cache.h
    IPAPIGeoIPProvider.cpp
    IPAPIGeoIPProvider.h
    IPAPIGeoIPProviderComponent.cpp
    IPAPIGeoIPProviderComponent.h
    IPAPIGeoIPProviderSpec.h
)

pingnoo_use_qt_libraries(Network Sql)

pingnoo_use_component(Core)

pingnoo_use_shared_library(ComponentSystem)

pingnoo_set_component_metadata("1.0.0" "Geo IP Providers" "Provides an ip-api geo lookup")

pingnoo_end_component()
