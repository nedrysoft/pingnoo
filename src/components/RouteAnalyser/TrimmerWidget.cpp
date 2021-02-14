/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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

#include "ThemeSupport.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

auto constexpr gripInsertWidth = 2;
auto constexpr gripInsertHeight = 30;
auto constexpr gripInnerColour = qRgb(0x80, 0x80, 0x00);
auto constexpr viewportGripBorderSize = 2;
auto constexpr trimmerBackgroundColourDark = qRgb(0x43,0x43, 0x43);
auto constexpr trimmerBackgroundColourLight = qRgb(0xff,0xff, 0xff);
auto constexpr viewportBackgroundColourDark = qRgb(0x80, 0x80, 0x80);
auto constexpr viewportBackgroundColourLight = qRgb(0xdd, 0xdd, 0xdd);
auto constexpr viewportGripColour = qRgb(0xff, 0xcc, 0x00);
#if defined(Q_OS_MACOS)
constexpr auto darkModeTrimmerBackgroundFactor = 150;
#else
constexpr auto darkModeTrimmerBackgroundFactor = 100;
#endif
constexpr auto lightModeGripBackgroundFactor = 100;
constexpr auto lightModeTrimmerBackgroundFactor = 120;
constexpr auto lightModeViewportBackgroundFactor = 100;

auto constexpr trimmerCornerRadius = 8;
auto constexpr defaultWidth = 200;
auto constexpr defaultHeight = 60;

Nedrysoft::RouteAnalyser::TrimmerWidget::TrimmerWidget(QWidget *parent) :
        QWidget(parent),
        m_editingState(State::NotEditing),
        m_viewportPosition(0),
        m_viewportSize(0.5),
        m_canBeResized(false) {

}

Nedrysoft::RouteAnalyser::TrimmerWidget::~TrimmerWidget() {

}

auto Nedrysoft::RouteAnalyser::TrimmerWidget::paintEvent(QPaintEvent *event) -> void {
    QWidget::paintEvent(event);

    auto contentRect = rect();
    double contentWidth = contentRect.width();
    double viewportWidth = contentWidth * m_viewportSize;
    QPixmap pixmap(rect().size());

    pixmap.fill(Qt::transparent);

    /**
     * the control is drawn to a pixmap buffer as it produces a repeatable image, drawing directly to the widget
     * results in a platform dependent image, this is particularly important for macOS where coordinates are real
     * numbers and depending on pixel position and therefore primitives may not appear as expected.
     */

    QPainter painter(&pixmap);

    QRectF gripperRectLeft = QRectF(contentRect);
    QRectF gripperRectRight = QRectF(contentRect);

    QBrush gripBrush;
    QBrush viewportBackgroundBrush;
    QBrush trimmerBackgroundBrush;
    QBrush gripInnerBrush;

    if (isEnabled()) {
        gripBrush = QBrush(viewportGripColour);
        gripInnerBrush = QBrush(gripInnerColour);

        if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
            viewportBackgroundBrush = QBrush(viewportBackgroundColourDark);
            trimmerBackgroundBrush = QBrush(trimmerBackgroundColourDark);
        } else {
            viewportBackgroundBrush = QBrush(viewportBackgroundColourLight);
            trimmerBackgroundBrush = QBrush(trimmerBackgroundColourLight);
        }
    } else {
        if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
            gripBrush = QBrush(QColor(Qt::lightGray).darker());
            viewportBackgroundBrush = QBrush(gripBrush.color().darker());
            trimmerBackgroundBrush = QBrush(gripBrush.color().darker(darkModeTrimmerBackgroundFactor));
            gripInnerBrush = QBrush(viewportBackgroundBrush.color().darker());
        } else {
            gripBrush = QBrush(
                    QColor(Qt::lightGray).darker(lightModeGripBackgroundFactor) );

            viewportBackgroundBrush = QBrush(
                    QColor(viewportBackgroundColourLight).darker(lightModeViewportBackgroundFactor) );

            trimmerBackgroundBrush = QBrush(
                    QColor(trimmerBackgroundColourLight).darker(lightModeTrimmerBackgroundFactor) );

            gripInnerBrush = QBrush(viewportBackgroundBrush.color().lighter());
        }
    }

    auto colourPen = QPen(QColor(viewportGripColour));

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);

    painter.setBrush(trimmerBackgroundBrush);

    // draw the main trimmer background.

    painter.drawRoundedRect(
            contentRect.adjusted(0,0,-1,0),
            static_cast<double>(trimmerCornerRadius)/2.0,
            static_cast<double>(trimmerCornerRadius)/2.0,
            Qt::AbsoluteSize );

    painter.translate(m_viewportPosition*contentWidth, 0);

    // set the gripper rectangles to the correct rectangles.

    gripperRectLeft.setWidth(trimmerCornerRadius-1);

    gripperRectRight.setLeft(viewportWidth-trimmerCornerRadius);
    gripperRectRight.setWidth(trimmerCornerRadius-1);

    // draw the highlight background for the viewport area.

    painter.fillRect(QRectF(gripperRectLeft.topRight(), gripperRectRight.bottomLeft()), viewportBackgroundBrush);

    painter.setBrush(gripBrush);

    // draw viewport left (start) grip

    painter.drawRoundedRect(
            gripperRectLeft,
            static_cast<double>(trimmerCornerRadius)/2.0,
            static_cast<double>(trimmerCornerRadius)/2.0,
            Qt::AbsoluteSize );

    painter.fillRect(
            QRectF(gripperRectLeft.adjusted(static_cast<double>(trimmerCornerRadius)/2.0, 0, 0, 0)),
            gripBrush );

    painter.fillRect(
            QRectF(
                gripperRectLeft.center().x()-(static_cast<double>(gripInsertWidth)/2.0),
                gripperRectLeft.center().y()-(static_cast<double>(gripInsertHeight)/2.0),
                gripInsertWidth+1,
                gripInsertHeight+1 ),

            gripInnerBrush );

    // draw viewport right (end) grip

    painter.drawRoundedRect(
            gripperRectRight,
            static_cast<double>(trimmerCornerRadius)/2.0,
            static_cast<double>(trimmerCornerRadius)/2.0,
            Qt::AbsoluteSize );

    painter.fillRect(gripperRectRight.adjusted(0, 0, -(trimmerCornerRadius/2), 0), gripBrush);

    painter.fillRect(
            QRectF(
                gripperRectRight.center().x()-(static_cast<double>(gripInsertWidth)/2.0),
                gripperRectRight.center().y()-(static_cast<double>(gripInsertHeight)/2.0),
                gripInsertWidth+1,
                gripInsertHeight+1 ),

            gripInnerBrush );

    // top and bottom viewport border

    painter.fillRect(
            QRectF(
                gripperRectLeft.right(),
                gripperRectLeft.top(),
                gripperRectRight.left()-(static_cast<double>(trimmerCornerRadius)/2.0),
                gripperRectLeft.top()+viewportGripBorderSize ),

            gripBrush );

    painter.fillRect(
            QRectF(
                gripperRectLeft.right(),
                gripperRectLeft.bottom()-viewportGripBorderSize,
                gripperRectRight.left()-(static_cast<double>(trimmerCornerRadius)/2.0),
                gripperRectLeft.bottom() ),

            gripBrush );

    painter.end();

    // paint the pixmap to the widget.

    QPainter widgetPainter(this);

    widgetPainter.drawPixmap(QPoint(0,0), pixmap);
}

QSize Nedrysoft::RouteAnalyser::TrimmerWidget::sizeHint() const {
    return QSize(defaultWidth, defaultHeight);
}

void Nedrysoft::RouteAnalyser::TrimmerWidget::mousePressEvent(QMouseEvent *event) {
    double viewportSize = static_cast<double>(rect().size().width())*m_viewportSize;
    double viewportOrigin = static_cast<double>(rect().size().width())*m_viewportPosition;

    double origin = event->pos().x()-viewportOrigin;

    if (!isEnabled()) {
        return;
    }

    if (origin<0) {
        return;
    }

    if (origin>viewportSize) {
        return;
    }

    if (origin>viewportSize-trimmerCornerRadius) {
        if (m_canBeResized) {
            if (!m_flags.testFlag(TrimmerFlag::FixedEnd)) {
                m_editingState = State::MovingViewportEnd;
            }
        }
    } else if (origin<trimmerCornerRadius) {
        if (m_canBeResized) {
            if (!m_flags.testFlag(TrimmerFlag::FixedStart)) {
                m_editingState = State::MovingViewportStart;
            }
        }
    } else {
        if (!m_flags) {
            m_editingState = State::MovingViewport;
        }
    }

    m_origin = event->pos().x();
    m_viewportEnd = m_viewportPosition+m_viewportSize;
}

void Nedrysoft::RouteAnalyser::TrimmerWidget::mouseReleaseEvent(QMouseEvent *event) {
    m_editingState = State::NotEditing;
}

void Nedrysoft::RouteAnalyser::TrimmerWidget::mouseMoveEvent(QMouseEvent *event) {
    double trimmerWidth = rect().width();
    double delta = static_cast<double>(event->pos().x()-m_origin)/trimmerWidth;

    switch(m_editingState) {
        case State::NotEditing: {
            return;
        }

        case State::MovingViewport: {
            double min = 0;
            double max = 1.0-m_viewportSize;

            m_viewportPosition += delta;

            if (m_viewportPosition<min) {
                m_viewportPosition = min;
            } else if (m_viewportPosition>max) {
                m_viewportPosition = max;
            }

            break;
        }

        case State::MovingViewportStart: {
            double min = 0;
            double max = m_viewportEnd-(static_cast<double>(trimmerCornerRadius*2.0)/trimmerWidth);

            m_viewportPosition += delta;

            if (m_viewportPosition<min) {
                m_viewportPosition = min;
            } else if (m_viewportPosition>max) {
                m_viewportPosition = max;
            }

            m_viewportSize = m_viewportEnd-m_viewportPosition;

            break;
        }

        case State::MovingViewportEnd: {
            double min = static_cast<double>(trimmerCornerRadius*2.0)/trimmerWidth;
            double max = 1-m_viewportPosition;

            m_viewportSize += delta;

            if (m_viewportSize<min) {
                m_viewportSize = min;
            } else if (m_viewportSize>max) {
                m_viewportSize = max;
            }

            break;
        }
    }

    m_origin = event->pos().x();

    update();

    Q_EMIT positionChanged(viewportStart(), viewportEnd());
}

auto Nedrysoft::RouteAnalyser::TrimmerWidget::setViewport(double start, double end) -> void {
    if (start>end) {
        std::swap(start, end);
    }

    m_viewportPosition = qMax(start, 0.0);
    m_viewportSize = qMin(end-start, 1.0);

    update();
}

auto Nedrysoft::RouteAnalyser::TrimmerWidget::viewportStart() -> double {
    return qMin(qMax(0.0, m_viewportPosition), 1.0);
}

auto Nedrysoft::RouteAnalyser::TrimmerWidget::viewportEnd() -> double {
    return qMin(qMax(0.0, m_viewportPosition+m_viewportSize), 1.0);
}

auto Nedrysoft::RouteAnalyser::TrimmerWidget::viewportSize() -> double {
    return viewportEnd()-viewportStart();
}

auto Nedrysoft::RouteAnalyser::TrimmerWidget::setFlags(TrimmerWidget::TrimmerFlags flags) -> void {
    m_flags = flags;
}

auto Nedrysoft::RouteAnalyser::TrimmerWidget::setResizable(bool canBeResized) -> void {
    m_canBeResized = canBeResized;
}