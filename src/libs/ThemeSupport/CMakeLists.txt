#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 31/03/2021.
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

pingnoo_start_shared_library()

pingnoo_add_sources(
    ThemeSupport.cpp
    includes/ThemeSupport.h
    ../3rdparty/QDarkStyleSheet/qdarkstyle/light/style.qrc
    ../3rdparty/QDarkStyleSheet/qdarkstyle/dark/style.qrc
)

if(APPLE)
    pingnoo_add_sources(ThemeSupport_mac.mm)
elseif(UNIX)
    pingnoo_add_sources(ThemeSupport_linux.cpp)
elseif(WIN32)
    pingnoo_add_sources(ThemeSupport_windows.cpp)
endif()

pingnoo_set_description("Theme Support extension")

pingnoo_use_qt_libraries(Core Gui Widgets)

pingnoo_use_frameworks("Foundation" "Cocoa")
pingnoo_use_system_libraries(APPLE stdc++ objc)

pingnoo_end_shared_library()

