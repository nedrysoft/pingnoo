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

#include "RegExHostMaskerComponent.h"

#include "ComponentSystem/IComponentManager.h"
#include "RegExHostMasker.h"
#include "RegExHostMaskerSettingsPage.h"

#include <QJsonDocument>

RegExHostMaskerComponent::RegExHostMaskerComponent() :
        m_hostMasker(nullptr),
        m_settingsPage(nullptr) {

    qRegisterMetaType<Nedrysoft::RegExHostMasker::RegExHostMaskerItem>();
}

RegExHostMaskerComponent::~RegExHostMaskerComponent() {

}

auto RegExHostMaskerComponent::initialiseEvent() -> void {
    m_hostMasker = new Nedrysoft::RegExHostMasker::RegExHostMasker();
    m_settingsPage = new Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPage();

    Nedrysoft::ComponentSystem::addObject(m_hostMasker);
    Nedrysoft::ComponentSystem::addObject(m_settingsPage);
}

auto RegExHostMaskerComponent::initialisationFinishedEvent() -> void {
}

auto RegExHostMaskerComponent::finaliseEvent() -> void {
    if (m_settingsPage) {
        Nedrysoft::ComponentSystem::removeObject(m_settingsPage);

        delete m_settingsPage;
    }

    if (m_hostMasker) {
        Nedrysoft::ComponentSystem::removeObject(m_hostMasker);

        delete m_hostMasker;
    }
}