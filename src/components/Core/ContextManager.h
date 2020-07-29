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

#ifndef FIZZYADE_CORE_CONTEXTMANAGER_H
#define FIZZYADE_CORE_CONTEXTMANAGER_H

#include "IContextManager.h"
#include <QObject>
#include <QString>
#include <QMap>

namespace FizzyAde::Core
{

    /**
     * ContextManager implementation
     *
     * Provides the implementation of an IContextManager
     *
     */
    class ContextManager :
        public FizzyAde::Core::IContextManager
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IContextManager)

    public:
        /**
         * Constructor
         */
        ContextManager();

        /**
         * @sa IContextManager
         */
        virtual int registerContext(QString contextIdentifier);
        virtual int setContext(int contextIdentifier);
        virtual int context();

    private:
        int m_nextContextId;                                //! The next available context id available to register
        int m_currentContextId;                             //! The currently selected context id
        QMap<QString, int> m_contextIds;                    //! The list of registered context ids
    };
}

#endif // FIZZYADE_CORE_CONTEXTMANAGER_H
