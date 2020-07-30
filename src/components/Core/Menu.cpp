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

#include "Menu.h"
#include "IContextManager.h"
#include "ICommand.h"
#include <QDebug>
#include "Pingnoo.h"

FizzyAde::Core::Menu::Menu()
{
    m_menuBar = nullptr;
    m_menu = nullptr;

    m_groupList.append(new GroupItem(Pingnoo::Constants::defaultGroupTop));
    m_groupList.append(new GroupItem(Pingnoo::Constants::defaultGroupMiddle));
    m_groupList.append(new GroupItem(Pingnoo::Constants::defaultGroupBottom));
}

FizzyAde::Core::Menu::~Menu() = default;

FizzyAde::Core::Menu::Menu(QMenuBar *menuBar) :
    Menu()
{
    m_menuBar = menuBar;
}

FizzyAde::Core::Menu::Menu(QMenu *menu) :
    Menu()
{
    m_menu = menu;
}

FizzyAde::Core::MenuTypes FizzyAde::Core::Menu::type()
{
    if (m_menuBar) {
        return FizzyAde::Core::MenuTypes::isMenuBar;
    }

    if (m_menu && (!qobject_cast<QMenuBar *>(m_menu->parent()))) {
        return FizzyAde::Core::MenuTypes::isSubMenu;
    }

    return FizzyAde::Core::MenuTypes::isMenu;
}

QMenu *FizzyAde::Core::Menu::menu()
{
    return m_menu;
}

QMenuBar *FizzyAde::Core::Menu::menuBar()
{
    return m_menuBar;
}

void FizzyAde::Core::Menu::addCommand(FizzyAde::Core::ICommand *command, QString group)
{
    if (!m_menu || !command) {
        return;
    }

    qDebug() << "adding command " << command->action()->text() << " to group " << group;

    for(auto group : m_groupList) {
        qDebug() << "  " << group->m_id;
    }

    auto groupItem = findGroup(group);

    if (!groupItem) {
        return;
    }

    if (!groupItem->m_items.count()) {
        qDebug() << "  inserting at start " << m_menu->actions().count();
        groupItem->m_items.insert(0, command);

        m_menu->insertAction(nullptr, command->action());

        return;
    }

    auto castToCommand = qobject_cast<FizzyAde::Core::ICommand *>(groupItem->m_items.first());

    if (castToCommand) {
        qDebug() << "  adding before " << castToCommand->action()->text();
        m_menu->insertAction(castToCommand->action(), command->action());

        return;
    }

    qDebug() << "Unable to insert menu";
}

FizzyAde::Core::Menu::GroupItem *FizzyAde::Core::Menu::findGroup(QString groupIdentifier, int *groupPosition)
{
    if (groupPosition) {
        *groupPosition = 0;
    }

    for (auto item : m_groupList) {
        if (item->m_id==groupIdentifier) {
            return item;
        }

        if (groupPosition) {
            (*groupPosition)++;
        }
    }

    return nullptr;
}

bool FizzyAde::Core::Menu::addGroupAfter(QString afterIdentifier, QString groupIdentifier)
{
    int position;

    auto result = findGroup(afterIdentifier, &position);

    if (!result) {
        return false;
    }

    if (position==m_groupList.length()-1) {
        m_groupList.append(new GroupItem(groupIdentifier));

        return true;
    }

    m_groupList.insert(position+1, new GroupItem(groupIdentifier));

    return(true);
}

bool FizzyAde::Core::Menu::addGroupBefore(QString beforeIdentifier, QString groupIdentifier)
{
    int position;

    auto result = findGroup(beforeIdentifier, &position);

    if (!result) {
        return false;
    }

    if (position==0) {
        m_groupList.insert(0, new GroupItem(groupIdentifier));

        return true;
    }

    m_groupList.insert(position, new GroupItem(groupIdentifier));

    return(true);
}

void FizzyAde::Core::Menu::appendGroup(QString groupIdentifier)
{
    m_groupList.append(new GroupItem(groupIdentifier));
}

void FizzyAde::Core::Menu::insertGroup(QString groupIdentifier)
{
    m_groupList.insert(0, new GroupItem(groupIdentifier));
}

