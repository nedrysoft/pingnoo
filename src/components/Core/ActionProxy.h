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

#ifndef NEDRYSOFT_CORE_ACTIONPROXY_H
#define NEDRYSOFT_CORE_ACTIONPROXY_H

#include <QAction>
#include <QPointer>

namespace Nedrysoft::Core {
    /**
     * @brief       ActionProxy
     *
     * @details     A proxy class for a QAction, allows an Action
     *              to be proxied through, mutliple inputs can be
     *              selected one at a time to be the output.
     */
    class ActionProxy :
            public QAction {

        public:

            /**
             * @brief       Constructor
             *
             * @param[in]   parent the parent owner object
             */
            ActionProxy(QObject *parent = nullptr);

            /**
             * @brief       Sets the currently active target action
             *
             * @param[in]   action the action to be proxied
             */
            void setActive(QAction *action);

        protected:
            /**
             * @brief       Connects the current action to the proxy
             */
            void connectAction();

            /**
             * @brief       Disconnects the current action from the proxy
             */
            void disconnectAction();

        private:
            QPointer<QAction> m_action;                         //! Pointer to active action
    };
}

#endif // NEDRYSOFT_CORE_ACTIONPROXY_H
