/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 16/03/2021.
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

#include "JitterPlot.h"

#include "CPAxisTickerMS.h"
#include "Core/ICore.h"
#include "JitterBackgroundLayer.h"

#include <QLabel>
#include <cmath>

Nedrysoft::JitterPlot::JitterPlot::JitterPlot(const QMargins &margins) :
        m_customPlot(0),
        m_previousValue(-1),
        m_margins(margins) {

}

Nedrysoft::JitterPlot::JitterPlot::~JitterPlot() {

}

constexpr auto DefaultGraphHeight = 150;

auto Nedrysoft::JitterPlot::JitterPlot::widget() -> QWidget * {
    auto customPlot = new QCustomPlot();

    customPlot->addLayer("newBackground", customPlot->layer("grid"), QCustomPlot::limBelow);

    m_backgroundLayer = new JitterBackgroundLayer(customPlot);

    customPlot->setCurrentLayer("main");

    customPlot->setMinimumHeight(DefaultGraphHeight);

    customPlot->addGraph();

    customPlot->axisRect()->setAutoMargins(QCP::msNone);
    customPlot->axisRect()->setMargins(m_margins);

    QSharedPointer<CPAxisTickerMS> msTicker(new CPAxisTickerMS);

    customPlot->yAxis->setTicker(msTicker);
    customPlot->yAxis->setLabel(tr("Jitter (ms)"));
    customPlot->yAxis->setRange(0, 0.06);

    customPlot->yAxis->ticker()->setTickCount(1);

    constexpr auto DefaultTimeWindow = 60.0;

    auto viewportSize = DefaultTimeWindow;

    customPlot->xAxis->setVisible(false);

    customPlot->xAxis->setRange(
            static_cast<double>(QDateTime::currentDateTime().toSecsSinceEpoch()),
            static_cast<double>(QDateTime::currentDateTime().toSecsSinceEpoch() + viewportSize) );

    customPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph(0)->setPen(QPen(Qt::black, 1));

    QPalette palette = Nedrysoft::Core::mainWindow()->palette();

    customPlot->setBackground(palette.color(QPalette::Base));

    customPlot->xAxis->setLabelColor(palette.color(QPalette::Text));
    customPlot->yAxis->setLabelColor(palette.color(QPalette::Text));
    customPlot->xAxis->setTickLabelColor(palette.color(QPalette::Text));
    customPlot->yAxis->setTickLabelColor(palette.color(QPalette::Text));

    /**
     * scroll wheel events, by default QCustomPlot does not propagate these so this code ensures that they cause
     * the scroll area to scroll.
     */

    connect(customPlot, &QCustomPlot::mouseWheel, [=](QWheelEvent *event) {
        auto parentWidget = customPlot->parentWidget();

        while(parentWidget) {
            auto scrollArea = qobject_cast<QAbstractScrollArea *>(parentWidget);

            if (scrollArea) {
                scrollArea->verticalScrollBar()->setValue(
                        scrollArea->verticalScrollBar()->value() - event->angleDelta().y() );

                break;
            }

            parentWidget = parentWidget->parentWidget();
        }
    });

    m_customPlot = customPlot;

    return customPlot;
}

auto Nedrysoft::JitterPlot::JitterPlot::update(double time, double value) -> void {
    if (m_customPlot) {
        if (m_previousValue!=-1) {
            m_customPlot->graph(0)->addData(time, std::abs(value - m_previousValue));
        }

        m_previousValue = value;

        if (m_customPlot->isVisible()) {
            if (!m_customPlot->visibleRegion().isEmpty()) {
                m_customPlot->replot(QCustomPlot::rpQueuedReplot);
            }
        }
    }
}

auto Nedrysoft::JitterPlot::JitterPlot::updateRange(double min, double max) -> void {
    m_customPlot->xAxis->setRange(min, max);
    m_customPlot->replot(QCustomPlot::rpQueuedReplot);
}

auto Nedrysoft::JitterPlot::JitterPlot::setRange(double targetJitter, double maximumJitter) -> void {
    m_backgroundLayer->setRange(targetJitter, maximumJitter);
    m_customPlot->replot(QCustomPlot::rpQueuedReplot);
}