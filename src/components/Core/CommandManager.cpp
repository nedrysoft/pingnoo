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

#include "CommandManager.h"
#include "ICore.h"
#include "Command.h"
#include "IContextManager.h"
#include "Menu.h"
#include "Pingnoo.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QAction>

FizzyAde::Core::CommandManager::CommandManager() = default;

FizzyAde::Core::ICommand *FizzyAde::Core::CommandManager::registerAction(QAction *action, QString id, const FizzyAde::Core::ContextList &contexts)
{
    if (m_commandMap.contains(id)) {
        auto command = m_commandMap[id];

        command->registerAction(action, contexts);
        command->setContext(FizzyAde::Core::IContextManager::getInstance()->context());

        command->setActive(action->isEnabled());

        return command;
    }

    auto command = new Command(id);

    command->registerAction(action, contexts);

    command->action()->setText(action->text());

    command->setContext(FizzyAde::Core::IContextManager::getInstance()->context());

    m_commandMap[id] = command;

    return command;
}

void FizzyAde::Core::CommandManager::setContext(int contextId)
{
    auto commandIterator = QMapIterator<QString, Command *>(m_commandMap);

    while(commandIterator.hasNext()) {
        commandIterator.next();

        commandIterator.value()->setContext(contextId);
    }
}

// also add createPopupMenu

FizzyAde::Core::IMenu *FizzyAde::Core::CommandManager::createMenu(const QString &identifier, IMenu *parentMenu)
{
    Q_UNUSED(identifier)
    FizzyAde::Core::Menu *newMenu = nullptr;

    if (m_menuMap.contains(identifier)) {
        return m_menuMap[identifier];
    }

    if (!parentMenu) {
        auto mainWindow = FizzyAde::Core::mainWindow();

        mainWindow->menuBar()->show();

        newMenu = new FizzyAde::Core::Menu(mainWindow->menuBar());
    } else {
        auto parent = qobject_cast<FizzyAde::Core::Menu *>(parentMenu);

        QMenuBar *parentMenuBar = nullptr;

        if (parent->m_menuBar) {
            parentMenuBar = parent->m_menuBar;
        }

        auto menu = new QMenu(Pingnoo::Constants::menuText(identifier), parentMenuBar);

        newMenu = new FizzyAde::Core::Menu(menu);

        if (parentMenuBar) {
            parentMenuBar->addAction(menu->menuAction());
        }
    }

    m_menuMap[identifier] = newMenu;

    return newMenu;
}

FizzyAde::Core::IMenu *FizzyAde::Core::CommandManager::findMenu(const QString &identifier)
{
    if (m_menuMap.contains(identifier)) {
        return m_menuMap[identifier];
    }

    return nullptr;
}

FizzyAde::Core::ICommand *FizzyAde::Core::CommandManager::findCommand(const QString &identifier)
{
    if (m_commandMap.contains(identifier)) {
        return m_commandMap[identifier];
    }

    return nullptr;
}

