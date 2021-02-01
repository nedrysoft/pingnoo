/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 22/01/2021.
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

#include "PingCommandPingComponent.h"

#include "ComponentSystem/IComponentManager.h"
#include "PingCommandPingEngineFactory.h"

PingCommandPingComponent::PingCommandPingComponent() :
        m_engineFactory(nullptr) {

}

PingCommandPingComponent::~PingCommandPingComponent() {

}

auto PingCommandPingComponent::finaliseEvent() -> void {
    if (m_engineFactory) {
        Nedrysoft::ComponentSystem::removeObject(m_engineFactory);

        delete m_engineFactory;
    }
}

auto PingCommandPingComponent::initialiseEvent() -> void {
    m_engineFactory = new Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory();

    Nedrysoft::ComponentSystem::addObject(m_engineFactory);
}
