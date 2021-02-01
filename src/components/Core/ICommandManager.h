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
     * @brief       The ICommandManager interface is responsible for creating Commands and updating them when the
     *              application context changes.
     *
     * @details     It provides methods for creating menus and locating commands.
     */
    class NEDRYSOFT_CORE_DLLSPEC ICommandManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Gets the Nedrysoft::Core::ICommandManager instance.
             *
             * @returns     the ICommandManager instance.
             */
            static auto getInstance() -> ICommandManager * {
                return ComponentSystem::getObject<ICommandManager>();
            }

            /**
             * @brief       Registers a QAction with a command for a given context.
             *
             * @details     This function registers an action by command id, if the command already exists
             *              then the action is added for the given context, otherwise a new command is created.
             *
             * @see         Nedrysoft::Core::ICommandManager::registerAction
             *
             * @param[in]   action the action.
             * @param[in]   id the identifier of the command.
             * @param[in]   contexts the contexts this action is valid in.
             *
             * @returns     a pointer to the ICommand
             */
            virtual auto registerAction(
                    QAction *action,
                    QString id,
                    const Nedrysoft::Core::ContextList &contexts ) -> Nedrysoft::Core::ICommand * = 0;

            /**
             * @brief       Registers an action to the given command.
             *
             * @details     This function registers an action by command id, if the command already exists
             *              then the action is added for the given context, otherwise a new command is created.
             *
             * @param[in]   action the action.
             * @param[in]   id the identifier of the command.
             * @param[in]   context the contexts this action is valid in.
             *
             * @returns     the Nedrysoft::Core::ICommand command.
             *
             */
            virtual auto registerAction(
                    QAction *action,
                    QString id,
                    int contextId = Nedrysoft::Core::GlobalContext ) -> Nedrysoft::Core::ICommand * {

                return registerAction(action, id, Nedrysoft::Core::ContextList() << contextId);
            }

            /**
             * @brief       Registers a QAction with a command for a given context.
             *
             * @details     This function registers an action to the given ICommand.
             *
             * @see         Nedrysoft::Core::ICommandManager::registerAction
             *
             * @param[in]   action the action.
             * @param[in]   command the identifier of the command.
             * @param[in]   context the contexts this action is valid in.
             *
             * @returns     true if the QAction was registered; otherwise false.
             */
            virtual auto registerAction(
                    QAction *action,
                    Nedrysoft::Core::ICommand *command,
                    int contextId = Nedrysoft::Core::GlobalContext ) -> bool {

                return registerAction(action, command, Nedrysoft::Core::ContextList() << contextId);
            }

            /**
             * @brief       Registers a QAction with a command for a given context.
             *
             * @details     This function registers an action to the given ICommand.
             *
             * @see         Nedrysoft::Core::ICommandManager::registerAction
             *
             * @param[in]   action the action.
             * @param[in]   command the identifier of the command.
             * @param[in]   context the contexts this action is valid in.
             *
             * @returns     true if the QAction was registered; otherwise false.
             */
            virtual auto registerAction(
                    QAction *action,
                    Nedrysoft::Core::ICommand *command,
                    const Nedrysoft::Core::ContextList &contexts ) -> bool = 0;

            /**
             * @brief       Sets the currently active context.
             *
             * @details     Updates all commands registered so that they connect to the correct QAction for the context.
             *
             * @param[in]   contextId the context to set as active.
             */
            virtual auto setContext(int contextId) -> void = 0;

            /**
              * @brief       Create a menu.
              *
              * @details     Creates an IMenu object, the given identifier should be unique.
              *
              * @param[in]   identifier the unique identifier for this menu
              * @param[in]   parentMenu  if the case of a submenu, parentMenu should be set to the parent IMenu instance.
              *
              * @returns     a new IMenu instance for the menu.
              */
            virtual auto createMenu(
                    const QString &identifier,
                    IMenu *parentMenu = nullptr ) -> Nedrysoft::Core::IMenu * = 0;

            /**
             * @brief       Find a menu.
             *
             * @details     Finds a menu by the given identifier, application defined constants are located in
             *              the Pingnoo::Constants namespace in Pingnoo.h
             *
             * @param[in]   identifier the unique identifier for the menu.
             *
             * @returns     the IMenu instance if the menu exists; otherwise nullptr.
             */
            virtual auto findMenu(const QString &identifier) -> Nedrysoft::Core::IMenu * = 0;

            /**
             * @brief       Find a command.
             *
             * @details     Finds a registered command by given identifier.
             *
             * @param[in]   identifier the identifier for the command.
             *
             * @returns     The ICommand instance if the command exists; otherwise nullptr;
             */
            virtual auto findCommand(const QString &identifier) -> Nedrysoft::Core::ICommand * = 0;

        public:

    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ICommandManager, "com.nedrysoft.core.ICommandManager/1.0.0")

#endif // NEDRYSOFT_CORE_ICOMMANDMANAGER_H
