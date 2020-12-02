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

#ifndef NEDRYSOFT_CORE_ICOMMANDMANAGER_H
#define NEDRYSOFT_CORE_ICOMMANDMANAGER_H

#include "ComponentSystem/IComponentManager.h"
#include "ComponentSystem/IInterface.h"
#include "Core/IContextManager.h"
#include "Core/IMenu.h"
#include "CoreSpec.h"
#include "ICommand.h"

#include <QAction>
#include <QList>
#include <QObject>

namespace Nedrysoft::Core {
    /**
     * @brief       ICommandManager interface
     *
     * @details     ICommandManager manages instances of ICommand, handling context
     *              changes.
     *
     */
    class NEDRYSOFT_CORE_DLLSPEC ICommandManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Gets the Nedrysoft::Core::ICommandManager instance.
             *
             * @return      the ICommandManager instance
             *
             */
            static ICommandManager *getInstance() {
                return ComponentSystem::getObject<ICommandManager>();
            }

            /**
             * @brief       Registers an action to the command
             *
             * @param[in]   action      the action
             * @param[in]   id          the identifier of the command
             * @param[in]   contexts    the contexts this action is valid in
             *
             * @return      the configuration
             *
             */
            virtual Nedrysoft::Core::ICommand *
            registerAction(QAction *action, QString id, const Nedrysoft::Core::ContextList &contexts) = 0;

            /**
             * @brief       Registers an action to the command
             *
             * @details     This function registers an action by command id, if the command already exists
             *              then the action is added for the given context, otherwise a new command is created
             *
             * @param[in]   action      the action
             * @param[in]   id          the identifier of the command
             * @param[in]   context     the contexts this action is valid in
             *
             * @return      the command
             *
             */
            virtual Nedrysoft::Core::ICommand *
            registerAction(QAction *action, QString id, int contextId = Nedrysoft::Core::GlobalContext) {
                return registerAction(action, id, Nedrysoft::Core::ContextList() << contextId);
            }

            /**
             * @brief       Registers an action to an existing command
             *
             * @details     This function registers an action to the given command to a single context
             *
             * @param[in]   action      the action
             * @param[in]   command     the command to register the action with
             * @param[in]   context     the contexts this action is valid in
             *
             * @return      true if added, otherwise false
             *
             */
            virtual bool registerAction(QAction *action, Nedrysoft::Core::ICommand *command,
                                        int contextId = Nedrysoft::Core::GlobalContext) {
                return registerAction(action, command, Nedrysoft::Core::ContextList() << contextId);
            }

            /**
             * @brief       Registers an action to the command
             *
             * @details     This function registers an action to the given command for the
             *              supplied contexts
             *
             * @param[in]   action      the action
             * @param[in]   command     the command to register the action with
             * @param[in]   contexts    the contexts this action is valid in
             *
             * @return      the configuration
             *
             */
            virtual bool registerAction(QAction *action, Nedrysoft::Core::ICommand *command,
                                        const Nedrysoft::Core::ContextList &contexts) = 0;

            /**
             * @brief       Sets the currently active context
             *
             * @details     Updates all commands registered so that they connect to the
             *              correct QAction for the context
             *
             * @param[in]   contextId the context
             *
             */
            virtual void setContext(int contextId) = 0;

            /**
             * @brief       Create a menu
             *
             * @details     Creates an menu object that implements IMenu, the given identifier
             *              should be unique.
             *
             * @param[in]   identifier      the unique identifier for this menu
             * @param[in]   parentMenu      if the case of a submenu, parentMenu should be set to the parent IMenu instance
             *
             */
            virtual Nedrysoft::Core::IMenu *createMenu(const QString &identifier, IMenu *parentMenu = nullptr) = 0;

            /**
             * @brief       Find a menu
             *
             * @details     Finds a menu by it's unique identifer, application defined constants are located in
             *              the Pingnoo::Constants namespace in Pingnoo.h
             *
             * @param[in]   identifier      the unique identifier for this menu
             *
             * @return      the IMenu instance of the requested menu
             *
             */
            virtual Nedrysoft::Core::IMenu *findMenu(const QString &identifier) = 0;

            /**
             * @brief       Find a command
             *
             * @details     Finds a registered command by identifier
             *
             * @param[in]   identifier      the unique identifier for this command
             *
             * @return      the ICommand instance
             *
             */
            virtual Nedrysoft::Core::ICommand *findCommand(const QString &identifier) = 0;

        public:

    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ICommandManager, "com.nedrysoft.core.ICommandManager/1.0.0")

#endif // NEDRYSOFT_CORE_ICOMMANDMANAGER_H
