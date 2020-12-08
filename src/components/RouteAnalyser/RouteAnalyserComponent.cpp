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

#include "RouteAnalyserComponent.h"

#include "ComponentSystem/IComponentManager.h"
#include "Core/ICommandManager.h"
#include "Core/IContextManager.h"
#include "Core/ICore.h"
#include "Core/IEditor.h"
#include "Core/IEditorManager.h"
#include "NewTargetDialog.h"
#include "Pingnoo.h"
#include "RouteAnalyserEditor.h"

#include <QDebug>

RouteAnalyserComponent::RouteAnalyserComponent() = default;

RouteAnalyserComponent::~RouteAnalyserComponent() = default;

void RouteAnalyserComponent::initialiseEvent() {
    auto contextManager = Nedrysoft::Core::IContextManager::getInstance();

    if (contextManager) {
        m_editorContextId = contextManager->registerContext("RouteAnalyserComponent::RouteAnalyserEditor");

        connect(contextManager, &Nedrysoft::Core::IContextManager::contextChanged,
                [&](int newContext, int previousContext) {
                    Q_UNUSED(newContext)
                    Q_UNUSED(previousContext)
                });
    }

    auto core = Nedrysoft::Core::ICore::getInstance();

    if (core) {
        connect(core, &Nedrysoft::Core::ICore::coreOpened, [&]() {
            auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

            if (commandManager) {
                // create New Target... action

                auto action = new QAction(tr("New Target..."));

                connect(action, &QAction::triggered, [this]() {
                    Nedrysoft::RouteAnalyser::NewTargetDialog newTargetDialog;

                    if (newTargetDialog.exec()) {
                        auto editorManager = Nedrysoft::Core::IEditorManager::getInstance();

                        if (editorManager) {
                            Nedrysoft::RouteAnalyser::RouteAnalyserEditor *editor =
                                    new Nedrysoft::RouteAnalyser::RouteAnalyserEditor(m_editorContextId);

                            editor->setPingEngine(newTargetDialog.pingEngineFactory());
                            editor->setTarget(newTargetDialog.pingTarget());
                            editor->setIPVersion(newTargetDialog.ipVersion());
                            editor->setInterval(newTargetDialog.interval());

                            editorManager->openEditor(editor);
                        }
                    }
                });

                // register File/New Target... menu option global context

                auto command = commandManager->registerAction(action, "Menu.File.NewTarget");

                auto menu = commandManager->findMenu(Pingnoo::Constants::menuFile);

                menu->appendCommand(command, Pingnoo::Constants::fileNewGroup);

                // create Edit/Cut action for this context

                action = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::editCut));

                connect(action, &QAction::triggered, [&](bool) {
                    qDebug() << "action triggered (route analyser) !";
                });

                // register Edit/Cut menu option for this context

                commandManager->registerAction(action, Pingnoo::Constants::editCut, m_editorContextId);

                Nedrysoft::Core::IContextManager::getInstance()->setContext(m_editorContextId);

                action->setEnabled(true);
            }
        });
    }
}

void RouteAnalyserComponent::initialisationFinishedEvent() {

}
