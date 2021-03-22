/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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
#include "ContextManager.h"
#include "Core.h"
#include "HostMaskerSettingsPage.h"
#include "IPingEngineFactory.h"
#include "IRibbonBarManager.h"
#include "IRibbonPage.h"
#include "IRouteEngine.h"
#include "PingResult.h"
#include "Pingnoo.h"

CoreComponent::CoreComponent() :
        m_core(nullptr),
        m_contextManager(nullptr),
        m_commandManager(nullptr) {

}
CoreComponent::~CoreComponent() {
}

auto CoreComponent::initialiseEvent() -> void {
    qRegisterMetaType<Nedrysoft::Core::PingResult>("Nedrysoft::Core::PingResult");
    qRegisterMetaType<Nedrysoft::Core::RouteList>("Nedrysoft::Core::RouteList");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<Nedrysoft::Core::IPingEngineFactory *>("Nedrysoft::Core::IPingEngineFactory *");
    qRegisterMetaType<Nedrysoft::Core::IPVersion>("Nedrysoft::Core::IPVersion");

    m_core = new Nedrysoft::Core::Core();
    m_contextManager = new Nedrysoft::Core::ContextManager();
    m_commandManager = new Nedrysoft::Core::CommandManager();
    m_hostMaskerSettingsPage = new Nedrysoft::Core::HostMaskerSettingsPage();

    Nedrysoft::ComponentSystem::addObject(m_core);
    Nedrysoft::ComponentSystem::addObject(m_contextManager);
    Nedrysoft::ComponentSystem::addObject(m_commandManager);
    Nedrysoft::ComponentSystem::addObject(m_hostMaskerSettingsPage);

    auto ribbonBarManager = Nedrysoft::Core::IRibbonBarManager::getInstance();

    if (ribbonBarManager) {
        /*auto page =*/ ribbonBarManager->addPage(tr("Host Masking"), Pingnoo::Constants::ribbonHostMaskingPage);

        //page->addGroup("My Group", "com.c", new QWidget());
    }

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

auto CoreComponent::finaliseEvent() -> void {
    if (m_hostMaskerSettingsPage) {
        delete m_hostMaskerSettingsPage;
    }

    if (m_core) {
        delete m_core;
    }

    if (m_contextManager) {
        delete m_contextManager;
    }

    if (m_commandManager) {
        delete m_commandManager;
    }
}