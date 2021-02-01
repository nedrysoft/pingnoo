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

#ifndef NEDRYSOFT_CORE_IMENU_H
#define NEDRYSOFT_CORE_IMENU_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QMenu>
#include <QMenuBar>

namespace Nedrysoft::Core {
    class ICommand;

    enum class MenuTypes {
        isMenuBar,
        isMenu,
        isSubMenu
    };

    /**
     * @brief       The IMenuInterface describes a menu or a menu bar.
     *
     * @details     Represents a menu or menubar, allows commands to be registered in the menu and allows items
     *              to be logically grouped.
     */
    class NEDRYSOFT_CORE_DLLSPEC IMenu :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       Returns the QMenu if this is a menu.
             *
             * @returns     the menu instance.
             */
            virtual auto menu() -> QMenu * = 0;

            /**
             * @brief       Returns the QMenuBar if this is a menu bar.
             *
             * @returns     the menubar instance.
             */
            virtual auto menuBar() -> QMenuBar * = 0;

            /**
             * @brief       Inserts a new group into the start of the menu.
             *
             * @details     Creates a named grouped section for a menu, allows commands to be grouped by function.
             *              The group will be inserted at the start of the menu.
             *
             * @param[in]   groupIdentifier the identifier of the group.
             */
            virtual auto insertGroup(QString groupIdentifier) -> void = 0;

            /**
             * @brief       Appends a new group to the end of the menu.
             *
             * @details     Creates a named grouped section for a menu, allows commands to be grouped by function.
             *              The group will be appended to the end of the menu.
             *
             * @param[in]   groupIdentifier the identifier of the group.
             */
            virtual auto appendGroup(QString groupIdentifier) -> void = 0;

            /**
             * @brief       Adds a new group before the given identifier.
             *
             * @details     Creates a named grouped section for a menu, allows commands to be grouped by function.
             *              The group will be inserted directly before the given existing identifier.
             *
             * @param[in]   beforeIdentifier the identifier which this group is to be inserted before.
             * @param[in]   groupIdentifier the identifier of the group.
             *
             * @returns     true if added; otherwise false.
             */
            virtual auto addGroupBefore(QString beforeIdentifier, QString groupIdentifier) -> bool = 0;

            /**
             * @brief       Adds a new group after the given identifier.
             *
             * @details     Creates a named grouped section for a menu, allows commands to be grouped by function.
             *              The group will be inserted directly after the given existing identifier.
             *
             * @param[in]   beforeIdentifier the identifier which this group is to be inserted after.
             * @param[in]   groupIdentifier the identifier of the group.
             *
             * @returns     true if added; otherwise false.
             */
            virtual auto addGroupAfter(QString afterIdentifier, QString groupIdentifier) -> bool = 0;

            /**
             * @brief       Appends a command to the end of a group.
             *
             * @details     The added command will be added to the end of the given group.
             *
             * @param[in]   command the command to add.
             * @param[in]   groupIdentifier the identifier of the group.
             */
            virtual auto appendCommand(
                    Nedrysoft::Core::ICommand *command,
                    QString groupIdentifier = QString() ) -> void = 0;

            /**
             * @brief       Inserts a command to the start of a group.
             *
             * @details     The added command will be added to the start of the given group.
             *
             * @param[in]   command the command to add.
             * @param[in]   groupIdentifier the identifier of the group.
             */
            virtual auto insertCommand(
                    Nedrysoft::Core::ICommand *command,
                    QString groupIdentifier = QString() ) -> void = 0;

        public:
            /**
             * @brief       Returns the type of menu (Menu Bar and Menu)
             *
             * @returns     The Nedrysoft::Core::MenuTypes type of the menu.
             */
            virtual auto type() -> Nedrysoft::Core::MenuTypes = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IMenu, "com.nedrysoft.core.IMenu/1.0.0")

#endif // NEDRYSOFT_CORE_IMENU_H
