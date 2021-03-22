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

pingnoo_start_component()

pingnoo_set_component_optional(ON)

pingnoo_add_defines(QCUSTOMPLOT_USE_LIBRARY)

pingnoo_add_sources(
    BarChart.cpp
    BarChart.h
    CPAxisTickerMS.cpp
    CPAxisTickerMS.h
    ColourDialog.h
    ColourManager.cpp
    ColourManager.h
    TargetManager.cpp
    TargetManager.h
    FavouriteEditorDialog.cpp
    FavouriteEditorDialog.h
    FavouriteEditorDialog.ui
    FavouritesManagerDialog.cpp
    FavouritesManagerDialog.h
    FavouritesManagerDialog.ui
    FavouritesSortProxyFilterModel.cpp
    FavouritesSortProxyFilterModel.h
    GraphLatencyLayer.cpp
    GraphLatencyLayer.h
    LatencyRibbonGroup.cpp
    LatencyRibbonGroup.h
    LatencyRibbonGroup.ui
    LatencySettings.cpp
    LatencySettings.h
    LatencySettingsPage.cpp
    LatencySettingsPage.h
    LatencySettingsPageWidget.cpp
    LatencySettingsPageWidget.h
    LatencySettingsPageWidget.ui
    LatencyWidget.cpp
    LatencyWidget.h
    LineSyntaxHighlighter.cpp
    LineSyntaxHighlighter.h
    NewTargetDialog.cpp
    NewTargetDialog.h
    NewTargetDialog.ui
    NewTargetRibbonGroup.cpp
    NewTargetRibbonGroup.h
    NewTargetRibbonGroup.ui
    OpenFavouriteDialog.cpp
    OpenFavouriteDialog.h
    OpenFavouriteDialog.ui
    PingData.cpp
    PingData.h
    PlotScrollArea.cpp
    PlotScrollArea.h
    RouteAnalyserComponent.cpp
    RouteAnalyserComponent.h
    RouteAnalyserEditor.cpp
    RouteAnalyserEditor.h
    RouteAnalyserSpec.h
    RouteAnalyserWidget.cpp
    RouteAnalyserWidget.h
    RouteTableItemDelegate.cpp
    RouteTableItemDelegate.h
    SDK/IPlot.h
    SDK/IPlotFactory.h
    TargetSettings.cpp
    TargetSettings.h
    TargetSettingsPage.cpp
    TargetSettingsPage.h
    TargetSettingsPageWidget.cpp
    TargetSettingsPageWidget.h
    TargetSettingsPageWidget.ui
    ThemeSupport.h
    ThemeSupport.mm
    TrimmerWidget.cpp
    TrimmerWidget.h
    Utils.cpp
    Utils.h
    ViewportRibbonGroup.cpp
    ViewportRibbonGroup.h
    ViewportRibbonGroup.ui
    icons.qrc
)

if(APPLE)
    pingnoo_add_sources(ColourDialog.mm)
else()
    pingnoo_add_sources(ColourDialog.cpp)
endif()

pingnoo_set_description("Route analyser component")

pingnoo_use_qt_libraries(Core Widgets Network PrintSupport)

pingnoo_use_component(Core)

pingnoo_use_shared_library(ComponentSystem)
pingnoo_use_shared_library(QCustomPlot)
pingnoo_use_shared_library(AppNap)

if(APPLE)
    pingnoo_use_shared_library(MacHelper)
endif()

pingnoo_set_component_metadata("Views" "Provides the route visualiser system")

target_include_directories(${PROJECT_NAME} PRIVATE ${PINGNOO_LIBRARIES_SOURCE_DIR}/3rdparty/rapidfuzz-cpp)

pingnoo_end_component()
