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

    m_groupList.append(GroupItem(Pingnoo::Constants::defaultGroupTop));
    m_groupList.append(GroupItem(Pingnoo::Constants::defaultGroupMiddle));
    m_groupList.append(GroupItem(Pingnoo::Constants::defaultGroupBottom));
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

QAction *FizzyAde::Core::Menu::getInsertAction(QList<FizzyAde::Core::Menu::GroupItem>::const_iterator groupIterator)
{
    if (groupIterator->m_items.count()) {
        auto castToCommand = qobject_cast<FizzyAde::Core::ICommand *>(groupIterator->m_items.first());

        if (castToCommand) {
            return castToCommand->action();
        }
    }

    while(groupIterator!=m_groupList.constEnd()) {
        if (groupIterator->m_items.count()) {
            break;
        }

        groupIterator++;
    }

    if (groupIterator==m_groupList.constEnd()) {
        return nullptr;
    }

    if (groupIterator->m_items.count()) {
        auto castToCommand = qobject_cast<FizzyAde::Core::ICommand *>(groupIterator->m_items.first());

        if (castToCommand) {
            return castToCommand->action();
        }
    }

    return nullptr;
}

QAction *FizzyAde::Core::Menu::getAppendAction(QList<FizzyAde::Core::Menu::GroupItem>::const_iterator groupIterator)
{
    groupIterator++;

    while(groupIterator!=m_groupList.constEnd()) {
        if (groupIterator->m_items.count()) {
            break;
        }

        groupIterator++;
    }

    if (groupIterator==m_groupList.constEnd()) {
        return nullptr;
    }

    if (groupIterator->m_items.count()) {
        auto castToCommand = qobject_cast<FizzyAde::Core::ICommand *>(groupIterator->m_items.first());

        if (castToCommand) {
            return castToCommand->action();
        }
    }

    return nullptr;
}

void FizzyAde::Core::Menu::insertCommand(FizzyAde::Core::ICommand *command, QString group)
{
    if (!m_menu || !command) {
        return;
    }

    auto groupIterator = findGroup(group);

    if (groupIterator == m_groupList.constEnd()) {
        return;
    }

    auto nextAction = getInsertAction(groupIterator);

    m_menu->insertAction(nextAction, command->action());

    m_groupList[groupIterator-m_groupList.constBegin()].m_items.append(command);
}

void FizzyAde::Core::Menu::appendCommand(FizzyAde::Core::ICommand *command, QString groupIdentifier)
{
    if (!m_menu || !command) {
        return;
    }

    auto groupIterator = findGroup(groupIdentifier);

    if (groupIterator == m_groupList.constEnd()) {
        return;
    }

    auto previousAction = getAppendAction(groupIterator);

    m_menu->insertAction(previousAction, command->action());

    m_groupList[groupIterator-m_groupList.constBegin()].m_items.append(command);
}

QList<FizzyAde::Core::Menu::GroupItem>::const_iterator FizzyAde::Core::Menu::findGroup(QString groupIdentifier)
{
    QList<FizzyAde::Core::Menu::GroupItem>::const_iterator groupIterator = m_groupList.constBegin();

    while(groupIterator!=m_groupList.constEnd()) {
        if (groupIterator->m_id==groupIdentifier) {
            break;
        }

        groupIterator++;
    }

    return groupIterator;
}

bool FizzyAde::Core::Menu::addGroupAfter(QString afterIdentifier, QString groupIdentifier)
{
    auto groupIterator = findGroup(afterIdentifier);

    if (groupIterator==m_groupList.constEnd()) {
        m_groupList.append(GroupItem(groupIdentifier));

        return true;
    }

    m_groupList.insert((groupIterator-m_groupList.constBegin())+1, GroupItem(groupIdentifier));

    return true;
}

bool FizzyAde::Core::Menu::addGroupBefore(QString beforeIdentifier, QString groupIdentifier)
{
    auto groupIterator = findGroup(beforeIdentifier);

    if (groupIterator == m_groupList.constEnd()) {
        return false;
    }

    if (groupIterator==m_groupList.constBegin()) {
        m_groupList.insert(0, GroupItem(groupIdentifier));

        return true;
    }

    m_groupList.insert(groupIterator-m_groupList.constBegin(), GroupItem(groupIdentifier));

    return true;
}

void FizzyAde::Core::Menu::appendGroup(QString groupIdentifier)
{
    m_groupList.append(GroupItem(groupIdentifier));
}

void FizzyAde::Core::Menu::insertGroup(QString groupIdentifier)
{
    m_groupList.insert(0, GroupItem(groupIdentifier));
}

