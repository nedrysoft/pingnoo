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
constexpr auto tinyNumber = 1.0/1e10;                             //! used to adjust a unit number to just under 1

constexpr auto latencyStopLineColour = Qt::black;

constexpr auto unusedRemovalTime = 5;

QMap<QString, QPixmap> Nedrysoft::RouteAnalyser::GraphLatencyLayer::m_buffers;// = QMap<QString, QPixmap>();
QMap<QString, uint64_t> Nedrysoft::RouteAnalyser::GraphLatencyLayer::m_age;

Nedrysoft::RouteAnalyser::GraphLatencyLayer::GraphLatencyLayer(QCustomPlot *customPlot) :
        QCPItemRect(customPlot),
        m_idealLatency(DefaultIdealLatency),
        m_warningLatency(DefaultWarningLatency),
        m_useGradient(true) {

}

auto Nedrysoft::RouteAnalyser::GraphLatencyLayer::removeUnused() -> void {
    QMutableMapIterator<QString, uint64_t> mapIterator(m_age);

    //TODO: this should probably be protected with a mutex.

    while(mapIterator.hasNext()) {
        mapIterator.next();

        if (QDateTime::currentSecsSinceEpoch()-mapIterator.value()>unusedRemovalTime) {
            QString key = mapIterator.key();

            m_buffers.remove(key);
            m_age.remove(key);
        }
    }
}

auto Nedrysoft::RouteAnalyser::GraphLatencyLayer::draw(QCPPainter *painter) -> void {
    auto graphMaxLatency = parentPlot()->yAxis->range().upper;
    auto rect = parentPlot()->axisRect()->rect();
    auto topLeft = rect.topLeft();

    auto idealStop = m_idealLatency.count()/graphMaxLatency;
    auto warningStop = m_warningLatency.count()/graphMaxLatency;

    QString bufferName = QString("%1_%2_%3_%4_%5").arg(rect.size().width()).arg(rect.size().height()).arg(idealStop).arg(warningStop).arg(m_useGradient);

    if (!m_buffers.contains(bufferName)) {
        rect.translate(-rect.left(), -rect.top());

        QPixmap bufferedImage(rect.size());

        QPainter bufferPainter(&bufferedImage);

        // use a rounded rectangle as a clipping path, it looks better in dark mode

        QPainterPath clippingPath;

        clippingPath.addRoundedRect(rect, roundedRectangleRadius, roundedRectangleRadius);

        bufferPainter.setClipPath(clippingPath);

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

        bufferPainter.fillRect(rect, graphGradient);

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

        bufferPainter.setPen(pen);

        bufferPainter.drawLine(startPoint, endPoint);

        if (warningStop < 1) {
            startPoint = QPointF(rect.left(), floatingPointRect.bottom()-(warningStop*floatingPointRect.height()));
            endPoint = QPointF(rect.right(), floatingPointRect.bottom()-(warningStop*floatingPointRect.height()));
        }

        bufferPainter.drawLine(startPoint, endPoint);

        bufferPainter.end();

        m_buffers[bufferName] = bufferedImage;
    }

    m_age[bufferName] = QDateTime::currentSecsSinceEpoch();

    painter->drawPixmap(topLeft, m_buffers[bufferName]);
}

auto Nedrysoft::RouteAnalyser::GraphLatencyLayer::setGradientEnabled(bool useGradient) -> void {
    m_useGradient = useGradient;
}