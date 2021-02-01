/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 01/12/2020.
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

#ifndef NEDRYSOFT_CORE_ACTIONPROXY_H
#define NEDRYSOFT_CORE_ACTIONPROXY_H

#include <QAction>
#include <QPointer>

namespace Nedrysoft::Core {
    /**
     * @brief       The ActionProxy class is used to proxy actions depending on context.
     *
     * @details     A proxy class for a QAction, allows an Action to be proxied through, the active QAction can
     *              be switched depending on the current context of the application.
     */
    class ActionProxy :
            public QAction {

        public:

            /**
             * @brief       Constructs a new ActionProxy instance which is a child of the parent.
             *
             * @param[in]   parent the owner object.
             */
            ActionProxy(QObject *parent = nullptr);

            /**
             * @brief       Sets the currently active QAction.
             *
             * @param[in]   action the action to be proxied.
             */
            auto setActive(QAction *action) -> void;

        protected:
            /**
             * @brief       Connects the current action to the proxy.
             */
            auto connectAction() -> void;

            /**
             * @brief       Disconnects the current action from the proxy.
             */
            auto disconnectAction() -> void;

        private:
            QPointer<QAction> m_action;                         //! Pointer to active action
    };
}

#endif // NEDRYSOFT_CORE_ACTIONPROXY_H
