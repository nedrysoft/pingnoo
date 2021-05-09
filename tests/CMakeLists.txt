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

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

project(Tests)

# discover which Qt version is available

if (NOT DEFINED QT_VERSION_MAJOR)
    if (DEFINED USE_QT_VERSION)
        set(QT_VERSION_MAJOR ${USE_QT_VERSION})
        message(STATUS "Qt${QT_VERSION_MAJOR} has been manually selected")
    else()
        message(STATUS "Detecting Qt version")

        find_package(Qt6 COMPONENTS Core QUIET)
        find_package(Qt5 COMPONENTS Core QUIET)

        if (Qt6_FOUND)
            set(QT_VERSION_MAJOR 6)
        elseif(Qt5_FOUND)
            set(QT_VERSION_MAJOR 5)
        else()
            message(FATAL_ERROR "No valid Qt version was set, and none could be found")
        endif()
        message(STATUS "Detecting Qt version - done")
    endif()

    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core QUIET)

    if (NOT Qt${QT_VERSION_MAJOR}_FOUND)
        message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} could not be found")
    endif()

    message(STATUS "Qt major version: ${QT_VERSION_MAJOR}")
endif()

# end of qt selection/detection

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network Test REQUIRED)

file(GLOB_RECURSE test_COMPONENTS "components/*.cpp" "components/*.qrc" "compoennts/*.ui")
file(GLOB_RECURSE test_LIBRARIES "libs/*.cpp" "libs/*.qrc" "libs/*.ui")

set(test_SOURCES
    main.cpp
    ${test_COMPONENTS}
    ${test_LIBRARIES}
)

set(Qt_LIBS
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Test)

add_executable(${PROJECT_NAME} ${test_SOURCES})

target_link_libraries(${PROJECT_NAME} "-L${PINGNOO_LIBRARIES_BINARY_DIR}"
    -lComponentSystem
    -lICMPPacket
    -lICMPSocket
)

target_compile_definitions(${PROJECT_NAME} PUBLIC "-DPINGNOO_TEST_LIBS_DIR=\"${PINGNOO_LIBRARIES_BINARY_DIR}\"")
target_compile_definitions(${PROJECT_NAME} PUBLIC "-DPINGNOO_TEST_COMPONENTS_DIR=\"${PINGNOO_COMPONENTS_BINARY_DIR}\"")

include_directories(${PINGNOO_SOURCE_DIR}/libs/Catch2)
include_directories(${PINGNOO_SOURCE_DIR}/libs/spdlog/include)

target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})
