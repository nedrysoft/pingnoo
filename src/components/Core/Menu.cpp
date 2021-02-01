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

#include "ICommand.h"
#include "Menu.h"
#include "Pingnoo.h"

Nedrysoft::Core::Menu::Menu() :
        m_menuBar(nullptr),
        m_menu(nullptr) {

    m_groupList.append(GroupItem(Pingnoo::Constants::defaultGroupTop));
    m_groupList.append(GroupItem(Pingnoo::Constants::defaultGroupMiddle));
    m_groupList.append(GroupItem(Pingnoo::Constants::defaultGroupBottom));
}

Nedrysoft::Core::Menu::~Menu() = default;

Nedrysoft::Core::Menu::Menu(QMenuBar *menuBar) :
        Menu() {
    m_menuBar = menuBar;
}

Nedrysoft::Core::Menu::Menu(QMenu *menu) :
        Menu() {
    m_menu = menu;
}

auto Nedrysoft::Core::Menu::type() -> Nedrysoft::Core::MenuTypes {
    if (m_menuBar) {
        return Nedrysoft::Core::MenuTypes::isMenuBar;
    }

    if (m_menu && ( !qobject_cast<QMenuBar *>(m_menu->parent()))) {
        return Nedrysoft::Core::MenuTypes::isSubMenu;
    }

    return Nedrysoft::Core::MenuTypes::isMenu;
}

auto Nedrysoft::Core::Menu::menu() -> QMenu * {
    return m_menu;
}

auto Nedrysoft::Core::Menu::menuBar() -> QMenuBar * {
    return m_menuBar;
}

auto Nedrysoft::Core::Menu::getInsertAction(
        QList<Nedrysoft::Core::Menu::GroupItem>::const_iterator groupIterator ) -> QAction * {

    if (groupIterator->m_items.count()) {
        auto castToCommand = qobject_cast<Nedrysoft::Core::ICommand *>(groupIterator->m_items.first());

        if (castToCommand) {
            return castToCommand->action();
        }
    }

    while (groupIterator != m_groupList.constEnd()) {
        if (groupIterator->m_items.count()) {
            break;
        }

        groupIterator++;
    }

    if (groupIterator == m_groupList.constEnd()) {
        return nullptr;
    }

    if (groupIterator->m_items.count()) {
        auto castToCommand = qobject_cast<Nedrysoft::Core::ICommand *>(groupIterator->m_items.first());

        if (castToCommand) {
            return castToCommand->action();
        }
    }

    return nullptr;
}

auto Nedrysoft::Core::Menu::getAppendAction(
        QList<Nedrysoft::Core::Menu::GroupItem>::const_iterator groupIterator ) -> QAction * {

    groupIterator++;

    while (groupIterator != m_groupList.constEnd()) {
        if (groupIterator->m_items.count()) {
            break;
        }

        groupIterator++;
    }

    if (groupIterator == m_groupList.constEnd()) {
        return nullptr;
    }

    if (groupIterator->m_items.count()) {
        auto castToCommand = qobject_cast<Nedrysoft::Core::ICommand *>(groupIterator->m_items.first());

        if (castToCommand) {
            return castToCommand->action();
        }
    }

    return nullptr;
}

auto Nedrysoft::Core::Menu::insertCommand(Nedrysoft::Core::ICommand *command, QString group) -> void {
    if (!m_menu || !command) {
        return;
    }

    auto groupIterator = findGroup(group);

    if (groupIterator == m_groupList.constEnd()) {
        return;
    }

    auto nextAction = getInsertAction(groupIterator);

    m_menu->insertAction(nextAction, command->action());

    m_groupList[groupIterator - m_groupList.constBegin()].m_items.append(command);
}

auto Nedrysoft::Core::Menu::appendCommand(Nedrysoft::Core::ICommand *command, QString groupIdentifier) -> void {
    if (!m_menu || !command) {
        return;
    }

    auto groupIterator = findGroup(groupIdentifier);

    if (groupIterator == m_groupList.constEnd()) {
        return;
    }

    auto previousAction = getAppendAction(groupIterator);

    m_menu->insertAction(previousAction, command->action());

    m_groupList[groupIterator - m_groupList.constBegin()].m_items.append(command);
}

auto Nedrysoft::Core::Menu::findGroup(QString groupIdentifier) -> QList<Nedrysoft::Core::Menu::GroupItem>::const_iterator {
    QList<Nedrysoft::Core::Menu::GroupItem>::const_iterator groupIterator = m_groupList.constBegin();

    while (groupIterator != m_groupList.constEnd()) {
        if (groupIterator->m_id == groupIdentifier) {
            break;
        }

        groupIterator++;
    }

    return groupIterator;
}

auto Nedrysoft::Core::Menu::addGroupAfter(QString afterIdentifier, QString groupIdentifier) -> bool {
    auto groupIterator = findGroup(afterIdentifier);

    if (groupIterator == m_groupList.constEnd()) {
        m_groupList.append(GroupItem(groupIdentifier));

        return true;
    }

    m_groupList.insert(( groupIterator - m_groupList.constBegin()) + 1, GroupItem(groupIdentifier));

    return true;
}

auto Nedrysoft::Core::Menu::addGroupBefore(QString beforeIdentifier, QString groupIdentifier) -> bool {
    auto groupIterator = findGroup(beforeIdentifier);

    if (groupIterator == m_groupList.constEnd()) {
        return false;
    }

    if (groupIterator == m_groupList.constBegin()) {
        m_groupList.insert(0, GroupItem(groupIdentifier));

        return true;
    }

    m_groupList.insert(groupIterator - m_groupList.constBegin(), GroupItem(groupIdentifier));

    return true;
}

auto  Nedrysoft::Core::Menu::appendGroup(QString groupIdentifier) -> void {
    m_groupList.append(GroupItem(groupIdentifier));
}

auto Nedrysoft::Core::Menu::insertGroup(QString groupIdentifier) -> void {
    m_groupList.insert(0, GroupItem(groupIdentifier));
}

