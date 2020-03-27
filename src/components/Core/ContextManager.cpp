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

#include "ContextManager.h"

FizzyAde::Core::ContextManager::ContextManager()
{
    m_currentContextId = 0;
    m_nextContextId = 1;
}

int FizzyAde::Core::ContextManager::registerContext(QString contextIdentifier)
{
    Q_UNUSED(contextIdentifier)

    m_contextIds[contextIdentifier] = m_nextContextId;

    return(m_nextContextId++);
}

int FizzyAde::Core::ContextManager::setContext(int contextIdentifier)
{
    emit contextChanged(contextIdentifier, m_currentContextId);

    m_currentContextId = contextIdentifier;

    return(0);
}

int FizzyAde::Core::ContextManager::context()
{
    return(m_currentContextId);
}
