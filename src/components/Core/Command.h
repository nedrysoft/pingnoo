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

#ifndef FIZZYADE_CORE_COMMAND_H
#define FIZZYADE_CORE_COMMAND_H

#include "ICommand.h"
#include "IContextManager.h"
#include <QObject>
#include <QString>
#include <QMap>

namespace FizzyAde::Core
{
    class ActionProxy;

    /**
     * @brief       Command implementation
     *
     * @details     Provides the implementation of an ICommand for the
     *              CommandManager class
     *
     */
    class Command :
        public FizzyAde::Core::ICommand
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::ICommand)

    public:
        /**
         * @brief       Constructor
         *
         * @details     Constructs a new Command object with the given id
         *
         * @param[in]   id      id of the created command
         *
         */
        Command(QString id);

        /**
         * @brief       Destructor
         *
         */
        ~Command();

        /**
         * @sa ICommand
         *
         */
        virtual QAction *action();
        virtual void setActive(bool state);
        virtual bool active();

    protected:
        /**
         * @brief       Registers an action to the given contexts
         *
         * @param[in]   action      the action
         * @param[in]   contexts    the list of contexts this action is used in
         *
         */
        void registerAction(QAction *action, const FizzyAde::Core::ContextList &contexts);

        /**
         * @brief       Sets the current context for this command
         *
         * @param[in]   contextId   the context id
         *
         */
        void setContext(int contextId);

        friend class CommandManager;

    private:

        QMap<int, QAction *> m_actions;                     //! Map containing actions stored by context key

        FizzyAde::Core::ActionProxy *m_action;              //! The proxy action used by the UI
        QString m_id;                                       //! Unique identifier for the command
    };
}

#endif // FIZZYADE_CORE_COMMAND_H
