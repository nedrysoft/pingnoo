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

#include "CommandManager.h"

#include "Command.h"
#include "ICore.h"
#include "Pingnoo.h"

#include <QMenu>
#include <QMenuBar>

Nedrysoft::Core::CommandManager::CommandManager() = default;

Nedrysoft::Core::CommandManager::~CommandManager() {
    qDeleteAll(m_menuMap);
    qDeleteAll(m_commandMap);
}

auto Nedrysoft::Core::CommandManager::registerAction(
        QAction *action,
        QString id,
        const Nedrysoft::Core::ContextList &contexts) -> Nedrysoft::Core::ICommand * {

    if (m_commandMap.contains(id)) {
        auto command = m_commandMap[id];

        command->registerAction(action, contexts);
        command->setContext(Nedrysoft::Core::IContextManager::getInstance()->context());

        command->setActive(action->isEnabled());

        return command;
    }

    auto command = new Command(id);

    command->registerAction(action, contexts);

    command->action()->setText(action->text());

    command->setContext(Nedrysoft::Core::IContextManager::getInstance()->context());

    m_commandMap[id] = command;

    return command;
}

auto Nedrysoft::Core::CommandManager::registerAction(
        QAction *action,
        Nedrysoft::Core::ICommand *command,
        const Nedrysoft::Core::ContextList &contexts) -> bool {

    Nedrysoft::Core::Command *commandClass = qobject_cast<Nedrysoft::Core::Command *>(command);

    if (commandClass) {
        commandClass->registerAction(action, contexts);
        commandClass->setContext(Nedrysoft::Core::IContextManager::getInstance()->context());

        commandClass->setActive(action->isEnabled());
    }

    return false;
}

auto Nedrysoft::Core::CommandManager::setContext(int contextId) -> void {
    auto commandIterator = QMapIterator<QString, Command *>(m_commandMap);

    while (commandIterator.hasNext()) {
        commandIterator.next();

        commandIterator.value()->setContext(contextId);
    }
}

// TODO: also add createPopupMenu

auto Nedrysoft::Core::CommandManager::createMenu(
        const QString &identifier,
        IMenu *parentMenu ) -> Nedrysoft::Core::IMenu * {

    Q_UNUSED(identifier)

    Nedrysoft::Core::Menu *newMenu;

    if (m_menuMap.contains(identifier)) {
        return m_menuMap[identifier];
    }

    if (!parentMenu) {
        auto mainWindow = Nedrysoft::Core::mainWindow();

        mainWindow->menuBar()->show();

        newMenu = new Nedrysoft::Core::Menu(mainWindow->menuBar());
    } else {
        auto parent = qobject_cast<Nedrysoft::Core::Menu *>(parentMenu);

        QMenuBar *parentMenuBar = nullptr;

        if (parent->m_menuBar) {
            parentMenuBar = parent->m_menuBar;
        }

        auto menu = new QMenu(Pingnoo::Constants::menuText(identifier), parentMenuBar);

        newMenu = new Nedrysoft::Core::Menu(menu);

        if (parentMenuBar) {
            parentMenuBar->addAction(menu->menuAction());
        }
    }

    m_menuMap[identifier] = newMenu;

    return newMenu;
}

auto Nedrysoft::Core::CommandManager::findMenu(const QString &identifier) -> Nedrysoft::Core::IMenu * {
    if (m_menuMap.contains(identifier)) {
        return m_menuMap[identifier];
    }

    return nullptr;
}

auto Nedrysoft::Core::CommandManager::findCommand(const QString &identifier) -> Nedrysoft::Core::ICommand * {
    if (m_commandMap.contains(identifier)) {
        return m_commandMap[identifier];
    }

    return nullptr;
}

