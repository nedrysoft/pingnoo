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

FizzyAde::Core::Menu::Menu()
{
    m_menuBar = nullptr;
    m_menu = nullptr;
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
        return(FizzyAde::Core::MenuTypes::isMenuBar);
    }

    if (m_menu && (!qobject_cast<QMenuBar *>(m_menu->parent()))) {
        return(FizzyAde::Core::MenuTypes::isSubMenu);
    }

    return(FizzyAde::Core::MenuTypes::isMenu);
}

QMenu *FizzyAde::Core::Menu::menu()
{
    return(m_menu);
}

QMenuBar *FizzyAde::Core::Menu::menuBar()
{
    return(m_menuBar);
}

void FizzyAde::Core::Menu::addCommand(FizzyAde::Core::ICommand *command)
{
    if (!m_menu || !command) {
        return;
    }

    m_menu->addAction(command->action());
}
