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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PINGNOO_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

set(PINGNOO_BINARY_DIR "${PINGNOO_ROOT_DIR}/bin")
set(PINGNOO_SOURCE_DIR "${PINGNOO_ROOT_DIR}/src")

execute_process(COMMAND git --git-dir=${PINGNOO_ROOT_DIR}/.git log -1 --format=%cd --date=format:%y OUTPUT_VARIABLE PINGNOO_GIT_YEAR)
execute_process(COMMAND git --git-dir=${PINGNOO_ROOT_DIR}/.git log -1 --format=%cd --date=format:%m OUTPUT_VARIABLE PINGNOO_GIT_MONTH)
execute_process(COMMAND git --git-dir=${PINGNOO_ROOT_DIR}/.git log -1 --format=%cd --date=format:%d OUTPUT_VARIABLE PINGNOO_GIT_DAY)
execute_process(COMMAND git --git-dir=${PINGNOO_ROOT_DIR}/.git log -1 --format=%h OUTPUT_VARIABLE PINGNOO_GIT_HASH)
execute_process(COMMAND git --git-dir=${PINGNOO_ROOT_DIR}/.git branch --show-current OUTPUT_VARIABLE PINGNOO_GIT_BRANCH)
execute_process(COMMAND git --git-dir=${PINGNOO_ROOT_DIR}/.git diff --quiet HEAD RESULT_VARIABLE PINGNOO_GIT_UNCOMMITED)

if (PINGNOO_GIT_UNCOMMITED EQUAL 1)
    set(PINGNOO_GIT_UNCOMMITED "+")
else()
    set(PINGNOO_GIT_UNCOMMITED "")
endif()

if ("${PINGNOO_GIT_YEAR}" STREQUAL "")
    set(PINGNOO_GIT_YEAR 0)
    set(PINGNOO_GIT_MONTH 0)
    set(PINGNOO_GIT_DAY 0)
    set(PINGNOO_GIT_BRANCH none)
    set(PINGNOO_GIT_HASH 0)
else()
    string(STRIP "${PINGNOO_GIT_YEAR}" PINGNOO_GIT_YEAR)
    string(STRIP "${PINGNOO_GIT_MONTH}" PINGNOO_GIT_MONTH)
    string(STRIP "${PINGNOO_GIT_DAY}" PINGNOO_GIT_DAY)
    string(STRIP "${PINGNOO_GIT_BRANCH}" PINGNOO_GIT_BRANCH)
    string(STRIP "${PINGNOO_GIT_HASH}" PINGNOO_GIT_HASH)
endif()

add_definitions("-DPINGNOO_GIT_YEAR=\"${PINGNOO_GIT_YEAR}\"")
add_definitions("-DPINGNOO_GIT_MONTH=\"${PINGNOO_GIT_MONTH}\"")
add_definitions("-DPINGNOO_GIT_DAY=\"${PINGNOO_GIT_DAY}\"")
add_definitions("-DPINGNOO_GIT_BRANCH=\"${PINGNOO_GIT_BRANCH}\"")
add_definitions("-DPINGNOO_GIT_HASH=\"${PINGNOO_GIT_HASH}\"")

if (WIN32)
    set(PINGNOO_PLATFORM_TARGET "Windows")
elseif(APPLE)
    set(PINGNOO_PLATFORM_TARGET "Mac")
else()
    set(PINGNOO_PLATFORM_TARGET "Linux")
endif()

if((APPLE) AND (CMAKE_OSX_ARCHITECTURES MATCHES "arm64"))
    set(PINGNOO_PLATFORM_ARCH "arm64")
else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PINGNOO_PLATFORM_ARCH "x86_64")
    else()
        set(PINGNOO_PLATFORM_ARCH "x86_32")
    endif()
endif()

set(PINGNOO_LIBRARIES_SOURCE_DIR "${PINGNOO_SOURCE_DIR}/libs")
set(PINGNOO_COMPONENTS_SOURCE_DIR "${PINGNOO_SOURCE_DIR}/components")

if(APPLE)
    set(PINGNOO_BINARY_ROOT "${PINGNOO_BINARY_DIR}/${PINGNOO_PLATFORM_ARCH}/${CMAKE_BUILD_TYPE}")
    set(PINGNOO_APPLICATION_BINARY "${PINGNOO_BINARY_ROOT}/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}")
    set(PINGNOO_LIBRARIES_BINARY_DIR "${PINGNOO_BINARY_ROOT}/${PROJECT_NAME}.app/Contents/Frameworks")
    set(PINGNOO_COMPONENTS_BINARY_DIR "${PINGNOO_BINARY_ROOT}/${PROJECT_NAME}.app/Contents/PlugIns")
else()
    set(PINGNOO_BINARY_ROOT "${PINGNOO_BINARY_DIR}/${PINGNOO_PLATFORM_ARCH}/${CMAKE_BUILD_TYPE}")
    set(PINGNOO_APPLICATION_BINARY "${PINGNOO_BINARY_ROOT}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
    set(PINGNOO_LIBRARIES_BINARY_DIR "${PINGNOO_BINARY_ROOT}")
    set(PINGNOO_COMPONENTS_BINARY_DIR "${PINGNOO_BINARY_ROOT}/Components")
endif()

file(MAKE_DIRECTORY ${PINGNOO_LIBRARIES_BINARY_DIR})
file(MAKE_DIRECTORY ${PINGNOO_COMPONENTS_BINARY_DIR})

include_directories(${PINGNOO_LIBRARIES_SOURCE_DIR}/ComponentSystem/src)
include_directories(${PINGNOO_LIBRARIES_SOURCE_DIR}/ComponentSystem/includes)
include_directories(${PINGNOO_LIBRARIES_SOURCE_DIR}/SettingsDialog/includes)
include_directories(${PINGNOO_LIBRARIES_SOURCE_DIR})
include_directories(${PINGNOO_COMPONENTS_SOURCE_DIR})

link_directories(${PINGNOO_LIBRARIES_BINARY_DIR})
link_directories(${PINGNOO_COMPONENTS_BINARY_DIR})

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

macro(pingnoo_use_component componentName)
    target_link_libraries(${PROJECT_NAME} ${componentName})

    list(APPEND pingnooComponentDependenciesList ${componentName})
endmacro(pingnoo_use_component)

macro(pingnoo_use_shared_library libraryName)
    target_link_libraries(${PROJECT_NAME} ${libraryName})
endmacro(pingnoo_use_shared_library)

macro(pingnoo_use_static_library libraryName)
    target_link_libraries(${PROJECT_NAME} ${libraryName})
endmacro(pingnoo_use_static_library)

macro(pingnoo_set_library_outputs)
    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PINGNOO_LIBRARIES_BINARY_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PINGNOO_LIBRARIES_BINARY_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PINGNOO_LIBRARIES_BINARY_DIR}")
endmacro(pingnoo_set_library_outputs)

macro(pingnoo_set_component_outputs)
    set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PINGNOO_COMPONENTS_BINARY_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PINGNOO_COMPONENTS_BINARY_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PINGNOO_COMPONENTS_BINARY_DIR}")
endmacro(pingnoo_set_component_outputs)

macro(pingnoo_set_optional prefix defaultValue)
    option("${prefix}_${PROJECT_NAME}" "Build ${PROJECT_NAME} Component" ${defaultValue})

    set(PINGNOO_OPTIONAL_BUILD ${${prefix}_${PROJECT_NAME}})
endmacro(pingnoo_set_optional)

macro(pingnoo_set_component_optional defaultValue)
    pingnoo_set_optional("Component" ${defaultValue})

    if (NOT ${PINGNOO_OPTIONAL_BUILD})
        return()
    endif()
endmacro(pingnoo_set_component_optional)

macro(pingnoo_start_component)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    get_filename_component(pingnooCurrentProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    project(${pingnooCurrentProjectName})

    set(pingnooCurrentProjectType "SHARED")

    string(TOUPPER ${pingnooCurrentProjectName} pingnooCurrentProjectNameUpperCase)

    add_definitions("-DNEDRYSOFT_COMPONENT_${pingnooCurrentProjectNameUpperCase}_EXPORT")
    add_definitions("-DNEDRYSOFT_MODULE_FILENAME=\"${pingnooCurrentProjectName}.dll\"")
    add_definitions("-DNEDRYSOFT_MODULE_NAME=\"${pingnooCurrentProjectName}\"")

    set(pingnooComponentDependenciesList "")
endmacro(pingnoo_start_component)

macro(pingnoo_end_component)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/metadata.json.in" AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/metadata.json.in")
        include_directories(${CMAKE_CURRENT_BINARY_DIR})
        configure_file("metadata.json.in" "metadata.json")
    endif()

    if (WIN32)
        target_sources(${pingnooCurrentProjectName} "PRIVATE" "${PINGNOO_SOURCE_DIR}/common/version.rc")
    endif()

    pingnoo_set_component_outputs()

    add_logging_library()

    target_include_directories(${pingnooCurrentProjectName} PRIVATE ".")

    pingnoo_sign(\"${PINGNOO_COMPONENTS_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${pingnooCurrentProjectName}${CMAKE_SHARED_LIBRARY_SUFFIX}\")
endmacro(pingnoo_end_component)

macro(pingnoo_start_shared_library)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    get_filename_component(pingnooCurrentProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    project(${pingnooCurrentProjectName})

    set(pingnooCurrentProjectType "SHARED")

    string(TOUPPER ${pingnooCurrentProjectName} pingnooCurrentProjectNameUpperCase)

    add_definitions("-DNEDRYSOFT_LIBRARY_${pingnooCurrentProjectNameUpperCase}_EXPORT")
    add_definitions("-DNEDRYSOFT_MODULE_FILENAME=\"${pingnooCurrentProjectName}.dll\"")
    add_definitions("-DNEDRYSOFT_MODULE_NAME=\"${pingnooCurrentProjectName}\"")
endmacro(pingnoo_start_shared_library)

macro(pingnoo_end_shared_library)
    if(UNIX)
        target_compile_options(${pingnooCurrentProjectName} PRIVATE -Wno-deprecated-declarations -Wno-deprecated)
    endif()

    if (WIN32)
        target_sources(${pingnooCurrentProjectName} "PRIVATE" "${PINGNOO_SOURCE_DIR}/common/version.rc")
    endif()

    pingnoo_set_library_outputs()

    add_logging_library()

    target_include_directories(${pingnooCurrentProjectName} PRIVATE ".")

    pingnoo_sign(\"${PINGNOO_LIBRARIES_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${pingnooCurrentProjectName}${CMAKE_SHARED_LIBRARY_SUFFIX}\")
endmacro(pingnoo_end_shared_library)

macro(pingnoo_start_static_library)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    get_filename_component(pingnooCurrentProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    project(${pingnooCurrentProjectName})

    set(pingnooCurrentProjectType "STATIC")
endmacro(pingnoo_start_static_library)

macro(pingnoo_end_static_library)
    if (APPLE)
        SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
        SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
        SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
        SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
    endif()

    add_logging_library()

    target_include_directories(${pingnooCurrentProjectName} PRIVATE ".")

    pingnoo_set_library_outputs()
endmacro(pingnoo_end_static_library)

macro(pingnoo_start_executable)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    if (NOT ${ARGC})
        get_filename_component(pingnooCurrentProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    else()
        set(pingnooCurrentProjectName ${ARGV0})
    endif()

    project(${pingnooCurrentProjectName})

    set(pingnooCurrentProjectType "EXECUTABLE")

    set(CMAKE_BUILD_RPATH "@executable_path/../Frameworks")

    add_definitions("-DNEDRYSOFT_MODULE_FILENAME=\"${pingnooCurrentProjectName}.exe\"")
    add_definitions("-DNEDRYSOFT_MODULE_NAME=\"${pingnooCurrentProjectName}\"")
endmacro(pingnoo_start_executable)

macro(pingnoo_end_executable)
    set_target_properties(${pingnooCurrentProjectName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PINGNOO_BINARY_ROOT}")
    set_target_properties(${pingnooCurrentProjectName} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in)

    if (WIN32)
        set_property(TARGET ${pingnooCurrentProjectName} PROPERTY WIN32_EXECUTABLE true)
        target_sources(${pingnooCurrentProjectName} "PRIVATE" "${PINGNOO_SOURCE_DIR}/common/version.rc")
    endif()

    add_logging_library()

    target_include_directories(${pingnooCurrentProjectName} PRIVATE ".")

    pingnoo_sign(${PINGNOO_APPLICATION_BINARY})
endmacro(pingnoo_end_executable)

macro(add_logging_library)
    include_directories("${PINGNOO_LIBRARIES_SOURCE_DIR}/spdlog/include")

    add_dependencies(${PROJECT_NAME} spdlog)

    set(debug_SUFFIX)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(NOT APPLE)
        set(debug_SUFFIX "d")
    endif()
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} \"${spdlog_BINARY_DIR}\\spdlog${debug_SUFFIX}\")
else()
    target_link_libraries(${PROJECT_NAME} -L${spdlog_BINARY_DIR} -lspdlog${debug_SUFFIX})
endif()
endmacro(add_logging_library)

macro(pingnoo_use_qt_libraries)
    set(pingnooFindPackageList "")
    set(pingnooLinkPackageList "")

    list(APPEND pingnooFindPackageList "Qt5" "COMPONENTS")
    list(APPEND pingnooLinkPackageList "${pingnooCurrentProjectName}")

    foreach(arg IN ITEMS ${ARGN})
        list(APPEND pingnooFindPackageList "${arg}")
        list(APPEND pingnooLinkPackageList "Qt5::${arg}")
    endforeach()

    list(APPEND pingnooFindPackageList "REQUIRED")

    find_package(${pingnooFindPackageList})
    target_link_libraries(${pingnooLinkPackageList})
endmacro(pingnoo_use_qt_libraries)

macro(pingnoo_add_sources)
    set(pingnooSourceList "")
    set(pingnooMacIcons FALSE)

    foreach(arg IN ITEMS ${ARGN})
        get_filename_component(fileExtension ${arg} EXT)

        if (fileExtension STREQUAL ".mm")
            if (APPLE)
                list(APPEND pingnooSourceList "${arg}")
            endif()
        elseif(fileExtension STREQUAL ".rc")
            if (WIN32)
                list(APPEND pingnooSourceList "${arg}")
            endif()
        elseif(fileExtension STREQUAL ".icns")
            if (APPLE)
                list(APPEND pingnooSourceList "${arg}")
                set(pingnooMacIcons TRUE)
            endif()
        else()
            list(APPEND pingnooSourceList "${arg}")
        endif()
    endforeach()

    if (NOT pingnooTarget_${pingnooCurrentProjectName})
        if(pingnooCurrentProjectType STREQUAL "EXECUTABLE")
            list(APPEND pingnooSourceList "${PINGNOO_SOURCE_DIR}/includes/Pingnoo.h")
            if(APPLE)
                add_executable(${pingnooCurrentProjectName} MACOSX_BUNDLE ${pingnooSourceList})
                if(pingnooMacIcons)
                    set(MACOSX_BUNDLE_ICON_FILE appicon.icns)
                    set_source_files_properties(appicon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
                endif()
            else()
                add_executable(${pingnooCurrentProjectName} ${pingnooSourceList})
            endif()
            target_include_directories(${pingnooCurrentProjectName} PRIVATE ${PINGNOO_SOURCE_DIR}/includes)
        else()
            list(APPEND pingnooSourceList "${PINGNOO_SOURCE_DIR}/includes/Pingnoo.h")
            add_library(${pingnooCurrentProjectName} ${pingnooCurrentProjectType} ${pingnooSourceList})
            target_include_directories(${pingnooCurrentProjectName} PRIVATE ${PINGNOO_SOURCE_DIR}/includes)
        endif()

        set(pingnooTarget_${pingnooCurrentProjectName} TRUE)
    else()
        target_sources(${pingnooCurrentProjectName} "PRIVATE" "${pingnooSourceList}")
    endif()
endmacro(pingnoo_add_sources)

macro(pingnoo_use_frameworks)
    if(APPLE)
        set(pingnooFrameworkList "")

        list(APPEND pingnooFrameworkList "${pingnooCurrentProjectName}")

        foreach(arg IN ITEMS ${ARGN})
            list(APPEND pingnooFrameworkList "-framework ${arg}")
        endforeach()

        target_link_libraries(${pingnooFrameworkList})
    endif()
endmacro(pingnoo_use_frameworks)

macro(pingnoo_use_system_libraries platform)
    if (${platform})
        set(pingnooSystemLibraryList "")

        list(APPEND pingnooSystemLibraryList "${pingnooCurrentProjectName}")

        foreach(arg IN ITEMS ${ARGN})
            list(APPEND pingnooSystemLibraryList "${arg}")
        endforeach()

        target_link_libraries(${pingnooSystemLibraryList})
    endif()
endmacro(pingnoo_use_system_libraries)

macro(pingnoo_add_compiler_options platform)
    if (${platform})
        set(pingnooOptionList "")

        foreach(arg IN ITEMS ${ARGN})
            list(APPEND pingnooOptionList "${arg}")
        endforeach()

        target_compile_options(${pingnooCurrentProjectName} PRIVATE ${pingnooOptionList})
    endif()
endmacro(pingnoo_add_compiler_options)

macro(pingnoo_add_defines)
    set(pingnooDefineList "")

    foreach(arg IN ITEMS ${ARGN})
        list(APPEND pingnooDefineList -D${arg})
    endforeach()

    add_definitions(${pingnooDefineList})
endmacro(pingnoo_add_defines)

macro(pingnoo_set_component_metadata pingnooComponentCategory pingnooComponentDescription)
    set(pingnooComponentName ${pingnooCurrentProjectName})
    set(pingnooComponentVersion ${PINGNOO_GIT_YEAR}.${PINGNOO_GIT_MONTH}.${PINGNOO_GIT_YEAR})
    set(pingnooComponentBranch ${PINGNOO_GIT_BRANCH})
    set(pingnooComponentRevision ${PINGNOO_GIT_HASH}${PINGNOO_GIT_UNCOMMITED})
    set(pingnooComponentCategory ${pingnooComponentCategory})
    set(pingnooComponentDescription ${pingnooComponentDescription})
    set(pingnooComponentDependencies "")

    if (pingnooComponentDependenciesList)
        set(isFirst TRUE)
        foreach(componentName IN ITEMS ${pingnooComponentDependenciesList})
            if (NOT isFirst)
                string(APPEND pingnooComponentDependencies ",\r\n")
            endif()

            set(isFirst FALSE)

            string(APPEND pingnooComponentDependencies "            \{ \"Name\" : \"${componentName}\", \"Version\" : \"1.0.0\" \}")
        endforeach()
    endif()
endmacro(pingnoo_set_component_metadata)

macro(pingnoo_set_description description)
    add_definitions("-DVER_FILEDESCRIPTION_STR=\"${description}\"")
endmacro()

macro(pingnoo_add_optional_command systemName optionName optionDescription optionDefault)
    if(${CMAKE_SYSTEM_NAME} STREQUAL ${systemName})
        option(Pingnoo_${optionName} optionDescription optionDefault)

        if (Pingnoo_${optionName})
            set(commandList "")

            foreach(arg IN ITEMS ${ARGN})
                list(APPEND commandList ${arg})
            endforeach()

            add_custom_command(TARGET ${pingnooCurrentProjectName} ${commandList})
        endif()
    endif()
endmacro()

macro(pingnoo_sign filename)

endmacro()
