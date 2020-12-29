/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 29/12/2020.
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

#include "TrimmerWidget.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

auto constexpr gripperInsertWidth = 2;
auto constexpr gripperInsertHeight = 30;
auto constexpr viewportBorderSize = 2;
auto constexpr trimmerBackgroundColour = qRgb(0x43,0x43, 0x43);
auto constexpr viewportBackgroundColour = qRgb(0x80, 0x80, 0x80);
auto constexpr viewportBorderColour = qRgb(0xff, 0xcc, 0x00);
auto constexpr trimmerCornerRadius = 8;

Nedrysoft::RouteAnalyser::TrimmerWidget::TrimmerWidget(QWidget *parent) :
        QWidget(parent),
        m_editingState(State::NotEditing),
        m_viewportPosition(0.3),
        m_viewportSize(0.5) {

}

Nedrysoft::RouteAnalyser::TrimmerWidget::~TrimmerWidget() {

}

auto Nedrysoft::RouteAnalyser::TrimmerWidget::paintEvent(QPaintEvent *event) -> void {
    QWidget::paintEvent(event);

    auto contentRect = rect();
    double contentWidth = contentRect.width();
    double viewportWidth = contentWidth * m_viewportSize;

    QPixmap pixmap(rect().size());

    QPainter painter(&pixmap);

    // draw the trimmer background

    painter.setPen(Qt::NoPen);

    painter.setBrush(QColor(trimmerBackgroundColour));
    painter.drawRoundedRect(contentRect, trimmerCornerRadius, trimmerCornerRadius, Qt::AbsoluteSize);

    painter.translate(m_viewportPosition*contentWidth, 0);

    auto gripperRectLeft = contentRect;
    auto gripperRectRight = contentRect;

    gripperRectLeft.setWidth(trimmerCornerRadius-1);

    gripperRectRight.setLeft(viewportWidth-trimmerCornerRadius);
    gripperRectRight.setWidth(trimmerCornerRadius-1);

    auto brush = QBrush(viewportBorderColour);

    auto viewportBackgroundBrush = QBrush(viewportBackgroundColour);

    painter.fillRect(QRect(gripperRectLeft.topRight(), gripperRectRight.bottomLeft()), viewportBackgroundBrush);

    // left gripper

    painter.setBrush(brush);

    painter.drawRoundedRect(gripperRectLeft, trimmerCornerRadius, trimmerCornerRadius, Qt::AbsoluteSize);
    painter.fillRect(gripperRectLeft.adjusted(trimmerCornerRadius/2,0,0,0), brush);

    painter.fillRect(
            QRect(
                gripperRectLeft.center().x()-(gripperInsertWidth/2),
                gripperRectLeft.center().y()-(gripperInsertHeight/2),
                gripperInsertWidth+1,
                gripperInsertHeight+1 ),
            Qt::darkYellow );

    // right gripper

    painter.drawRoundedRect(gripperRectRight, trimmerCornerRadius, trimmerCornerRadius, Qt::AbsoluteSize);
    painter.fillRect(gripperRectRight.adjusted(0, 0, -(trimmerCornerRadius/2), 0), brush);

    painter.fillRect(
            QRect(
                    gripperRectRight.center().x()-(gripperInsertWidth/2),
                    gripperRectRight.center().y()-(gripperInsertHeight/2),
                    gripperInsertWidth+1,
                    gripperInsertHeight+1 ),

            Qt::darkYellow );

    // top and bottom border

    painter.fillRect(QRect(
            gripperRectLeft.right(),
            gripperRectLeft.top(),
            gripperRectRight.left()-4,
            gripperRectLeft.top()+viewportBorderSize),
            brush );

    painter.fillRect(QRect(
            gripperRectLeft.right(),
            gripperRectLeft.bottom()-viewportBorderSize,
            gripperRectRight.left()-4,
            gripperRectLeft.bottom()),
            brush );

    painter.end();

    QPainter widgetPainter(this);

    widgetPainter.drawPixmap(QPoint(0,0), pixmap);
}

QSize Nedrysoft::RouteAnalyser::TrimmerWidget::sizeHint() const {
    return QSize(200,60);
}

void Nedrysoft::RouteAnalyser::TrimmerWidget::mousePressEvent(QMouseEvent *event) {
    double viewportSize = static_cast<double>(rect().size().width())*m_viewportSize;
    double viewportOrigin = static_cast<double>(rect().size().width())*m_viewportPosition;

    double origin = event->pos().x()-viewportOrigin;

    if (origin<0) {
        return;
    }

    if (origin>viewportSize) {
        return;
    }

    if (origin>viewportSize-trimmerCornerRadius) {
        m_editingState = State::MovingViewportEnd;
    } else if (origin<trimmerCornerRadius) {
        m_editingState = State::MovingViewportStart;
    } else {
        m_editingState = State::MovingViewport;
    }

    m_origin = event->pos().x();
    m_offset = m_origin;
}

void Nedrysoft::RouteAnalyser::TrimmerWidget::mouseReleaseEvent(QMouseEvent *event) {
    m_editingState = State::NotEditing;
}

void Nedrysoft::RouteAnalyser::TrimmerWidget::mouseMoveEvent(QMouseEvent *event) {
    switch(m_editingState) {
        case State::NotEditing: {
            return;
        }

        case State::MovingViewport: {
            double trimmerWidth = rect().width();
            double delta = event->pos().x()-m_origin;
            double min = static_cast<double>(trimmerCornerRadius/2)/trimmerWidth;
            double max = 1.0-((static_cast<double>(trimmerCornerRadius/2))/trimmerWidth)-m_viewportSize;

            m_viewportPosition += (delta/trimmerWidth);

            if (m_viewportPosition<=min) {
                m_viewportPosition = min;
            } else if (m_viewportPosition>=max) {
                m_viewportPosition = max;
            }

            m_origin = event->pos().x();

            update();

            return;
        }

        case State::MovingViewportStart: {
            return;
        }

        case State::MovingViewportEnd: {
            return;
        }
    }
}