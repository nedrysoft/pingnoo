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

#ifndef NEDRYSOFT_CORE_ICONTEXTMANAGER_H
#define NEDRYSOFT_CORE_ICONTEXTMANAGER_H

#include "ComponentSystem/IComponentManager.h"
#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QObject>

namespace Nedrysoft::Core {
    using ContextList = QList<int>;

    constexpr int GlobalContext = 0;

    /**
     * @brief       The ContextManager interface is used to manage the current application context.
     *
     * @details     A context is a state that the application enters, for example an application may have multiple
     *              editor types and each editor will have its own context id.  When an editor becomes active, the
     *              context for that editor is set as active, and the visibility or active state of commands or menus
     *              will be updated.
     */
    class NEDRYSOFT_CORE_DLLSPEC IContextManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the IContextManager instance.
             */
            static IContextManager *getInstance() {
                return ComponentSystem::getObject<IContextManager>();
            }

            /**
             * @brief       Registers a context with the application.
             *
             * @details     A context represents a state that the software enters, usually when an editor has
             *              focus.  The context manager maintains this providing a mechanism to update menus
             *              the reflect the current context state.
             *
             * @param[in]   contextIdentifier the id of the new context.
             *
             * @returns     the numeric identifier of this context.
             */
            virtual int registerContext(QString contextIdentifier) = 0;

            /**
             * @brief       Sets the current context.
             *
             * @details     Sets the currently active context to the given identifier.
             *
             * @param[in]   contextIdentifier the id of the context being activated.
             *
             * @returns     the status of the context switch.
             */
            virtual int setContext(int contextIdentifier) = 0;

            /**
             * @brief       Gets the current context.
             *
             * @details     Gets the numeric identifier of the current context.
             *
             * @returns     the context identifier.
             */
            virtual int context() = 0;

            /**
             * @brief       Signals that the context has changed.
             *
             * @details     Emitted when the context manager switches context.
             *
             * @param[in]   newContext the newly activated context.
             * @param[in]   previousContext the previous selected context.
             */
            Q_SIGNAL void contextChanged(int newContext, int previousContext);
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IContextManager, "com.nedrysoft.core.IContextManager/1.0.0")

#endif // NEDRYSOFT_CORE_ICONTEXTMANAGER_H
