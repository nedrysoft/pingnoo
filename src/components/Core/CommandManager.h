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

#ifndef NEDRYSOFT_CORE_COMMANDMANAGER_H
#define NEDRYSOFT_CORE_COMMANDMANAGER_H

#include "ICommandManager.h"
#include "Menu.h"

#include <QMap>
#include <QObject>
#include <QString>

namespace Nedrysoft::Core {
    class Command;

    /**
     * @brief       The CommandManager class is responsible for creating Commands and updating them when the
     *              application context changes.
     *
     * @details     It provides methods for creating menus and locating commands.
     */
    class CommandManager :
            public Nedrysoft::Core::ICommandManager {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::ICommandManager)

        public:
            /**
             * @brief       Constructs a new CommandManager.
             */
            CommandManager();

            /**
             * @brief       Destroys the CommandManager.
             */
            ~CommandManager();

        public:
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
            auto registerAction(
                    QAction *action,
                    QString id,
                    const Nedrysoft::Core::ContextList &contexts ) -> Nedrysoft::Core::ICommand * override;

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
            auto registerAction(
                    QAction *action,
                    Nedrysoft::Core::ICommand *command,
                    const Nedrysoft::Core::ContextList &contexts ) -> bool override;

            /**
             * @brief       Sets the currently active context.
             *
             * @details     Updates all commands registered so that they connect to the correct QAction for the context.
             *
             * @see         Nedrysoft::Core::ICommandManager::setContext
             *
             * @param[in]   contextId the context to set as active.
             */
            auto setContext(int contextId) -> void override;

            /**
              * @brief       Create a menu.
              *
              * @details     Creates an IMenu object, the given identifier should be unique.
              *
              * @see         Nedrysoft::Core::ICommandManager::createMenu
              *
              * @param[in]   identifier the unique identifier for this menu
              * @param[in]   parentMenu  if the case of a submenu, parentMenu should be set to the parent IMenu instance.
              *
              * @returns     a new IMenu instance for the menu.
              */
            auto createMenu(
                    const QString &identifier,
                    IMenu *parentMenu = nullptr) -> Nedrysoft::Core::IMenu * override;

            /**
             * @brief       Find a menu.
             *
             * @details     Finds a menu by the given identifier, application defined constants are located in
             *              the Pingnoo::Constants namespace in Pingnoo.h
             *
             * @see         Nedrysoft::Core::ICommandManager::createMenu
             *
             * @param[in]   identifier the unique identifier for the menu.
             *
             * @returns     the IMenu instance if the menu exists; otherwise nullptr.
             */
            auto findMenu(const QString &identifier) -> Nedrysoft::Core::IMenu * override;

            /**
             * @brief       Find a command.
             *
             * @details     Finds a registered command by given identifier.
             *
             * @see         Nedrysoft::Core::ICommandManager::createMenu
             *
             * @param[in]   identifier the identifier for the command.
             *
             * @returns     The ICommand instance if the command exists; otherwise nullptr;
             */
            auto findCommand(const QString &identifier) -> Nedrysoft::Core::ICommand * override;

        private:

            QMap<QString, Command *> m_commandMap;                           //! Map containing commands registered in this manager
            QMap<QString, Nedrysoft::Core::Menu *> m_menuMap;                //! Map containing all menus registered in this manager
    };
}

#endif // NEDRYSOFT_CORE_CONTEXTMANAGER_H
