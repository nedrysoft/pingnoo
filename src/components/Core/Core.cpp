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

#include "Core.h"

Nedrysoft::Core::Core::Core() :
        m_mainWindow(new Nedrysoft::Core::MainWindow) {

    m_mainWindow->show();
    m_mainWindow->raise();
    m_mainWindow->activateWindow();

    Nedrysoft::ComponentSystem::addObject(m_mainWindow);
}

Nedrysoft::Core::Core::~Core() {
    Nedrysoft::ComponentSystem::removeObject(m_mainWindow);

    delete m_mainWindow;
}

auto Nedrysoft::Core::Core::open() -> void {
    m_mainWindow->initialise();

    Q_EMIT coreOpened();
}

auto Nedrysoft::Core::Core::mainWindow() -> QMainWindow * {
    return m_mainWindow;
}
