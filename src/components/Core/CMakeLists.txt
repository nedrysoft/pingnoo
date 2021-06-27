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

pingnoo_add_sources(
    AboutDialog.cpp
    AboutDialog.h
    AboutDialog.ui
    ActionProxy.cpp
    ActionProxy.h
    ClipboardRibbonGroup.cpp
    ClipboardRibbonGroup.h
    ClipboardRibbonGroup.ui
    Command.cpp
    Command.h
    CommandManager.cpp
    CommandManager.h
    ContextManager.cpp
    ContextManager.h
    Core.cpp
    Core.h
    CoreComponent.cpp
    CoreComponent.h
    CoreSpec.h
    EditorManager.cpp
    EditorManager.h
    EditorManagerTabWidget.cpp
    EditorManagerTabWidget.h
    HostMaskerManager.cpp
    HostMaskerManager.h
    HostMaskerSettingsPage.cpp
    HostMaskerSettingsPage.h
    HostMaskerSettingsPageWidget.cpp
    HostMaskerSettingsPageWidget.h
    HostMaskerSettingsPageWidget.ui
    HostMaskingRibbonGroup.cpp
    HostMaskingRibbonGroup.h
    HostMaskingRibbonGroup.ui
    ICommand.h
    ICommandManager.h
    IConfiguration.h
    IContextManager.h
    ICore.h
    IEditor.h
    IEditorManager.h
    IGeoIPProvider.h
    IHostMasker.h
    IHostMaskerManager.h
    IHostMaskerSettingsPage.h
    ILogger.h
    IMenu.h
    IRibbonBarManager.h
    IRibbonGroup.h
    IRibbonPage.h
    IStatusbarManager.h
    ISystemTrayIcon.h
    ISystemTrayIconManager.h
    ISystemTrayIconWidget.h
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    Menu.cpp
    Menu.h
    RibbonBarManager.cpp
    RibbonBarManager.h
        RibbonAction.h
        RibbonActionProxy.cpp
        RibbonActionProxy.h
    RibbonGroup.cpp
    RibbonGroup.h
    RibbonPage.cpp
    RibbonPage.h
    StatusbarManager.cpp
    StatusbarManager.h
    SystemTrayIcon.cpp
    SystemTrayIcon.h
    SystemTrayIconManager.cpp
    SystemTrayIconManager.h
    ThemeSettingsPage.cpp
    ThemeSettingsPage.h
    fonts.qrc
    icons.qrc
)

pingnoo_set_description("Core application support component")

pingnoo_use_qt_libraries(Core Network Widgets)

pingnoo_use_shared_library(SettingsDialog)
pingnoo_use_shared_library(ComponentSystem)
pingnoo_use_shared_library(Ribbon)
pingnoo_use_shared_library(FontAwesome)
pingnoo_use_shared_library(ThemeSupport)

pingnoo_set_component_metadata("Core" "Provides core services to components.")

if(APPLE)
    pingnoo_use_shared_library(MacHelper)

    target_link_libraries(${PROJECT_NAME} "-framework AppKit" "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} stdc++ objc)
endif()

pingnoo_end_component()
