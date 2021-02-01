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

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(helper)

include(FindPkgConfig)

pkg_check_modules(dbus REQUIRED dbus-1)

add_executable(helper
        helper.cpp
)

target_include_directories(helper PUBLIC ${dbus_INCLUDE_DIRS})
target_link_libraries(helper PUBLIC ${dbus_LIBRARIES})

if(DEFINED NEDRYSOFT_LIBRARY_DIR)
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${NEDRYSOFT_LIBRARY_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${NEDRYSOFT_LIBRARY_DIR}")
else()
    message(STATUS "Set NEDRYSOFT_LIBRARY_DIR to set the binary output dir.")
endif()
