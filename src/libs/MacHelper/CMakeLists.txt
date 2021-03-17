#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of the Nedrysoft SettingsDialog. (https://github.com/nedrysoft/SettingsDialog)
#
# A cross-platform settings dialog
#
# Created by Adrian Carpenter on 07/12/2020.
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

if(APPLE)
    cmake_minimum_required(VERSION 3.10)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    find_package(Qt5 COMPONENTS Core Gui Widgets MacExtras REQUIRED)

    add_definitions(-DQT_NO_KEYWORDS)

    add_definitions(-DNEDRYSOFT_LIBRARY_MACHELPER_EXPORT)

    project(MacHelper)

    set(library_SOURCES
        includes/MacHelper/MacHelper.h
        src/MacHelper.mm
    )

    include_directories("src" "includes")

    add_library(${PROJECT_NAME} SHARED
        ${library_SOURCES}
    )

    target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})

    set(Qt_LIBS Qt5::Core Qt5::Widgets Qt5::Gui Qt5::MacExtras)

    if(DEFINED NEDRYSOFT_MACHELPER_LIBRARY_DIR)
        set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${NEDRYSOFT_MACHELPER_LIBRARY_DIR}")
        set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${NEDRYSOFT_MACHELPER_LIBRARY_DIR}")
    else()
        message(STATUS "Set NEDRYSOFT_MACHELPER_LIBRARY_DIR to set the binary output dir.")
    endif()

    target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})
    target_link_libraries(${PROJECT_NAME} "-framework AppKit" "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} stdc++ objc)
endif()