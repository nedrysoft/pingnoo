/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 28/12/2020.
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

#include "BarChart.h"

constexpr auto roundedRectangleRadius = 10;

Nedrysoft::RouteAnalyser::BarChart::BarChart(QCPAxis *keyAxis, QCPAxis *valueAxis) :
        QCPBars(keyAxis, valueAxis) {

}

void Nedrysoft::RouteAnalyser::BarChart::draw(QCPPainter *painter) {
    auto rect = parentPlot()->axisRect()->rect();
    QPainterPath clippingPath;

    rect.adjust(0,1,0,0);

    clippingPath.addRoundedRect(rect, roundedRectangleRadius, roundedRectangleRadius);

    painter->save();
    painter->setClipPath(clippingPath);

    QCPBars::draw(painter);

    painter->restore();
}