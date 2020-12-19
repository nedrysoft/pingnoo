#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
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
set(NEDRYSOFT_RIBBON_BUILD_DESIGNER_PLUGIN ${APPLICATION_BINARY_FOLDER})

add_definitions("-DUSE_SPDLOG")

# add spdlog to global includes, libraries will inherit this and can then make use of it

include_directories(PUBLIC ${PINGNOO_LIBRARIES_SOURCE_DIR}/spdlog/include)

# find libraries and add to build

file(GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)

foreach (child ${children})
    if (IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${child})
        if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${child}/CMakeLists.txt)
            add_subdirectory(${child})
        endif ()
    endif ()
endforeach ()


