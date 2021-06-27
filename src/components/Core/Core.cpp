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

#include <QApplication>
#include <QDir>
#include <QMenu>
#include <QStandardPaths>
#include "ThemeSupport"

Nedrysoft::Core::Core::Core() :
        m_mainWindow(new Nedrysoft::Core::MainWindow) {

    Nedrysoft::ComponentSystem::addObject(m_mainWindow);

    m_randomGenerator = new std::mt19937(m_randomDevice());
}

Nedrysoft::Core::Core::~Core() {
    Nedrysoft::ComponentSystem::removeObject(m_mainWindow);

    delete m_randomGenerator;

    delete m_mainWindow;

    delete Nedrysoft::ThemeSupport::ThemeSupport::getInstance();
}

auto Nedrysoft::Core::Core::open() -> void {
    //m_mainWindow->initialise();

    Q_EMIT coreOpened();
}

auto Nedrysoft::Core::Core::mainWindow() -> QMainWindow * {
    return m_mainWindow;
}

auto Nedrysoft::Core::Core::random(int minimumValue, int maximumValue) -> int {
    std::uniform_int_distribution<uint16_t> dist(minimumValue, maximumValue);

    return dist(*m_randomGenerator);
}

auto Nedrysoft::Core::Core::storageFolder() -> QString {
    auto applicationDir = QDir(qApp->applicationDirPath());

    if (QDir(applicationDir).exists("data")) {
        return QDir::cleanPath(applicationDir.absolutePath()+"/data");
    }

    return QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)
                   + "/" + qApp->organizationName() + "/" + qApp->applicationName();
}

auto Nedrysoft::Core::Core::applicationContextMenu() -> IMenu * {
    return m_mainWindow->applicationContextMenu();
}