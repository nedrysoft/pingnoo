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

#include "Command.h"
#include "ActionProxy.h"
#include "IContextManager.h"
#include <QDebug>

Nedrysoft::Core::Command::Command(QString id) {
    m_action = new ActionProxy();
    m_id = id;
}

Nedrysoft::Core::Command::~Command() {
    delete m_action;
}

QAction *Nedrysoft::Core::Command::action() {
    return m_action;
}

void Nedrysoft::Core::Command::registerAction(QAction *action, const Nedrysoft::Core::ContextList &contexts) {
    connect(action, &QAction::changed, [action, this] {
        m_action->setEnabled(action->isEnabled());
    });

    for (auto contextId : contexts) {
        m_actions[contextId] = action;
    }

    if (Nedrysoft::Core::IContextManager::getInstance()) {
        setContext(Nedrysoft::Core::IContextManager::getInstance()->context());
    }
}

void Nedrysoft::Core::Command::setContext(int contextId) {
    if (m_actions.contains(contextId)) {
        m_action->setActive(m_actions[contextId]);
    } else {
        if (m_actions.contains(Nedrysoft::Core::GlobalContext)) {
            m_action->setActive(m_actions[Nedrysoft::Core::GlobalContext]);
        } else {
            m_action->setActive(nullptr);
        }
    }
}

void Nedrysoft::Core::Command::setActive(bool state) {
    m_action->setEnabled(state);
}

bool Nedrysoft::Core::Command::active() {
    return m_action->isEnabled();
}
