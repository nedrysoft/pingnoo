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

#ifndef FIZZYADE_CORE_MENU_H
#define FIZZYADE_CORE_MENU_H

#include "IMenu.h"
#include "IContextManager.h"
#include <QObject>
#include <QString>
#include <QMap>
#include <QMenuBar>
#include <QMenu>

namespace FizzyAde::Core
{
    /**
     * @brief       Implementation of an IMenu
     *
     * @details     Represents a menu, submenu or menubar, it provides functions to insert or append commands to specific locations in the menu.
     *
     * @note        This class cannot be instantiated directly and can only be created by the FizzyAde::Core::CommandManager class.
     *
     * @see         FizzyAde::Core::CommandManager
     */
    class Menu :
        public FizzyAde::Core::IMenu
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IMenu)

    private:
        /**
         * @brief       Defines a object to store grouped menu items.

         * @details     Defines a object to store grouped menu items, contains the list of items that are members of the group.
         */
        class GroupItem {
        public:
            /**
             * @brief       Constructs a group item.
             *
             * @details     Constructs a group item with the identifier set to parameter @p id.
             *
             * @param[in]   id is the identifier of the group.
             */
            GroupItem(QString id)
            {
                m_id = id;
            }

        public:
            QString m_id;

            QList<QObject *> m_items;
        };

    private:
        /**
         * @brief       Constructs a menu.
         *
         * @details     Constructs a menu.
         */
        Menu();

        /**
         * @brief       Constructs a menu for the top level menu bar.
         *
         * @details     Constructs a menu for the top level menu bar.
         *
         * @param[in]   menuBar is the menu bar that is controlled by this instance.
         */
        Menu(QMenuBar *menuBar);

        /**
         * @brief       Constructs a menu for a main or sub menu.
         *
         * @details     Constructs a menu for a main or sub menu.
         *
         * @param[in]   menu is the menu that is controlled by this instance.
         */
        Menu(QMenu *menu);

    public:
        /**
         * @brief       Destroys the menu.
         *
         * @details     Destroys the menu.
         */
        ~Menu();

        /**
         * @copydoc     FizzyAde::Core::IMenu::type()
         */
        virtual FizzyAde::Core::MenuTypes type();

        /**
         * @copydoc     FizzyAde::Core::IMenu::menu()
         */
        virtual QMenu *menu();

        /**
         * @copydoc     FizzyAde::Core::IMenu::menuBar()
         */
        virtual QMenuBar *menuBar();

        /**
         * @copydoc     FizzyAde::Core::IMenu::insertGroup(QString)
         */
        virtual void insertGroup(QString groupIdentifier);

        /**
         * @copydoc     FizzyAde::Core::IMenu::appendGroup(QString)
         */
        virtual void appendGroup(QString groupIdentifier);

        /**
         * @copydoc     FizzyAde::Core::IMenu::addGroupBefore(QString, QString)
         */
        virtual bool addGroupBefore(QString beforeIdentifier, QString groupIdentifier);

        /**
         * @copydoc     FizzyAde::Core::IMenu::addGroupAfter(QString, QString)
         */
        virtual bool addGroupAfter(QString afterIdentifier, QString groupIdentifier);

        /**
         * @copydoc     FizzyAde::Core::IMenu::addGroupAfter(FizzyAde::Core::ICommand *, QString)
         */
        virtual void appendCommand(FizzyAde::Core::ICommand *command, QString groupIdentifier=QString());

        /**
         * @copydoc     FizzyAde::Core::IMenu::insertCommand(FizzyAde::Core::ICommand *, QString)
         */
        virtual void insertCommand(FizzyAde::Core::ICommand *command, QString groupIdentifier=QString());

    private:
        /**
         * @brief       Finds the group by the identifier passed in parameter @p groupIdentifier.
         *
         * @details     Finds the group by the identifier passed in parameter @p groupIdentifier, if the group exists then the iterator will point at
         *              the item; otherwise it will point at the end of the list.
         *
         * @param[in]   groupIdentifier is the identifier of the group to find.
         *
         * @return      the list iterator.
         */
        QList<GroupItem>::const_iterator findGroup(QString groupIdentifier);

        /**
         * @brief       Gets a pointer to the action for to be inserted before.
         *
         * @details     Given a iterator into the group list, this function will find the action which is used as the root for an append operation.
         *
         *              The group at the @p groupIterator is checked to see if it contains any items, if it does then the action of the first item in
         *              the group is returned.
         *
         *              If the group does not contain any items, then the function will iterate through all the remaining groups until a group is found
         *              that contains items and the action of the first item returned.
         *
         *              If no action could be found, then nullptr is returned.
         *
         * @param[in]   groupIdentifier is the identifier of the group to find.
         *
         * @return      a pointer to the QAction in the menu or nullptr if the no existing action could be found.
         */
        QAction *getInsertAction(QList<FizzyAde::Core::Menu::GroupItem>::const_iterator groupIterator);

        /**
         * @brief       Gets a pointer to the action to be appended after.
         *
         * @details     Given a iterator into the group list, this function will find the action which is used as the root for an append operation.
         *
         *              The group directly after the @p groupIterator position is checked to see if it contains any items, if it does then the action
         *              of the first item in the group is returned.
         *
         *              If the group does not contain any items, then the function will iterate through all the remaining groups until a group is found
         *              that contains items and the action of the first item returned.
         *
         *              If no action could be found, then nullptr is returned.
         *
         * @param[in]   groupIdentifier is the identifier of the group to find.
         *
         * @return      a pointer to the QAction in the menu or nullptr if the no existing action could be found.
         */
        QAction *getAppendAction(QList<FizzyAde::Core::Menu::GroupItem>::const_iterator groupIterator);

        friend class CommandManager;

    private:
        QMenuBar *m_menuBar;
        QMenu *m_menu;
        QList<GroupItem> m_groupList;
    };
}

#endif // FIZZYADE_CORE_MENU_H
