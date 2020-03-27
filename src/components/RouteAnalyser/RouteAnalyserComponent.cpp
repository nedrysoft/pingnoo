/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
 * An open source ping path analyser
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

#include "RouteAnalyserComponent.h"
#include "RouteAnalyserEditor.h"
#include "ComponentSystem/IComponentManager.h"
#include "Core/ICore.h"
#include "Core/IContextManager.h"
#include <QDebug>

RouteAnalyserComponent::RouteAnalyserComponent() = default;

RouteAnalyserComponent::~RouteAnalyserComponent() = default;

void RouteAnalyserComponent::initialiseEvent()
{
    auto core = FizzyAde::Core::ICore::getInstance();

    if (core) {
        connect(core, &FizzyAde::Core::ICore::coreOpened, [&] () {
            // do something when core is opened
        });
    }

    auto contextManager = FizzyAde::Core::IContextManager::getInstance();

    if (contextManager) {
        m_editorContextId = contextManager->registerContext("RouteAnalyserComponent::RouteAnalyserEditor");

        connect(contextManager, &FizzyAde::Core::IContextManager::contextChanged, [&] (int newContext, int previousContext) {
            Q_UNUSED(newContext)
            Q_UNUSED(previousContext)
        });
    }
}

void RouteAnalyserComponent::initialisationFinishedEvent()
{
    FizzyAde::ComponentSystem::addObject(new FizzyAde::RouteAnalyser::RouteAnalyserEditor(m_editorContextId));
}
