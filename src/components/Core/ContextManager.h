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

#ifndef NEDRYSOFT_CORE_CONTEXTMANAGER_H
#define NEDRYSOFT_CORE_CONTEXTMANAGER_H

#include "IContextManager.h"

#include <QMap>
#include <QObject>
#include <QString>

namespace Nedrysoft::Core {
    /**
     * @brief       The ContextManager class is used to manage the current application context.
     *
     * @details     A context is a state that the application enters, for example an application may have multiple
     *              editor types and each editor will have its own context id.  When an editor becomes active, the
     *              context for that editor is set as active, and the visibility or active state of commands or menus
     *              will be updated.
     */
    class ContextManager :
            public Nedrysoft::Core::IContextManager {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IContextManager)

        public:
            /**
             * @brief       Constructs a new ContextManager instance.
             */
            ContextManager();

        public:
            /**
             * @brief       Registers a context with the application.
             *
             * @details     A context represents a state that the software enters, usually when an editor has
             *              focus.  The context manager maintains this providing a mechanism to update menus
             *              the reflect the current context state.
             *
             * @see         Nedrysoft::Core::IContextManager::registerContext
             *
             * @param[in]   contextIdentifier the id of the new context.
             *
             * @returns     the numeric identifier of this context.
             */
            auto registerContext(QString contextIdentifier) -> int override;

            /**
             * @brief       Sets the current context.
             *
             * @details     Sets the currently active context to the given identifier.
             *
             * @see         Nedrysoft::Core::IContextManager::setContext
             *
             * @param[in]   contextIdentifier the id of the context being activated.
             *
             * @returns     the status of the context switch.
             */
            auto setContext(int contextIdentifier) -> int override;

            /**
             * @brief       Gets the current context.
             *
             * @details     Gets the numeric identifier of the current context.
             *
             * @see         Nedrysoft::Core::IContextManager::context
             *
             * @returns     the context identifier.
             */
            auto context() -> int override;

            /**
             * @brief       Gets the context id by name.
             *
             * @details     Gets the numeric identifier of the given context.
             *
             * @param[in]   contextName the context name;
             *
             * @returns     the context identifier.
             */
            auto context(QString contextName) -> int override;

        private:
            int m_currentContextId;                             //! The currently selected context id
            int m_nextContextId;                                //! The next available context id available to register
            QMap<QString, int> m_contextIds;                    //! The list of registered context ids
    };
}

#endif // NEDRYSOFT_CORE_CONTEXTMANAGER_H
