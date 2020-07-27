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
    auto visualiserWidget = new FizzyAde::RouteAnalyser::RouteAnalyserWidget(m_pingTarget, m_ipVersion, m_interval, m_pingEngineFactory);

    return(visualiserWidget);
}

void FizzyAde::RouteAnalyser::RouteAnalyserEditor::setPingEngine(FizzyAde::Core::IPingEngineFactory *pingEngineFactory)
{
    m_pingEngineFactory = pingEngineFactory;
}

void FizzyAde::RouteAnalyser::RouteAnalyserEditor::setTarget(QString target)
{
    m_pingTarget = target;
}

QString FizzyAde::RouteAnalyser::RouteAnalyserEditor::displayName()
{
    return m_pingTarget;
}

void FizzyAde::RouteAnalyser::RouteAnalyserEditor::setIPVersion(FizzyAde::Core::IPVersion ipVersion)
{
    m_ipVersion = ipVersion;
}

void FizzyAde::RouteAnalyser::RouteAnalyserEditor::setInterval(double interval)
{
    m_interval = interval;
}
