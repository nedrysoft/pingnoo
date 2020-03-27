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

pingnoo_start_component()

pingnoo_add_sources(
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    RibbonIcons.qrc
    CoreComponent.cpp
    CoreComponent.h
    CoreSpec.h
    ICore.h
    IConfiguration.h
    IEditor.h
    IGeoIPProvider.h
    IHostMasker.h
    IPingEngine.h
    IPingEngineFactory.h
    IPingTarget.h
    IRouteEngine.h
    IRibbonBarManager.h
    IStatusBarManager.h
    ICommand.h
    ICommandManager.h
    IMenu.h
    IContextManager.h
    PingResult.cpp
    PingResult.h
    ContextManager.cpp
    ContextManager.h
    Core.cpp
    Core.h
    CommandManager.cpp
    CommandManager.h
    Command.cpp
    Command.h
    ActionProxy.cpp
    ActionProxy.h
    Menu.cpp
    Menu.h
    StatusBarManager.cpp
    StatusBarManager.h
)

pingnoo_use_qt_libraries(Network Widgets)

pingnoo_use_shared_library(ComponentSystem)
pingnoo_use_shared_library(Ribbon)

pingnoo_set_component_metadata("1.0.0" "Core" "Provides core services to components.")

pingnoo_end_component()
