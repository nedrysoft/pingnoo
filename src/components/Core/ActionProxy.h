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

#ifndef FIZZYADE_CORE_ACTIONPROXY_H
#define FIZZYADE_CORE_ACTIONPROXY_H

#include <QAction>
#include <QPointer>

namespace FizzyAde::Core
{
    /**
     * ActionProxy
     *
     * A proxy class for a QAction, allows an Action
     * to be proxied through, mutliple inputs can be
     * selected one at a time to be the output.
     *
     */

    class ActionProxy :
        public QAction
    {
    public:

        /**
         * Constructor
         *
         * @param[in]   parent      parent object
         *
         */
        ActionProxy(QObject *parent=nullptr);

        /**
         * Sets the currently active target action
         *
         * @param[in] action the action to be proxied
         *
         */
        void setActive(QAction *action);

    protected:
        /**
         * Connects the current action to the proxy
         *
         */
        void connectAction();

        /**
         * Disconnects the current action from the proxy
         *
         */
        void disconnectAction();

    private:
        QPointer<QAction> m_action;                         //! Pointer to active action
    };
}

#endif // FIZZYADE_CORE_ACTIONPROXY_H
