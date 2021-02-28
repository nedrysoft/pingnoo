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

#ifndef NEDRYSOFT_CORE_MENU_H
#define NEDRYSOFT_CORE_MENU_H

#include "IContextManager.h"
#include "IMenu.h"

#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QString>

namespace Nedrysoft::Core {
    /**
     * @brief       The Menu class provides a QMenu based implementation of IMenu.
     *
     * @details     Represents a menu or menubar, allows commands to be registered in the menu and allows items
     *              to be logically grouped.
     */
    class Menu :
            public Nedrysoft::Core::IMenu {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IMenu)

        private:
            /**
             * @brief       The GroupItem class defines an object that is used to store grouped menu items.
             */
            class GroupItem {
                public:
                    /**
                     * @brief       Constructs a group item.
                     *
                     * @details     Constructs a group item with the identifier.
                     *
                     * @param[in]   id is the identifier of the group.
                     */
                    GroupItem(QString id) {
                        m_id = id;
                    }

                public:
                    QString m_id;

                    QList<QObject *> m_items;
            };

        private:
            /**
             * @brief       Constructs a Menu instance.
             */
            Menu();

            /**
             * @brief       Constructs a menu with the top level menu bar.
             *
             * @details     Constructs a menu with the top level menu bar.
             *
             * @see         Nedrysoft::Core::Menu::Menu
             *
             * @param[in]   menuBar is the menu bar that is to be attached to this instance.
             */
            Menu(QMenuBar *menuBar);

            /**
             * @brief       Constructs a menu for a main or sub menu.
             *
             * @details     Constructs a menu for a main or sub menu.
             *
             * @see         Nedrysoft::Core::Menu::Menu
             *
             * @param[in]   menu is the menu that is controlled by this instance.
             */
            Menu(QMenu *menu);

        public:
            /**
             * @brief       Destroys the Menu.
             */
            ~Menu();

            /**
             * @brief       Returns the type of menu (Menu Bar and Menu)
             *
             * @see         Nedrysoft::Core::IMenu::type
             *
             * @returns     The Nedrysoft::Core::MenuTypes type of the menu.
             */
            auto type() -> Nedrysoft::Core::MenuTypes override;

            /**
             * @brief       Returns the QMenu if this is a menu.
             *
             * @see         Nedrysoft::Core::IMenu::menu
             *
             * @returns     the menu instance.
             */
            auto menu() -> QMenu * override;

            /**
             * @brief       Returns the QMenuBar if this is a menu bar.
             *
             * @see         Nedrysoft::Core::Menu::menuBar
             *
             * @returns     the menubar instance.
             */
            auto menuBar() -> QMenuBar * override;

            /**
             * @brief       Inserts a new group into the start of the menu.
             *
             * @details     Creates a named grouped section for a menu, allows commands to be grouped by function.
             *              The group will be inserted at the start of the menu.
             *
             * @see         Nedrysoft::Core::Menu::insertGroup
             *
             * @param[in]   groupIdentifier the identifier of the group.
             */
            auto insertGroup(QString groupIdentifier) -> void override;

            /**
             * @brief       Appends a new group to the end of the menu.
             *
             * @details     Creates a named grouped section for a menu, allows commands to be grouped by function.
             *              The group will be appended to the end of the menu.
             *
             * @see         Nedrysoft::Core::Menu::appendGroup
             *
             * @param[in]   groupIdentifier the identifier of the group.
             */
            auto appendGroup(QString groupIdentifier) -> void override;

            /**
             * @brief       Adds a new group before the given identifier.
             *
             * @details     Creates a named grouped section for a menu, allows commands to be grouped by function.
             *              The group will be inserted directly before the given existing identifier.
             *
             * @see         Nedrysoft::Core::Menu::addGroupBefore
             *
             * @param[in]   beforeIdentifier the identifier which this group is to be inserted before.
             * @param[in]   groupIdentifier the identifier of the group.
             *
             * @returns     true if added; otherwise false.
             */
            auto addGroupBefore(QString beforeIdentifier, QString groupIdentifier) -> bool override;

            /**
             * @brief       Adds a new group after the given identifier.
             *
             * @details     Creates a named grouped section for a menu, allows commands to be grouped by function.
             *              The group will be inserted directly after the given existing identifier.
             *
             * @see         Nedrysoft::Core::Menu::addGroupAfter
             *
             * @param[in]   beforeIdentifier the identifier which this group is to be inserted after.
             * @param[in]   groupIdentifier the identifier of the group.
             *
             * @returns     true if added; otherwise false.
             */
            auto addGroupAfter(QString afterIdentifier, QString groupIdentifier) -> bool override;

            /**
             * @brief       Appends a command to the end of a group.
             *
             * @details     The added command will be added to the end of the given group.
             *
             * @see         Nedrysoft::Core::Menu::appendCommand
             *
             * @param[in]   command the command to add.
             * @param[in]   groupIdentifier the identifier of the group.
             */
            auto appendCommand(Nedrysoft::Core::ICommand *command, QString groupIdentifier = QString()) -> void override;

            /**
             * @brief       Inserts a command to the start of a group.
             *
             * @details     The added command will be added to the start of the given group.
             *
             * @see         Nedrysoft::Core::Menu::insertCommand
             *
             * @param[in]   command the command to add.
             * @param[in]   groupIdentifier the identifier of the group.
             */
            auto insertCommand(Nedrysoft::Core::ICommand *command, QString groupIdentifier = QString()) -> void override;

        private:
            /**
             * @brief       Finds the group by the identifier.
             *
             * @details     Finds the group by the identifier, if the group exists then the iterator will point at
             *              the item; otherwise it will point at the end of the list.
             *
             * @param[in]   groupIdentifier is the identifier of the group to find.
             *
             * @returns     the list iterator.
             */
            auto findGroup(QString groupIdentifier) -> QList<GroupItem>::const_iterator;

            /**
             * @brief       Gets a pointer to the action for to be inserted before.
             *
             * @details     Given a iterator into the group list, this function will find the action which is used as the root for an append operation.
             *
             *              The group at the groupIterator is checked to see if it contains any items, if it does then the action of the first item in
             *              the group is returned.
             *
             *              If the group does not contain any items, then the function will iterate through all the remaining groups until a group is found
             *              that contains items and the action of the first item returned.
             *
             *              If no action could be found, then nullptr is returned.
             *
             * @param[in]   groupIterator is the iterator for the groups.
             *
             * @returns     a pointer to the QAction in the menu if found; otherwise nullptr.
             */
            auto getInsertAction(QList<Nedrysoft::Core::Menu::GroupItem>::const_iterator groupIterator) -> QAction *;

            /**
             * @brief       Gets a pointer to the action for to be appended after.
             *
             * @details     Given a iterator into the group list, this function will find the action which is used as the root for an append operation.
             *
             *              The group at the groupIterator is checked to see if it contains any items, if it does then the action of the first item in
             *              the group is returned.
             *
             *              If the group does not contain any items, then the function will iterate through all the remaining groups until a group is found
             *              that contains items and the action of the first item returned.
             *
             *              If no action could be found, then nullptr is returned.
             *
             * @param[in]   groupIterator is the iterator for the groups.
             *
             * @returns     a pointer to the QAction in the menu if found; otherwise nullptr.
             */
            auto getAppendAction(QList<Nedrysoft::Core::Menu::GroupItem>::const_iterator groupIterator) -> QAction *;

            friend class CommandManager;

        private:
            QMenuBar *m_menuBar;
            QMenu *m_menu;
            QList<GroupItem> m_groupList;
    };
}

#endif // NEDRYSOFT_CORE_MENU_H
