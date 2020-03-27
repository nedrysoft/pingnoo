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

cmake_minimum_required(VERSION 3.10)

project(Pingnoo)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/pingnoo.cmake)

option(Pingnoo_Build_Tests "Build tests" OFF)

add_subdirectory(src/libs)
add_subdirectory(src/components)

if (Pingnoo_Build_Tests)
    add_subdirectory(src/tests)
else()
    add_subdirectory(src/app)
endif()

