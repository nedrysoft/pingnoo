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

#pragma warning(push)
#pragma warning(disable : 4996)

#include "GraphLatencyLayer.h"
#include "LatencySettings.h"

#pragma warning(pop)

#include "ColourManager.h"

#include <cassert>
#include <chrono>

using namespace std::chrono_literals;

constexpr auto DefaultWarningLatency = 200ms;
constexpr auto DefaultCriticalLatency = 500ms;

constexpr auto roundedRectangleRadius = 10;
constexpr auto tinyNumber = 0.0001;                             //! used to adjust a unit number to just under 1

constexpr auto latencyStopLineColour = Qt::black;

constexpr auto unusedRemovalTime = 5;

QMap<QString, QPixmap> Nedrysoft::RouteAnalyser::GraphLatencyLayer::m_buffers;// = QMap<QString, QPixmap>();
QMap<QString, uint64_t> Nedrysoft::RouteAnalyser::GraphLatencyLayer::m_age;

Nedrysoft::RouteAnalyser::GraphLatencyLayer::GraphLatencyLayer(QCustomPlot *customPlot) :
        QCPItemRect(customPlot),
        m_warningLatency(DefaultWarningLatency),
        m_criticalLatency(DefaultCriticalLatency),
        m_useGradient(true) {

}

auto Nedrysoft::RouteAnalyser::GraphLatencyLayer::invalidate() -> void {
    m_buffers.clear();
    m_age.clear();
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

    auto idealStop = m_warningLatency.count()/graphMaxLatency;
    auto warningStop = m_criticalLatency.count()/graphMaxLatency;

    auto latencySettings = Nedrysoft::RouteAnalyser::LatencySettings::getInstance();

    assert(latencySettings!=nullptr);

    QString bufferName = QString("%1_%2_%3_%4_%5").arg(rect.size().width()).arg(rect.size().height()).arg(idealStop).arg(warningStop).arg(m_useGradient);

    if (!m_buffers.contains(bufferName)) {
        rect.translate(-rect.left(), -rect.top());

        QPixmap bufferedImage(rect.size());

        QPainter bufferPainter(&bufferedImage);

        QLinearGradient graphGradient = QLinearGradient(QPoint(rect.x(), rect.bottom()), QPoint(rect.x(), rect.top()));

        if (idealStop > 1) {
            graphGradient.setColorAt(0, QColor(latencySettings->idealColour()));
            graphGradient.setColorAt(1, QColor(latencySettings->idealColour()));
        } else {
            if (warningStop > 1) {
                if (idealStop < 1) {
                    graphGradient.setColorAt(0, QColor(latencySettings->idealColour()));
                    graphGradient.setColorAt(1, QColor(latencySettings->warningColour()));

                    if (!m_useGradient) {
                        graphGradient.setColorAt(idealStop, QColor(latencySettings->warningColour()));
                        graphGradient.setColorAt(idealStop-tinyNumber, QColor(latencySettings->idealColour()));
                    }
                }
            } else {
                graphGradient.setColorAt(0, QColor(latencySettings->idealColour()));
                graphGradient.setColorAt(idealStop, QColor(latencySettings->warningColour()));
                graphGradient.setColorAt(warningStop, QColor(latencySettings->criticalColour()));
                graphGradient.setColorAt(1, QColor(latencySettings->criticalColour()));

                if (!m_useGradient) {
                    graphGradient.setColorAt(idealStop-tinyNumber, QColor(latencySettings->idealColour()));
                    graphGradient.setColorAt(warningStop-tinyNumber, QColor(latencySettings->warningColour()));
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

    QPainterPath clippingPath;

    clippingPath.addRoundedRect(parentPlot()->axisRect()->rect(), roundedRectangleRadius, roundedRectangleRadius);

    painter->setClipPath(clippingPath);

    painter->drawPixmap(topLeft, m_buffers[bufferName]);
}

auto Nedrysoft::RouteAnalyser::GraphLatencyLayer::setGradientEnabled(bool useGradient) -> void {
    m_useGradient = useGradient;
    invalidate();
}