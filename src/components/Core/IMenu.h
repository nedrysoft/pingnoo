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

#ifndef NEDRYSOFT_CORE_IMENU_H
#define NEDRYSOFT_CORE_IMENU_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QMenu>
#include <QMenuBar>

namespace Nedrysoft::Core {
    class ICommand;

    enum MenuTypes {
        isMenuBar,
        isMenu,
        isSubMenu
    };

    /**
     * @brief       Interface definition of a menu
     *
     * @details     represents a menu or menubar, allows commands to be registered in
     *              the menu and allows items to be logically grouped.
     */
    class NEDRYSOFT_CORE_DLLSPEC IMenu :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       Return the QMenu if this is a menu
             *
             * @return      the menu instance
             */
            virtual QMenu *menu() = 0;

            /**
             * @brief       Return the QMenuBar if this is a menu bar
             *
             * @return      the menubar instance
             */
            virtual QMenuBar *menuBar() = 0;

            /**
             * @brief       Adds a new group
             *
             * @details     Creates a named grouped section for a menu, allows command to
             *              be grouped by function.  The group will be inserted at the start of
             *              the menu.
             *
             * @param[in]   groupIdentifier the identifier of the group
             */
            virtual void insertGroup(QString groupIdentifier) = 0;

            /**
             * @brief       Adds a new group
             *
             * @details     Creates a named grouped section for a menu, allows command to
             *              be grouped by function.  The group will be appended to the end of
             *              the menu.
             *
             * @param[in]   groupIdentifier the identifier of the group
             */
            virtual void appendGroup(QString groupIdentifier) = 0;

            /**
             * @brief       Adds a new group
             *
             * @details     Creates a named grouped section for a menu, allows command to
             *              be grouped by function.  The group will be inserted directly
             *              before the given existing identifier.
             *
             * @param[in]   groupIdentifier the identifier of the group
             *
             * @return      true if added; otherwisefalse
             */
            virtual bool addGroupBefore(QString beforeIdentifier, QString groupIdentifier) = 0;

            /**
             * @brief       Adds a new group
             *
             * @details     Creates a named grouped section for a menu, allows command to
             *              be grouped by function.  The group will be appended directly after
             *              the given existing identifier.
             *
             * @param[in]   groupIdentifier the identifier of the group
             *
             * @return      true if added; otherwise false.
             */
            virtual bool addGroupAfter(QString afterIdentifier, QString groupIdentifier) = 0;

            /**
             * @brief       Adds a command to a group
             *
             * @details     The added command will be added to the end of the given group
             *
             * @param[in]   command the command to add
             * @param[in]   groupIdentifier the identifier of the group
             */
            virtual void appendCommand(Nedrysoft::Core::ICommand *command, QString groupIdentifier = QString()) = 0;

            /**
             * @brief       Adds a command to a group
             *
             * @details     The added command will be inserted at the start of the given group
             *
             * @param[in]   command the command to add
             * @param[in]   groupIdentifier the identifier of the group
             */
            virtual void insertCommand(Nedrysoft::Core::ICommand *command, QString groupIdentifier = QString()) = 0;

        public:

            /**
             * @brief       Return the type of menu
             *
             * @return      a type defined in Nedrysoft::Core::MenuTypes
             */
            virtual Nedrysoft::Core::MenuTypes type() = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IMenu, "com.nedrysoft.core.IMenu/1.0.0")

#endif // NEDRYSOFT_CORE_IMENU_H
