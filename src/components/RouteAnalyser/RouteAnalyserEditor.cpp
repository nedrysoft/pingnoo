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

#include "RouteAnalyserEditor.h"
#include "RouteAnalyserWidget.h"
#include "Core/ICommandManager.h"
#include "RouteAnalyserComponent.h"
#include "Pingnoo.h"
#include <QObject>
#include <QLabel>

FizzyAde::RouteAnalyser::RouteAnalyserEditor::RouteAnalyserEditor(int contextId)
{
    m_contextId = contextId;
}

QJsonObject FizzyAde::RouteAnalyser::RouteAnalyserEditor::saveConfiguration()
{
    return(QJsonObject());
}

bool FizzyAde::RouteAnalyser::RouteAnalyserEditor::loadConfiguration(QJsonObject configuration)
{
    Q_UNUSED(configuration)

    return(false);
}

QWidget *FizzyAde::RouteAnalyser::RouteAnalyserEditor::widget()
{
    static auto visualiserWidget = new FizzyAde::RouteAnalyser::RouteAnalyserWidget();

    static auto commandManager = FizzyAde::Core::ICommandManager::getInstance();

    if (commandManager) {
        QAction *action = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::editCut));

        connect(action, &QAction::triggered, [&] (bool) {
            qDebug() << "action triggered (route analyser) !";
        });

        commandManager->registerAction(action, Pingnoo::Constants::editCut, m_contextId);

        FizzyAde::Core::IContextManager::getInstance()->setContext(m_contextId);

        action->setEnabled(false);
        action->setEnabled(true);
    }

    return(visualiserWidget);
}
