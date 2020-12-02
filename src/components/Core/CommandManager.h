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
     * @brief       CommandManager implementation
     *
     * @details     Provides the implementation of an ICommandManager
     *
     */
    class CommandManager :
            public Nedrysoft::Core::ICommandManager {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::ICommandManager)

        public:
            /**
             * @brief       Constructor
             *
             */
            CommandManager();

            /**
             * @sa ICommandManager
             *
             */
            virtual Nedrysoft::Core::ICommand *
            registerAction(QAction *action, QString id, const Nedrysoft::Core::ContextList &contexts);

            virtual bool registerAction(QAction *action, Nedrysoft::Core::ICommand *command,
                                        const Nedrysoft::Core::ContextList &contexts);

            virtual void setContext(int contextId);

            virtual Nedrysoft::Core::IMenu *createMenu(const QString &identifier, IMenu *parentMenu = nullptr);

            virtual Nedrysoft::Core::IMenu *findMenu(const QString &identifier);

            virtual Nedrysoft::Core::ICommand *findCommand(const QString &identifier);

        private:

            QMap<QString, Command *> m_commandMap;                          //! Map containing commands registered in this manager
            QMap<QString, Nedrysoft::Core::Menu *> m_menuMap;                //! Map containing all menus registered in this manager
    };
}

#endif // NEDRYSOFT_CORE_CONTEXTMANAGER_H
