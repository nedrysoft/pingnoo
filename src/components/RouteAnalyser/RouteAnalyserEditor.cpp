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

#include "Core/IContextManager.h"
#include "RouteAnalyser.h"
#include "RouteAnalyserWidget.h"

#include <QLabel>
#include <QObject>

Nedrysoft::RouteAnalyser::RouteAnalyserEditor::RouteAnalyserEditor() :
        m_editorWidget(nullptr) {

    auto contextManger = Nedrysoft::Core::IContextManager::getInstance();

    if (contextManger) {
        m_contextId = contextManger->context(Pingnoo::Constants::routeAnalyserContext);
    }
}

Nedrysoft::RouteAnalyser::RouteAnalyserEditor::~RouteAnalyserEditor() {
    if (m_editorWidget) {
        m_editorWidget->deleteLater();
    }
}

QJsonObject Nedrysoft::RouteAnalyser::RouteAnalyserEditor::saveConfiguration() {
    return QJsonObject();
}

bool Nedrysoft::RouteAnalyser::RouteAnalyserEditor::loadConfiguration(QJsonObject configuration) {
    Q_UNUSED(configuration)

    return false;
}

QWidget *Nedrysoft::RouteAnalyser::RouteAnalyserEditor::widget() {
    m_editorWidget = new RouteAnalyserWidget(
            m_pingTarget,
            m_ipVersion,
            m_interval,
            m_pingEngineFactory );

    return m_editorWidget;
}

void Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setPingEngine(Nedrysoft::Core::IPingEngineFactory *pingEngineFactory) {
    m_pingEngineFactory = pingEngineFactory;
}

void Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setTarget(QString target) {
    m_pingTarget = target;
}

QString Nedrysoft::RouteAnalyser::RouteAnalyserEditor::displayName() {
    return m_pingTarget;
}

void Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setIPVersion(Nedrysoft::Core::IPVersion ipVersion) {
    m_ipVersion = ipVersion;
}

void Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setInterval(double interval) {
    m_interval = interval;
}
