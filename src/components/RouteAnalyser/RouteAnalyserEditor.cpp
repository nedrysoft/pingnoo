/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
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

#include "RouteAnalyserEditor.h"

#include "LatencyRibbonGroup.h"
#include "RouteAnalyser.h"
#include "RouteAnalyserWidget.h"
#include "ViewportRibbonGroup.h"

#include "QCustomPlot/qcustomplot.h"
#include <QObject>

using namespace std::chrono_literals;

constexpr std::chrono::duration<double> defaultWindowSize = 10min;

Nedrysoft::RouteAnalyser::RouteAnalyserEditor::RouteAnalyserEditor() :
        m_editorWidget(nullptr),
        m_viewportStart(0),
        m_viewportEnd(1),
        m_viewportWindow(std::chrono::duration_cast<std::chrono::milliseconds>(defaultWindowSize).count()) {

    auto contextManager = Nedrysoft::Core::IContextManager::getInstance();

    if (contextManager) {
        m_contextId = contextManager->context(Pingnoo::Constants::routeAnalyserContext);
    }

    Nedrysoft::ComponentSystem::addObject(this);
}

Nedrysoft::RouteAnalyser::RouteAnalyserEditor::~RouteAnalyserEditor() {
    Nedrysoft::ComponentSystem::removeObject(this);
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::widget() -> QWidget * {
    if (!m_editorWidget) {
        m_editorWidget = new RouteAnalyserWidget(
                m_pingTarget,
                m_ipVersion,
                m_interval,
                m_pingEngineFactory );

        m_editorWidget->setViewportWindow(m_viewportWindow);

        connect(m_editorWidget, &RouteAnalyserWidget::plotChanged,
                [=](QCustomPlot *customPlot, std::chrono::duration<double> time, std::chrono::duration<double> roundTrip) {


        });
    }

    return m_editorWidget;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setPingEngine(
        Nedrysoft::Core::IPingEngineFactory *pingEngineFactory ) -> void {

    m_pingEngineFactory = pingEngineFactory;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setTarget(QString target) -> void {
    m_pingTarget = target;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::displayName() -> QString {
    return m_pingTarget;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setIPVersion(Nedrysoft::Core::IPVersion ipVersion) -> void {
    m_ipVersion = ipVersion;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setInterval(double interval) -> void {
    m_interval = interval;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::activated() -> void {
    auto viewportWidget = ComponentSystem::getObject<ViewportRibbonGroup>();
    auto latencyWidget = ComponentSystem::getObject<LatencyRibbonGroup>();

    if (viewportWidget) {
        viewportWidget->setViewport(0.1, 0.9);

        connect(viewportWidget, &ViewportRibbonGroup::viewportChanged, [=](double start, double end) {

        });

        connect(viewportWidget, &ViewportRibbonGroup::viewportWindowChanged, [=](double size) {
            if (m_editorWidget) {
                m_editorWidget->setViewportWindow(size);
            }
        });
    }

    if (latencyWidget)  {
        connect(latencyWidget, &LatencyRibbonGroup::valueChanged, [=](LatencyRibbonGroup::LatencyType type, double value) {

        });
    }
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::deactivated() -> void {
    auto latencyWidget = ComponentSystem::getObject<LatencyRibbonGroup>();
    auto viewportWidget = ComponentSystem::getObject<ViewportRibbonGroup>();

    if (latencyWidget) {
        disconnect(latencyWidget);
    }

    if (viewportWidget) {
        disconnect(viewportWidget);
    }
}
