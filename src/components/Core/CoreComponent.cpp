/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/03/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "CoreComponent.h"

#include "CommandManager.h"
#include "ComponentSystem/IComponentManager.h"
#include "ContextManager.h"
#include "Core.h"
#include "IPingEngineFactory.h"
#include "IRouteEngine.h"
#include "PingResult.h"

auto CoreComponent::initialiseEvent() -> void {
    qRegisterMetaType<Nedrysoft::Core::PingResult>("Nedrysoft::Core::PingResult");
    qRegisterMetaType<Nedrysoft::Core::RouteList>("Nedrysoft::Core::RouteList");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<Nedrysoft::Core::IPingEngineFactory *>("Nedrysoft::Core::IPingEngineFactory *");

    Nedrysoft::ComponentSystem::addObject(new Nedrysoft::Core::Core());
    Nedrysoft::ComponentSystem::addObject(new Nedrysoft::Core::ContextManager());
    Nedrysoft::ComponentSystem::addObject(new Nedrysoft::Core::CommandManager());
}

auto CoreComponent::initialisationFinishedEvent() -> void {
    auto core = Nedrysoft::ComponentSystem::getObject<Nedrysoft::Core::Core>();

    connect(Nedrysoft::Core::IContextManager::getInstance(), &Nedrysoft::Core::IContextManager::contextChanged,
            [&](int newContext, int oldContext) {
                Q_UNUSED(oldContext)
                
                Nedrysoft::Core::ICommandManager::getInstance()->setContext(newContext);
            });

    core->open();
}
