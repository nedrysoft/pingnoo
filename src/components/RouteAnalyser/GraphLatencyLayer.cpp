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

#pragma warning(push)
#pragma warning(disable : 4996)

#include "GraphLatencyLayer.h"

#pragma warning(pop)

#include "ColourManager.h"

#include <chrono>

using namespace std::chrono_literals;

constexpr auto DefaultIdealLatency = 100ms;
constexpr auto DefaultWarningLatency = 200ms;

constexpr auto roundedRectangleRadius = 10;
constexpr auto tinyNumber = 0.0001;                             //! used to adjust a unit number to just under 1

constexpr auto latencyStopLineColour = Qt::black;

Nedrysoft::RouteAnalyser::GraphLatencyLayer::GraphLatencyLayer(QCustomPlot *customPlot) :
        QCPItemRect(customPlot),
        m_idealLatency(DefaultIdealLatency),
        m_warningLatency(DefaultWarningLatency),
        m_useGradient(true) {

}

auto Nedrysoft::RouteAnalyser::GraphLatencyLayer::draw(QCPPainter *painter) -> void {
    auto graphMaxLatency = parentPlot()->yAxis->range().upper;
    auto rect = parentPlot()->axisRect()->rect();

    auto idealStop = m_idealLatency.count()/graphMaxLatency;
    auto warningStop = m_warningLatency.count()/graphMaxLatency;

    painter->save();

    // use a rounded rectangle as a clipping path, it looks better in dark mode

    QPainterPath clippingPath;

    clippingPath.addRoundedRect(rect, roundedRectangleRadius, roundedRectangleRadius);

    painter->setClipPath(clippingPath);

    QLinearGradient graphGradient = QLinearGradient(QPoint(rect.x(), rect.bottom()), QPoint(rect.x(), rect.top()));

    if (idealStop > 1) {
        graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour()));
        graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour()));
    } else {
        if (warningStop > 1) {
            if (idealStop < 1) {
                graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour()));
                graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getWarningColour()));

                if (!m_useGradient) {
                    graphGradient.setColorAt(idealStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getWarningColour()));
                    graphGradient.setColorAt(idealStop-tinyNumber, QColor(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour()));
                }
            }
        } else {
            graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour()));
            graphGradient.setColorAt(idealStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getWarningColour()));
            graphGradient.setColorAt(warningStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getCriticalColour()));
            graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getCriticalColour()));

            if (!m_useGradient) {
                graphGradient.setColorAt(idealStop-tinyNumber, QColor(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour()));
                graphGradient.setColorAt(warningStop-tinyNumber, QColor(Nedrysoft::RouteAnalyser::ColourManager::getWarningColour()));
            }
        }
    }

    painter->fillRect(rect, graphGradient);

    auto startPoint = QPointF();
    auto endPoint = QPointF();
    auto floatingPointRect = QRectF(rect);

    if (idealStop < 1) {
        startPoint = QPointF(
                floatingPointRect.left(),
                floatingPointRect.bottom()-(idealStop*floatingPointRect.height()) );

        endPoint = QPointF(
                floatingPointRect.right(),
                floatingPointRect.bottom()-(idealStop*floatingPointRect.height()) );
    }

    auto pen = QPen(Qt::DashLine);

    pen.setColor(latencyStopLineColour);

    painter->setPen(pen);

    painter->drawLine(startPoint, endPoint);

    if (warningStop < 1) {
        startPoint = QPointF(rect.left(), floatingPointRect.bottom()-(warningStop*floatingPointRect.height()));
        endPoint = QPointF(rect.right(), floatingPointRect.bottom()-(warningStop*floatingPointRect.height()));
    }

    painter->drawLine(startPoint, endPoint);

    painter->restore();
}

auto Nedrysoft::RouteAnalyser::GraphLatencyLayer::setGradientEnabled(bool useGradient) -> void {
    m_useGradient = useGradient;
}