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

#include "ActionProxy.h"

Nedrysoft::Core::ActionProxy::ActionProxy(QObject *parent) :
        QAction(parent) {
    
    m_action = nullptr;
}

void Nedrysoft::Core::ActionProxy::setActive(QAction *action) {
    if (action == m_action) {
        return;
    }

    disconnectAction();

    m_action = action;

    if (action) {
        setMenuRole(action->menuRole());
    }

    connectAction();
}

void Nedrysoft::Core::ActionProxy::disconnectAction() {
    if (!m_action) {
        return;
    }

    disconnect(this, &QAction::triggered, m_action, &QAction::triggered);
    disconnect(this, &QAction::toggled, m_action, &QAction::setChecked);
}

void Nedrysoft::Core::ActionProxy::connectAction() {
    if (!m_action) {
        return;
    }

    connect(this, &QAction::triggered, m_action, &QAction::triggered);
    connect(this, &QAction::toggled, m_action, &QAction::setChecked);
}
