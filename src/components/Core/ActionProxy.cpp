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

#include "ActionProxy.h"

Nedrysoft::Core::ActionProxy::ActionProxy(QObject *parent) :
        QAction(parent),
        m_action(nullptr) {

}

auto Nedrysoft::Core::ActionProxy::setActive(QAction *action) -> void {
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

auto Nedrysoft::Core::ActionProxy::disconnectAction() -> void {
    if (!m_action) {
        return;
    }

    disconnect(this, &QAction::triggered, m_action, &QAction::triggered);
    disconnect(this, &QAction::toggled, m_action, &QAction::setChecked);
}

auto Nedrysoft::Core::ActionProxy::connectAction() -> void {
    if (!m_action) {
        return;
    }

    connect(this, &QAction::triggered, m_action, &QAction::triggered);
    connect(this, &QAction::toggled, m_action, &QAction::setChecked);
}
