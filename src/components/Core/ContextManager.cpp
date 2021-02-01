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

#include "ContextManager.h"

Nedrysoft::Core::ContextManager::ContextManager() :
        m_currentContextId(0),
        m_nextContextId(1) {

}

auto Nedrysoft::Core::ContextManager::registerContext(QString contextIdentifier) -> int {
    Q_UNUSED(contextIdentifier)

    m_contextIds[contextIdentifier] = m_nextContextId;

    return m_nextContextId++;
}

auto Nedrysoft::Core::ContextManager::setContext(int contextIdentifier) -> int {
    Q_EMIT contextChanged(contextIdentifier, m_currentContextId);

    m_currentContextId = contextIdentifier;

    return 0;
}

auto Nedrysoft::Core::ContextManager::context() -> int {
    return m_currentContextId;
}

auto Nedrysoft::Core::ContextManager::context(QString contextName) -> int {
    if (m_contextIds.contains(contextName)) {
        return m_contextIds[contextName];
    }

    return 0;
}
