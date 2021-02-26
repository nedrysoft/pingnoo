/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 13/12/2020.
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

#include "LatencyWidget.h"

#include "ColourDialog.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QWidget>
#include <QWindow>


#include <QDebug>

constexpr auto widgetRadius = 5;

Nedrysoft::RouteAnalyser::LatencyWidget::LatencyWidget(QWidget *parent) :
        QWidget(parent),
        m_colour(Qt::transparent),
        m_text(tr("")) {

}

Nedrysoft::RouteAnalyser::LatencyWidget::~LatencyWidget() = default;

auto Nedrysoft::RouteAnalyser::LatencyWidget::getContrastColour(QColor colour) -> QColor {
    if (colour.lightnessF()<0.5) {
        return Qt::white;
    }

    return Qt::black;
}

auto Nedrysoft::RouteAnalyser::LatencyWidget::paintEvent(QPaintEvent *event) -> void {
    QPainter painter(this);

    QPainterPath path;

    path.addRoundedRect(rect(), widgetRadius, widgetRadius);

    painter.fillPath(path, m_colour);

    painter.setPen(getContrastColour(m_colour));

    painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, m_text);
}

auto Nedrysoft::RouteAnalyser::LatencyWidget::setText(QString text) -> void {
    m_text = text;
}

auto Nedrysoft::RouteAnalyser::LatencyWidget::text() -> QString {
    return m_text;
}

auto Nedrysoft::RouteAnalyser::LatencyWidget::setColour(QColor colour) -> void {
    m_colour = colour;

    update();
}

auto Nedrysoft::RouteAnalyser::LatencyWidget::colour() -> QColor {
    return m_colour;
}

auto Nedrysoft::RouteAnalyser::LatencyWidget::mouseDoubleClickEvent(QMouseEvent *event) -> void {
    auto colourDialog = ColourDialog::getInstance();

    colourDialog->open(m_colour, m_text, [=](QColor colour) {
        Q_EMIT colourChanged(colour);
    });
}