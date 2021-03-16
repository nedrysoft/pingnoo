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

project(Pingnoo)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/pingnoo.cmake)

option(Pingnoo_Build_Tests "Build tests" OFF)

add_subdirectory(src/libs)
add_subdirectory(src/components)

if (${Pingnoo_Build_Tests})
    add_subdirectory(src/tests)
else()
    add_subdirectory(src/app)
endif()

if(UNIX AND NOT APPLE)
    set(NEDRYSOFT_LIBRARY_DIR ${PINGNOO_BINARY_ROOT})

    add_subdirectory(src/helper)
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} PINGNOO_BUILD_TYPE)

# create translations

file(GLOB translations RELATIVE ${PINGNOO_ROOT_DIR}/translations ${PINGNOO_ROOT_DIR}/translations/*.ts)

file(MAKE_DIRECTORY ${PINGNOO_RESOURCES_DIR}/translations)

set(translatedFiles "")

foreach (translation ${translations})
    pingnoo_add_translation(${PINGNOO_ROOT_DIR}/translations/${translation} ${PINGNOO_RESOURCES_DIR}/translations translatedFiles)
endforeach()

add_custom_target(
    Translations
    ALL DEPENDS
    ${translatedFiles}
)

if(APPLE)
    install(CODE
        "execute_process(
            WORKING_DIRECTORY \"${PINGNOO_ROOT_DIR}\"
            COMMAND \"${PINGNOO_ROOT_DIR}/deploy.py\"
            \"--appleid=$ENV{PINGNOO_DEVELOPER_EMAIL}\"
            \"--password=$ENV{PINGNOO_DEVELOPER_PASSWORD}\"
            \"--cert=$ENV{PINGNOO_DEVELOPER_CERTIFICATE}\"
            \"--arch=${PINGNOO_PLATFORM_ARCH}\"
            \"--type=${PINGNOO_BUILD_TYPE}\"
            \"--qtdir=${CMAKE_PREFIX_PATH}\"
            \"--version=${PINGNOO_GIT_YEAR}.${PINGNOO_GIT_MONTH}.${PINGNOO_GIT_DAY}-${PINGNOO_GIT_BRANCH}\"
        )"
        )
elseif(WIN32)
    file(TO_NATIVE_PATH "${PINGNOO_ROOT_DIR}" rootDir)
    file(TO_NATIVE_PATH "$ENV{PINGNOO_CURL_BINARY}" curlBin)

    if(NOT DEFINED ENV{PINGNOO_PYTHON_BINARY})
        set(pythonBin python)
    else()
        file(TO_NATIVE_PATH "$ENV{PINGNOO_PYTHON_BINARY}" pythonBin)
        string(REPLACE "\\" "\\\\" pythonBin ${pythonBin})
    endif()

    file(TO_NATIVE_PATH "${CMAKE_PREFIX_PATH}" qtPath)

    string(REPLACE "\\" "\\\\" curlBin ${curlBin})
    string(REPLACE "\\" "\\\\" rootDir ${rootDir})
    string(REPLACE "\\" "\\\\" qtPath ${qtPath})

    install(CODE
        "execute_process(
            WORKING_DIRECTORY ${PINGNOO_ROOT_DIR}
            COMMAND ${pythonBin}
            ${rootDir}\\\\deploy.py
            --curlbin=${curlBin}
            --qtdir=${qtPath}
            --type=${PINGNOO_BUILD_TYPE}
            --arch=${PINGNOO_PLATFORM_ARCH}
            --version=${PINGNOO_GIT_YEAR}.${PINGNOO_GIT_MONTH}.${PINGNOO_GIT_DAY}-${PINGNOO_GIT_BRANCH}
            --portable
        )"
        )
elseif(UNIX)
    install(CODE
        "execute_process(
            WORKING_DIRECTORY \"${PINGNOO_ROOT_DIR}\"
            COMMAND \"${PINGNOO_ROOT_DIR}/deploy.py\"
            \"--curlbin=/usr/bin/curl\"
            \"--arch=${PINGNOO_PLATFORM_ARCH}\"
            \"--qtdir=${CMAKE_PREFIX_PATH}\"
            \"--type=${PINGNOO_BUILD_TYPE}\"
            \"--version=${PINGNOO_GIT_YEAR}.${PINGNOO_GIT_MONTH}.${PINGNOO_GIT_DAY}-${PINGNOO_GIT_BRANCH}\"
            \"--cert=$ENV{PINGNOO_DEVELOPER_CERTIFICATE}\"
            \"$ENV{PINGNOO_ASSET_TYPES}\"
        )"
        )
endif()
