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

#include "JitterPlotComponent.h"

#include "JitterPlotFactory.h"

#include <IComponentManager>

SystemTrayComponent::SystemTrayComponent() :
        m_plotFactory(nullptr) {

}

SystemTrayComponent::~SystemTrayComponent() {

}

auto SystemTrayComponent::finaliseEvent() -> void {
    if (m_plotFactory) {
        Nedrysoft::ComponentSystem::removeObject(m_plotFactory);

        delete m_plotFactory;
    }
}

auto SystemTrayComponent::initialiseEvent() -> void {
    m_plotFactory = new Nedrysoft::JitterPlot::JitterPlotFactory();

    Nedrysoft::ComponentSystem::addObject(m_plotFactory);
}
