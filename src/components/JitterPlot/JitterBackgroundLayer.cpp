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

#include "JitterBackgroundLayer.h"

#pragma warning(pop)

constexpr auto RoundedRectangleRadius = 10;
constexpr auto JitterTargetLineWidth = 1;
constexpr auto JitterBackgroundColor = qRgb(0x77, 0xdc, 0xef);
constexpr auto GraphLineColour = qRgb(0x00, 0x00, 0x00);
constexpr auto TargetValue = 0.03;
constexpr auto MaximumValue = 0.06;

Nedrysoft::JitterPlot::JitterBackgroundLayer::JitterBackgroundLayer(QCustomPlot *customPlot) :
        QCPItemRect(customPlot),
        m_targetJitter(0.03),
        m_maximumJitter(0.06) {

}

auto Nedrysoft::JitterPlot::JitterBackgroundLayer::draw(QCPPainter *painter) -> void {
    auto rect = parentPlot()->axisRect()->rect();
    auto topLeft = rect.topLeft();

    if (m_bufferedImage.size()!=rect.size()) {
        m_bufferedImage = QPixmap(rect.size());

        rect.translate(-rect.left(), -rect.top());

        QPainter bufferPainter(&m_bufferedImage);

        bufferPainter.fillRect(rect, JitterBackgroundColor);

        bufferPainter.setPen(QPen(QColor(GraphLineColour), JitterTargetLineWidth, Qt::DashLine));

        auto targetY = static_cast<int>( static_cast<double>(parentPlot()->axisRect()->rect().height()) *
                         ( m_targetJitter / m_maximumJitter ));

        bufferPainter.drawLine(QPoint(rect.left(), targetY), QPoint(rect.right(), targetY));
    }

    painter->save();

    QPainterPath clippingPath;

    clippingPath.addRoundedRect(parentPlot()->axisRect()->rect(), RoundedRectangleRadius, RoundedRectangleRadius);

    painter->setClipPath(clippingPath);

    painter->drawPixmap(topLeft, m_bufferedImage);

    painter->restore();
}

auto Nedrysoft::JitterPlot::JitterBackgroundLayer::setRange(double targetJitter, double maximumJitter) -> void {
    m_targetJitter = targetJitter;
    m_maximumJitter = maximumJitter;
}
