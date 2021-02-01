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

#ifndef NEDRYSOFT_CORE_COMMAND_H
#define NEDRYSOFT_CORE_COMMAND_H

#include "ICommand.h"
#include "IContextManager.h"

#include <QMap>
#include <QObject>
#include <QString>

namespace Nedrysoft::Core {
    class ActionProxy;

    /**
     * @brief       ICommand interface
     *
     * @details     ICommand represents an actionable command in the system, commands
     *              are bound to QActions for given contexts, this allows the target of
     *              the command to change depending on the current context that the application
     *              is in.
     */
    class Command :
            public Nedrysoft::Core::ICommand {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::ICommand)

        public:
            /**
             * @brief       Constructs a new Command with the given id.
             *
             * @param[in]   id the identifier for this command.
             */
            Command(QString id);

            /**
             * @brief       Destroys the Command.
             */
            ~Command() override;

        public:
            /**
             * @brief       Returns the proxy action.
             *
             * @see         Nedrysoft::Core::ICommand::action
             *
             * @returns     the proxy action
             */
            auto action() -> QAction* override;

            /**
             * @brief       Sets the active state of the command.
             *
             * @see         Nedrysoft::Core::ICommand::setActive
             *
             * @param[in]   state true if active; otherwise false.
             */
            auto setActive(bool state) -> void override;

            /**
             * @brief       Returns the active state of the command.
             *
             * @see         Nedrysoft::Core::ICommand::active
             *
             * @returns     true if enabled; otherwise false.
             */
             auto active() -> bool override;

        protected:
            /**
             * @brief       Registers an action to the given contexts.
             *
             * @note        The command manager becomes the owner of the action.
             *
             * @param[in]   action the action.
             * @param[in]   contexts the list of contexts this action is used in.
             */
            auto registerAction(QAction *action, const Nedrysoft::Core::ContextList &contexts) -> void;

            /**
             * @brief       Sets the current context for this command.
             *
             * @brief       If there is a QAction registered with the contextId then it becomes the active
             *              active, if there is no QAction for the context then the command is disabled.
             *
             * @param[in]   contextId the context id.
             */
            auto setContext(int contextId) -> void;

            friend class CommandManager;

        private:

            QMap<int, QAction *> m_actions;                        //! Map containing actions stored by context key

            Nedrysoft::Core::ActionProxy *m_action;                //! The proxy action used by the UI
            QString m_id;                                          //! Unique identifier for the command
    };
}

#endif // NEDRYSOFT_CORE_COMMAND_H
