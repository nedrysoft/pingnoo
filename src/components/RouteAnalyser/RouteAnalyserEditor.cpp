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

#include <QObject>

constexpr auto defaultWindowSize = 10.0*60.0;
constexpr auto viewportSize = 0.5;

Nedrysoft::RouteAnalyser::RouteAnalyserEditor::RouteAnalyserEditor() :
        m_editorWidget(nullptr),
        m_viewportStart(0),
        m_viewportEnd(1) {

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

        auto viewportWidget = ComponentSystem::getObject<ViewportRibbonGroup>();
        double viewportSize = defaultWindowSize;

        if (viewportWidget) {
            viewportSize = viewportWidget->viewportSize();
        }

        m_editorWidget->setViewportSize(viewportSize);
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
    static auto hasBeenInitialised = false;

    if (viewportWidget) {
        if (m_editorWidget->datasetSize()<m_editorWidget->viewportSize()) {
            viewportWidget->setViewport(0.0, 1.0);
            viewportWidget->setViewportEnabled(false);
        } else {
            viewportWidget->setViewport(1-viewportSize, 1.0);
            viewportWidget->setViewportEnabled(true);
        }

        // datasetChanged -         the dataset in the editor has changed, start is the unit time when capture began
        //                          and end if the endpoint of the latest data.

        connect(m_editorWidget, &RouteAnalyserWidget::datasetChanged, [=](double start, double end) {
            if (end-start<m_editorWidget->viewportSize()) {
                auto trimmerSize = ((end-start)/m_editorWidget->viewportSize())*viewportSize;

                viewportWidget->setViewport(qMin(1.0-viewportSize, trimmerSize), 1.0);
                viewportWidget->setViewportEnabled(false);
            } else {
                if (!viewportWidget->isViewportEnabled()) {
                    viewportWidget->setViewport((1-viewportSize), 1.0);
                    m_editorWidget->setViewportPosition(1);
                }

                viewportWidget->setViewportEnabled(true);
            }

            viewportWidget->setStartAndEnd(start, end);
        });

        // viewportChanged -        the trimmer widget start or end has changed, start is the starting value of the
        //                          new window (0-1) and end is the stop point (0-1)

        connect(viewportWidget, &ViewportRibbonGroup::viewportChanged, [=](double start, double end) {
            if (m_editorWidget) {
                double viewportSize = 1.0-(end-start);
                double position = start/viewportSize;

                m_editorWidget->setViewportPosition(position);
            }
        });

        // viewportWindowChanged -  the size of the current viewport has been changed, size is number of seconds that
        //                          is displayed in the viewport.

        connect(viewportWidget, &ViewportRibbonGroup::viewportWindowChanged, [=](double size) {
            if (m_editorWidget) {
                m_editorWidget->setViewportSize(size);

                if (m_editorWidget->datasetSize()<m_editorWidget->viewportSize()) {
                    auto trimmerSize = (m_editorWidget->datasetSize()/m_editorWidget->viewportSize())*viewportSize;

                    m_editorWidget->setViewportPosition(0);
                    viewportWidget->setViewport(qMin(1.0-viewportSize, trimmerSize), 1.0);
                    viewportWidget->setViewportEnabled(false);
                } else {
                    if (!viewportWidget->isViewportEnabled()) {
                        m_editorWidget->setViewportPosition(1.0);
                        viewportWidget->setViewport((1-viewportSize), 1.0);
                    }

                    viewportWidget->setViewportEnabled(true);
                }
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
