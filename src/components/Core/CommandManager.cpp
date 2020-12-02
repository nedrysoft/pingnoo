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

#include "Command.h"
#include "IContextManager.h"
#include "ICore.h"
#include "Menu.h"
#include "Pingnoo.h"

#include <QAction>
#include <QDebug>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

Nedrysoft::Core::CommandManager::CommandManager() = default;

Nedrysoft::Core::ICommand *Nedrysoft::Core::CommandManager::registerAction(QAction *action, QString id,
                                                                           const Nedrysoft::Core::ContextList &contexts) {
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

bool Nedrysoft::Core::CommandManager::registerAction(QAction *action, Nedrysoft::Core::ICommand *command,
                                                     const Nedrysoft::Core::ContextList &contexts) {
    Nedrysoft::Core::Command *commandClass = qobject_cast<Nedrysoft::Core::Command *>(command);

    if (commandClass) {
        commandClass->registerAction(action, contexts);
        commandClass->setContext(Nedrysoft::Core::IContextManager::getInstance()->context());

        commandClass->setActive(action->isEnabled());
    }

    return false;
}

void Nedrysoft::Core::CommandManager::setContext(int contextId) {
    auto commandIterator = QMapIterator<QString, Command *>(m_commandMap);

    while (commandIterator.hasNext()) {
        commandIterator.next();

        commandIterator.value()->setContext(contextId);
    }
}

// also add createPopupMenu

Nedrysoft::Core::IMenu *Nedrysoft::Core::CommandManager::createMenu(const QString &identifier, IMenu *parentMenu) {
    Q_UNUSED(identifier)
    Nedrysoft::Core::Menu *newMenu = nullptr;

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

Nedrysoft::Core::IMenu *Nedrysoft::Core::CommandManager::findMenu(const QString &identifier) {
    if (m_menuMap.contains(identifier)) {
        return m_menuMap[identifier];
    }

    return nullptr;
}

Nedrysoft::Core::ICommand *Nedrysoft::Core::CommandManager::findCommand(const QString &identifier) {
    if (m_commandMap.contains(identifier)) {
        return m_commandMap[identifier];
    }

    return nullptr;
}

