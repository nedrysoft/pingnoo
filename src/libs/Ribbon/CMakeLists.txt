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

pingnoo_start_shared_library()

pingnoo_add_sources(
    ThemeSupport.mm
    ThemeSupport.h
    RibbonFontManager.cpp
    RibbonFontManager.h
    RibbonFontsResources.qrc
    RibbonGroup.cpp
    RibbonGroup.h
    RibbonTabBar.cpp
    RibbonTabBar.h
    RibbonWidget.cpp
    RibbonWidget.h
    RibbonSpec.h
)

pingnoo_use_frameworks("AppKit" "Cocoa")
pingnoo_use_system_libraries(APPLE stdc++ objc)

pingnoo_use_qt_libraries(Widgets)

pingnoo_end_shared_library()

# Ribbon Designer Plugin

option(Pingnoo_Build_RibbonDesignerPlugin "Build Ribbon QtDesigner Plugin" OFF)

if(Pingnoo_Build_RibbonDesignerPlugin)
    project(FizzyAde.Ribbon)

    add_definitions(-DRIBBON_DESIGNER_EXPORT)

    find_package(Qt5 COMPONENTS Widgets UiPlugin REQUIRED)

    add_library(${PROJECT_NAME} SHARED
        RibbonFontManager.cpp
        RibbonFontManager.h
        RibbonFontsResources.qrc
        RibbonGroup.cpp
        RibbonGroup.h
        RibbonTabBar.cpp
        RibbonTabBar.h
        RibbonWidget.cpp
        RibbonWidget.h
        RibbonWidgetPlugin.cpp
	RibbonWidgetPlugin.h
        RibbonGroupPlugin.cpp
	RibbonGroupPlugin.h
        RibbonWidgetsCollection.cpp
	RibbonWidgetsCollection.h
    )

    target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::UiPlugin)

    set(PLUGIN_OUTPUT_DIR ${PINGNOO_BINARY_DIR}/Designer/${CMAKE_LIBRARY_ARCHITECTURE}/${Qt5_VERSION})

    file(MAKE_DIRECTORY ${PLUGIN_OUTPUT_DIR})

    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}")

endif()

