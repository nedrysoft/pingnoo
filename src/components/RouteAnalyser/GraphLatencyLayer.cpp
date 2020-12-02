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

#include "GraphLatencyLayer.h"

#include "ColourManager.h"

#include <chrono>

using namespace std::chrono_literals;

constexpr auto DefaultLowRangeLatency = 100ms;
constexpr auto DefaultMidRangeLatency = 200ms;

constexpr auto roundedRectangleRadius = 10;
constexpr auto tinyNumber = 0.0001;                             //! used to adjust a unit number to just under 1

Nedrysoft::RouteAnalyser::GraphLatencyLayer::GraphLatencyLayer(QCustomPlot *customPlot) :
        QCPItemRect(customPlot),
        m_lowRangeLatency(DefaultLowRangeLatency),
        m_midRangeLatency(DefaultMidRangeLatency) {

}

void Nedrysoft::RouteAnalyser::GraphLatencyLayer::draw(QCPPainter *painter) {
    auto graphMaxLatency = parentPlot()->yAxis->range().upper;
    auto rect = parentPlot()->axisRect()->rect();
    auto smoothGradient = true;

    auto lowStop = m_lowRangeLatency.count() / graphMaxLatency;
    auto midStop = m_midRangeLatency.count() / graphMaxLatency;

    painter->save();

    // use a rounded rectangle as a clipping path, it looks better in dark mode

    QPainterPath clippingPath;

    clippingPath.addRoundedRect(rect, roundedRectangleRadius, roundedRectangleRadius);

    painter->setClipPath(clippingPath);

    QLinearGradient graphGradient = QLinearGradient(QPoint(rect.x(), rect.bottom()), QPoint(rect.x(), rect.top()));

    if (lowStop > 1) {
        graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()));
        graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()));
    } else {
        if (midStop > 1) {
            if (lowStop < 1) {
                graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()));
                graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMidColour()));
            }
        } else {
            graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()));
            graphGradient.setColorAt(lowStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMidColour()));
            graphGradient.setColorAt(midStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMaxColour()));
            graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMaxColour()));
        }
    }

    if (!smoothGradient) {
        graphGradient.setColorAt(lowStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMidColour()));
        graphGradient.setColorAt(lowStop - tinyNumber, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()));
        graphGradient.setColorAt(midStop - tinyNumber, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMaxColour()));
    }

    painter->fillRect(rect, graphGradient);

    auto startPoint = QPointF();
    auto endPoint = QPointF();
    auto floatingPointRect = QRectF(rect);

    if (lowStop < 1) {
        startPoint = QPointF(floatingPointRect.left(),
                             floatingPointRect.bottom() - ( lowStop * floatingPointRect.height()));
        endPoint = QPointF(floatingPointRect.right(),
                           floatingPointRect.bottom() - ( lowStop * floatingPointRect.height()));
    }

    auto pen = QPen(Qt::DashLine);

    pen.setColor(Qt::lightGray);

    painter->setPen(pen);

    painter->drawLine(startPoint, endPoint);

    if (midStop < 1) {
        startPoint = QPointF(rect.left(), floatingPointRect.bottom() - ( midStop * floatingPointRect.height()));
        endPoint = QPointF(rect.right(), floatingPointRect.bottom() - ( midStop * floatingPointRect.height()));
    }

    painter->drawLine(startPoint, endPoint);

    painter->restore();
}
