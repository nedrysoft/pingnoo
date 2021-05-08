#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of the Nedrysoft MacHelper library.
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

    # discover which Qt version is available

    if (USE_QT_VERSION)
        set(QT_VERSION_MAJOR ${USE_QT_VERSION})
    else()
        find_package(Qt6 COMPONENTS Core QUIET)
        find_package(Qt5 COMPONENTS Core QUIET)

        if (Qt6_FOUND)
            set(QT_VERSION_MAJOR 6)
        elseif(Qt5_FOUND)
            set(QT_VERSION_MAJOR 5)
        else()
            message(FATAL_ERROR, "No valid Qt version was set, and none could be found")
        endif()
    endif()

    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core QUIET)

    if (NOT Qt${QT_VERSION_MAJOR}_FOUND)
        message(FATAL_ERROR, "No valid Qt version was set, and none could be found")
    endif()

    # end of qt selection/detection

    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

    add_definitions(-DQT_NO_KEYWORDS)

    add_definitions(-DNEDRYSOFT_LIBRARY_MACHELPER_EXPORT)

    project(MacHelper)

    set(library_SOURCES
        includes/MacHelper/MacHelper.h
        includes/MacHelper/MenuBarIcon.h
        includes/MacHelper/Popover.h
        includes/MacHelper/PopoverHelper.h
        includes/MacHelper/StatusBarHelper.h
        src/MacHelper.mm
        src/MenuBarIcon.mm
        src/Popover.mm
        src/PopoverHelper.mm
        src/StatusBarHelper.mm
    )

    include_directories("src" "includes")

    add_library(${PROJECT_NAME} SHARED
        ${library_SOURCES}
    )

    target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})

    set(Qt_LIBS
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Gui)

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