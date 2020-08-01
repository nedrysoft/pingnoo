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
    class Menu :
        public FizzyAde::Core::IMenu
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IMenu)

    private:
        class GroupItem {
        public:
            GroupItem(QString id)
            {
                m_id = id;
            }

        public:
            QString m_id;

            QList<QObject *> m_items;
        };

    public:
        Menu();
        ~Menu();

        Menu(QMenuBar *menuBar);
        Menu(QMenu *menu);

        /**
         * @sa IMenu
         */

        virtual FizzyAde::Core::MenuTypes type();

        virtual QMenu *menu();
        virtual QMenuBar *menuBar();

        virtual void insertGroup(QString groupIdentifier);
        virtual void appendGroup(QString groupIdentifier);
        virtual bool addGroupBefore(QString beforeIdentifier, QString groupIdentifier);
        virtual bool addGroupAfter(QString afterIdentifier, QString groupIdentifier);

        virtual void addCommand(FizzyAde::Core::ICommand *command, QString groupIdentifier=QString());

    private:
        QList<GroupItem>::const_iterator findGroup(QString groupIdentifier);
        QAction *getInsertAction(QList<FizzyAde::Core::Menu::GroupItem>::const_iterator groupIterator);
        QAction *getAppendAction(QList<FizzyAde::Core::Menu::GroupItem>::const_iterator groupIterator);

        friend class CommandManager;

    private:
        QMenuBar *m_menuBar;
        QMenu *m_menu;
        QList<GroupItem> m_groupList;
    };
}

#endif // FIZZYADE_CORE_MENU_H
