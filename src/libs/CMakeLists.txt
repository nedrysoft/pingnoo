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

# set any variables required for libraries

set(NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER ON)
set(NEDRYSOFT_SETTINGSDIALOG_LIBRARY_DIR ${PINGNOO_LIBRARIES_BINARY_DIR})
set(NEDRYSOFT_COMPONENTSYSTEM_LIBRARY_DIR ${PINGNOO_LIBRARIES_BINARY_DIR})
set(NEDRYSOFT_RIBBON_LIBRARY_DIR ${PINGNOO_LIBRARIES_BINARY_DIR})
set(NEDRYSOFT_FONTAWESOME_LIBRARY_DIR ${PINGNOO_LIBRARIES_BINARY_DIR})
set(NEDRYSOFT_MACHELPER_LIBRARY_DIR ${PINGNOO_LIBRARIES_BINARY_DIR})
set(NEDRYSOFT_MACHELPER_INCLUDE_DIR ${PINGNOO_LIBRARIES_SOURCE_DIR}/MacHelper/includes)
set(NEDRYSOFT_RIBBON_BUILD_DESIGNER_PLUGIN ${APPLICATION_BINARY_FOLDER})
set(NEDRYSOFT_THEMESUPPORT_LIBRARY_DIR ${PINGNOO_LIBRARIES_BINARY_DIR})
set(NEDRYSOFT_THEMESUPPORT_INCLUDE_DIR ${PINGNOO_LIBRARIES_SOURCE_DIR}/ThemeSupport/includes)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(NEDRYSOFT_LIB_GIT_YEAR ${PINGNOO_GIT_YEAR})
set(NEDRYSOFT_LIB_GIT_MONTH ${PINGNOO_GIT_MONTH})
set(NEDRYSOFT_LIB_GIT_DAY ${PINGNOO_GIT_DAY})
set(NEDRYSOFT_LIB_GIT_HASH ${PINGNOO_GIT_HASH})
set(NEDRYSOFT_LIB_GIT_BRANCH ${PINGNOO_GIT_BRANCH})

add_definitions("-DUSE_SPDLOG")

# add spdlog to global includes, libraries will inherit this and can then make use of it

include_directories(SYSTEM 3rdparty/spdlog/include)

# libraries to build

add_subdirectory(3rdparty)

add_subdirectory(AppNap)
add_subdirectory(ThemeSupport)
add_subdirectory(ComponentSystem)
add_subdirectory(FontAwesome)
add_subdirectory(ICMPPacket)
add_subdirectory(ICMPSocket)
add_subdirectory(MacHelper)
add_subdirectory(MapWidget)
add_subdirectory(Ribbon)
add_subdirectory(SettingsDialog)


