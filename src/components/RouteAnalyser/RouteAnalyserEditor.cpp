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

#include "RouteAnalyserEditor.h"

#include "LatencyRibbonGroup.h"
#include "PlotScrollArea.h"
#include "RouteAnalyser.h"
#include "RouteAnalyserWidget.h"
#include "TargetManager.h"
#include "ViewportRibbonGroup.h"

#include <IContextManager>
#include <QObject>

constexpr auto DefaultWindowSize = 10.0*60.0;
constexpr auto ViewportSize = 0.5;

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
            m_pingEngineFactory
        );

        auto viewportWidget = ComponentSystem::getObject<ViewportRibbonGroup>();
        double newViewportSize = DefaultWindowSize;

        if (viewportWidget) {
            newViewportSize = viewportWidget->viewportSize();
        }

        m_editorWidget->setViewportSize(newViewportSize);

        auto favouritesManager = Nedrysoft::RouteAnalyser::TargetManager::getInstance();

        favouritesManager->addRecent(m_pingTarget, m_pingTarget, m_pingTarget, m_ipVersion);
    }

    return m_editorWidget;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::setPingEngine(
        Nedrysoft::RouteAnalyser::IPingEngineFactory *pingEngineFactory ) -> void {

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
    // unused static auto hasBeenInitialised = false;

    // TODO: the viewport widget needs to be properly sized when switching editors, currently it reverts to the initial
    // size and when the next data result comes it then changes to the correct size and position.

    if (viewportWidget) {
        if (m_editorWidget->datasetSize()<m_editorWidget->viewportSize()) {
            viewportWidget->setViewport(0.0, 1.0);
            viewportWidget->setViewportEnabled(false);
        } else {
            viewportWidget->setViewport(1-ViewportSize, 1.0);
            viewportWidget->setViewportEnabled(true);
        }

        connect(
            m_editorWidget,
            &RouteAnalyserWidget::datasetChanged,
            this,
            &Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onDatasetChanged
        );

        connect(
            viewportWidget,
            &ViewportRibbonGroup::viewportChanged,
            this,
            &Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onViewportChanged
        );

        connect(
            viewportWidget,
            &ViewportRibbonGroup::viewportWindowChanged,
            this,
            &Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onViewportWindowChanged
        );
    }

    if (latencyWidget)  {
        connect(
            latencyWidget,
            &LatencyRibbonGroup::valueChanged,
            this,
            &Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onLatencyValueChanged
        );
    }
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::deactivated() -> void {
    auto latencyWidget = ComponentSystem::getObject<LatencyRibbonGroup>();
    auto viewportWidget = ComponentSystem::getObject<ViewportRibbonGroup>();

    if (latencyWidget) {
        disconnect(
            latencyWidget,
            &LatencyRibbonGroup::valueChanged,
            this,
            &Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onLatencyValueChanged
        );
    }

    if (viewportWidget) {
        disconnect(
            viewportWidget,
            &ViewportRibbonGroup::viewportChanged,
            this,
            &Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onViewportChanged
        );

        disconnect(
            viewportWidget,
            &ViewportRibbonGroup::viewportWindowChanged,
            this,
            &Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onViewportWindowChanged

        );
    }

    disconnect(
        m_editorWidget,
        &RouteAnalyserWidget::datasetChanged,
        this,
        &Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onDatasetChanged
    );
}

void Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onDatasetChanged(double start, double end) {
    auto viewportWidget = ComponentSystem::getObject<ViewportRibbonGroup>();

    if (end-start<m_editorWidget->viewportSize()) {
        auto trimmerSize = ((end-start)/m_editorWidget->viewportSize())*ViewportSize;

        viewportWidget->setViewport(qMin(1.0-ViewportSize, trimmerSize), 1.0);
        viewportWidget->setViewportEnabled(false);
    } else {
        if (!viewportWidget->isViewportEnabled()) {
            viewportWidget->setViewport((1-ViewportSize), 1.0);
            m_editorWidget->setViewportPosition(1);
        }

        viewportWidget->setViewportEnabled(true);
    }

    viewportWidget->setStartAndEnd(start, end);
}

void Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onViewportChanged(double start, double end) {
    if (m_editorWidget) {
        double viewportSize = 1.0 - ( end - start );
        double position = start / viewportSize;

        m_editorWidget->setViewportPosition(position);
    }
}

void Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onViewportWindowChanged(double size) {
    auto viewportWidget = ComponentSystem::getObject<ViewportRibbonGroup>();

    if (m_editorWidget) {
        m_editorWidget->setViewportSize(size);

        if (m_editorWidget->datasetSize()<m_editorWidget->viewportSize()) {
            auto trimmerSize = (m_editorWidget->datasetSize()/m_editorWidget->viewportSize())*ViewportSize;

            m_editorWidget->setViewportPosition(0);
            viewportWidget->setViewport(qMin(1.0-ViewportSize, trimmerSize), 1.0);
            viewportWidget->setViewportEnabled(false);
        } else {
            if (!viewportWidget->isViewportEnabled()) {
                m_editorWidget->setViewportPosition(1.0);
                viewportWidget->setViewport((1-ViewportSize), 1.0);
            }

            viewportWidget->setViewportEnabled(true);
        }
    }
}

void Nedrysoft::RouteAnalyser::RouteAnalyserEditor::onLatencyValueChanged(
        LatencyRibbonGroup::LatencyType type,
        double value) {

}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::contextId() -> int {
    return m_contextId;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserEditor::generateOutput(
        Nedrysoft::RouteAnalyser::OutputType type,
        Nedrysoft::RouteAnalyser::OutputTarget target ) -> void {

    if (type==Nedrysoft::RouteAnalyser::OutputType::TableAsText) {
    } else if (type==Nedrysoft::RouteAnalyser::OutputType::TableAsPDF) {
    } else if (type==Nedrysoft::RouteAnalyser::OutputType::TableAsImage) {
    } else if (type==Nedrysoft::RouteAnalyser::OutputType::TableAsCSV) {
    } else if (type==Nedrysoft::RouteAnalyser::OutputType::GraphsAsImage) {
    } else if (type==Nedrysoft::RouteAnalyser::OutputType::GraphsAsPDF) {
    } else if (type==Nedrysoft::RouteAnalyser::OutputType::TableAndGraphsAsImage) {
    } else if (type==Nedrysoft::RouteAnalyser::OutputType::TableAndGraphsAsPDF) {
/*        QPrinter printer(QPrinter::HighResolution);

        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName("/Users/adriancarpenter/Desktop/pingnoo.pdf");

        //printer.setPageMargins(12, 16, 12, 20, QPrinter::Millimeter);
        //printer.setFullPage(false);

        QPainter painter(&printer);

        double xscale = printer.pageRect().width() / double(m_editorWidget->width());
        double yscale = printer.pageRect().height() / double(m_editorWidget->height());
        double scale = qMin(xscale, yscale);

        //painter.translate(printer.paperRect().center());
        //painter.scale(scale, scale);
        //painter.translate(-m_editorWidget->width()/ 2, -m_editorWidget->height()/ 2);

        int yOffset = 0;

        painter.scale(4, 4);

        QTextDocument doc;

        for (auto plot : m_editorWidget->m_plotList) {
plot->setPlottingHint()
            plot->render(&painter);
            painter.translate(0, 400);
        }

        //m_editorWidget->m_scrollArea->render(&painter);
        */
    }

    qDebug() << "generateOutput";
}
