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

#include "RouteTableItemDelegate.h"

#include "ColourManager.h"
#include "PingData.h"
#include "ThemeSupport.h"

#include <QDebug>
#include <QGradient>
#include <QHeaderView>
#include <QMap>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStandardItemModel>
#include <QTableView>

using namespace std::chrono_literals;

constexpr auto AverageLatencyRadius = 4;
constexpr auto CurrentLatencyLength = 3;
constexpr auto xOffset = ( AverageLatencyRadius * 2 );

constexpr auto DefaultLowRangeLatency = 100ms;
constexpr auto DefaultMidRangeLatency = 200ms;

constexpr auto roundedRectangleRadius = 10;
constexpr auto alternateRowFactor = 10;
constexpr auto tinyNumber = 0.0001;                             //! used to adjust a unit number to just under 1

constexpr auto NormalColourFactor = 100;
constexpr auto ActiveSelectedColourFactor = 105;
constexpr auto InactiveSelectedColourFactor = 102;

constexpr auto InvalidEntryLineWidth = 6;

constexpr auto OverrideSelectedColour = 1;

Nedrysoft::RouteAnalyser::RouteTableItemDelegate::RouteTableItemDelegate(QWidget *parent) :
        QStyledItemDelegate(parent),
        m_lowRangeLatency(DefaultLowRangeLatency),
        m_midRangeLatency(DefaultMidRangeLatency) {

}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::paint(
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const {

    if (!index.isValid()) {
        QStyledItemDelegate::paint(painter, option, index);

        return;
    }

    if (!index.siblingAtColumn(0).isValid()) {
        QStyledItemDelegate::paint(painter, option, index);

        return;
    }

    auto pingData = index.siblingAtColumn(0).data(Qt::UserRole + 1).value<Nedrysoft::RouteAnalyser::PingData *>();

    if (!pingData->hopValid() && ( index.column() != Nedrysoft::RouteAnalyser::PingData::Graph )) {
        paintInvalidHop(pingData, painter, option, index);

        return;
    }

    switch ( index.column()) {
        case Nedrysoft::RouteAnalyser::PingData::Graph: {
            paintGraph(pingData, painter, option, index);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::Hop: {
            paintHop(pingData, painter, option, index);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::Location: {
            paintBackground(pingData, painter, option, index);
            paintLocation(pingData, painter, option, index);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::IP: {
            paintBackground(pingData, painter, option, index);
            paintText(pingData->hostAddress(), painter, option, index);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::HostName: {
            paintBackground(pingData, painter, option, index);
            paintText(pingData->hostName(), painter, option, index);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::MinimumLatency: {
            paintBackground(pingData, painter, option, index);
            paintText(QString("%1").arg(
                    std::chrono::duration_cast<milliseconds_double>(pingData->m_minimumLatency).count(), 2, 'f', 2),
                      painter, option, index, Qt::AlignRight | Qt::AlignVCenter);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::MaximumLatency: {
            paintBackground(pingData, painter, option, index);
            paintText(QString("%1").arg(
                    std::chrono::duration_cast<milliseconds_double>(pingData->m_maximumLatency).count(), 2, 'f', 2),
                      painter, option, index, Qt::AlignRight | Qt::AlignVCenter);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::AverageLatency: {
            paintBackground(pingData, painter, option, index);
            paintText(QString("%1").arg(
                    std::chrono::duration_cast<milliseconds_double>(pingData->m_averageLatency).count(), 2, 'f', 2),
                      painter, option, index, Qt::AlignRight | Qt::AlignVCenter);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::CurrentLatency: {
            paintBackground(pingData, painter, option, index);
            paintText(QString("%1").arg(
                    std::chrono::duration_cast<milliseconds_double>(pingData->m_currentLatency).count(), 2, 'f', 2),
                      painter, option, index, Qt::AlignRight | Qt::AlignVCenter);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::PacketLoss: {
            paintBackground(pingData, painter, option, index);
            paintText(QString("%1").arg(pingData->packetLoss(), 2, 'f', 2), painter, option, index,
                      Qt::AlignRight | Qt::AlignVCenter);

            break;
        }

        case Nedrysoft::RouteAnalyser::PingData::Count: {
            paintBackground(pingData, painter, option, index);
            paintText(QString("%1").arg(pingData->count()), painter, option, index, Qt::AlignRight | Qt::AlignVCenter);

            break;
        }

        default: {
            QStyledItemDelegate::paint(painter, option, index);

            break;
        }
    }
}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::paintText(
        const QString &text, QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index,
        int alignment,
        int flags) const {

    Q_UNUSED(index)

    constexpr auto TextMargin = 5;
    auto pen = QPen();
    auto textColour = QColor();
    auto textRect = option.rect.adjusted(TextMargin, 0, -TextMargin, 0);

    painter->save();

    if (flags & OverrideSelectedColour) {
        if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
            textColour = option.palette.color(QPalette::Base);
        } else {
            textColour = option.palette.color(QPalette::Inactive, QPalette::Text);
        }
    } else if (option.state & QStyle::State_Selected) {
        if (option.state & QStyle::State_Active) {
            textColour = option.palette.color(QPalette::Active, QPalette::HighlightedText);
        } else {
            textColour = option.palette.color(QPalette::Inactive, QPalette::Text);
        }
    } else {
        if (option.state & QStyle::State_Active) {
            textColour = option.palette.color(QPalette::Active, QPalette::Text);
        } else {
            textColour = option.palette.color(QPalette::Inactive, QPalette::Text);
        }
    }

    pen = QPen(textColour);

    painter->setPen(pen);

    auto elidedText = painter->fontMetrics().elidedText(text, Qt::ElideRight, textRect.width());

    painter->drawText(textRect, alignment, elidedText);

    painter->restore();
}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::paintBackground(
        Nedrysoft::RouteAnalyser::PingData *pingData,
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const {

    Q_UNUSED(index)
    Q_UNUSED(pingData)

    if (option.state & QStyle::State_Selected) {
        auto brush = QBrush();

        if (option.state & QStyle::State_Active) {
            brush = option.palette.brush(QPalette::Active, QPalette::Highlight);
        } else {
            brush = option.palette.brush(QPalette::Inactive, QPalette::Highlight);
        }

        painter->fillRect(option.rect, brush);
    }
}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::paintLocation(
        Nedrysoft::RouteAnalyser::PingData *pingData,
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const {

    auto pen = QPen(QBrush(Nedrysoft::RouteAnalyser::ColourManager::getMaxColour()), option.rect.height() - InvalidEntryLineWidth);

    pen.setCapStyle(Qt::RoundCap);

    paintBackground(pingData, painter, option, index);

    if (!pingData->location().isEmpty()) {
        paintText(pingData->location(), painter, option, index);
    } else {
        auto rc = option.rect;

        pen.setWidth(InvalidEntryLineWidth);

        rc.adjust(pen.width() / 2, 0, -( pen.width() / 2 ), 0);

        painter->save();

        painter->setPen(pen);

        painter->drawLine(QPoint(rc.left(), rc.center().y()), QPoint(rc.right(), rc.center().y()));

        painter->restore();

        paintText(pingData->location(), painter, option, index);
    }
}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::paintInvalidHop(
        Nedrysoft::RouteAnalyser::PingData *pingData,
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const {

    auto tableView = qobject_cast<const QTableView *>(option.widget);
    auto pen = QPen(QBrush(Nedrysoft::RouteAnalyser::ColourManager::getMaxColour()), option.rect.height() - InvalidEntryLineWidth);

    auto visualIndex = tableView->horizontalHeader()->visualIndex(index.column());

    paintBackground(pingData, painter, option, index);

    pen.setCapStyle(Qt::RoundCap);

    painter->save();

    if (( option.state & QStyle::State_Selected ) && ( !tableView->hasFocus())) {
        pen.setColor(Qt::white);
    }

    auto rc = option.rect;

    if (index.column() == Nedrysoft::RouteAnalyser::PingData::Hop) {
        paintBubble(pingData, painter, option, index, Nedrysoft::RouteAnalyser::ColourManager::getMaxColour());
    }

    rc = option.rect;

    rc.setTop(option.rect.center().y());
    rc.setBottom(option.rect.center().y());

    pen.setWidth(InvalidEntryLineWidth);

    if (visualIndex == 0) {
        if (index.column() == Nedrysoft::RouteAnalyser::PingData::Hop) {
            rc.setLeft(rc.center().x());
        } else {
            rc.setLeft(rc.left() + ( pen.width() / 2 ));
        }
    }

    if (visualIndex == index.model()->columnCount() - 2) {
        if (index.column() == Nedrysoft::RouteAnalyser::PingData::Hop) {
            rc.setRight(rc.center().x());
        } else {
            rc.setRight(rc.right() - ( pen.width() / 2 ));
        }
    }

    painter->setPen(pen);

    painter->drawLine(QPoint(rc.left(), rc.center().y()), QPoint(rc.right(), rc.center().y()));

    painter->restore();

    if (index.column() == Nedrysoft::RouteAnalyser::PingData::Hop) {
        paintText(QString("%1").arg(pingData->hop()), painter, option, index, Qt::AlignHCenter | Qt::AlignVCenter,
                  OverrideSelectedColour);
    }
}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::paintBubble(
        Nedrysoft::RouteAnalyser::PingData *pingData,
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index, QRgb bubbleColour) const {

    Q_UNUSED(index)
    Q_UNUSED(pingData)

    auto tableView = qobject_cast<const QTableView *>(option.widget);
    auto bubbleRect = option.rect;
    auto pen = QPen(QBrush(bubbleColour), option.rect.height() - InvalidEntryLineWidth);

    pen.setCapStyle(Qt::RoundCap);

    bubbleRect.adjust(xOffset + ( pen.width() / 2 ), 0, -( xOffset + ( pen.width() / 2 )), 0);

    painter->save();

    if (( option.state & QStyle::State_Selected ) && ( !tableView->hasFocus())) {
        pen.setColor(Qt::white);
    }

    painter->setPen(pen);

    painter->drawLine(QPoint(bubbleRect.left(), bubbleRect.center().y()),
                      QPoint(bubbleRect.right(), bubbleRect.center().y()));

    painter->restore();
}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::paintHop(
        Nedrysoft::RouteAnalyser::PingData *pingData,
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const {

    constexpr auto interpolationTime = 1000.0;
    auto pen = QPen(QBrush(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()), option.rect.height() - InvalidEntryLineWidth);
    auto bubbleColour = QColor(Qt::white);

    QMap<double, QRgb> gradientMap;

    gradientMap[0] = Nedrysoft::RouteAnalyser::ColourManager::getMinColour();
    gradientMap[std::chrono::duration<double, std::milli>(m_lowRangeLatency).count() /
                interpolationTime] = Nedrysoft::RouteAnalyser::ColourManager::getMidColour();
    gradientMap[std::chrono::duration<double, std::milli>(m_midRangeLatency).count() /
                interpolationTime] = Nedrysoft::RouteAnalyser::ColourManager::getMaxColour();
    gradientMap[1] = Nedrysoft::RouteAnalyser::ColourManager::getMaxColour();

    pen.setCapStyle(Qt::RoundCap);

    paintBackground(pingData, painter, option, index);

    if (( option.state & ( QStyle::State_Active )) ||
        ( !( option.state & QStyle::State_Active ) && !( option.state & QStyle::State_Selected ))) {
        bubbleColour = getInterpolatedColour(gradientMap,
                                             pingData->latency(Nedrysoft::RouteAnalyser::PingData::AverageLatency));
    }

    paintBubble(pingData, painter, option, index, bubbleColour.rgb());

    paintText(QString("%1").arg(pingData->hop()), painter, option, index, Qt::AlignHCenter | Qt::AlignVCenter,
              OverrideSelectedColour);
}

auto Nedrysoft::RouteAnalyser::RouteTableItemDelegate::getInterpolatedColour(
        const QMap<double, QRgb> &keyFrames,
        double value) const -> QRgb {

    constexpr auto interpolationTime = 1000.0;
    QVariantAnimation colourInterpolator;
    auto keyFrameKeys = keyFrames.keys();

    value = qMin<double>(qMax<double>(value, 0), 1);

    if (keyFrames.contains(value)) {
        return keyFrames.value(value);
    }

    colourInterpolator.setEasingCurve(QEasingCurve::Linear);
    colourInterpolator.setDuration(static_cast<int>(interpolationTime));

    for (const auto &key : keyFrameKeys) {
        colourInterpolator.setKeyValueAt(key, QColor::fromRgb(keyFrames.value(key)));
    }

    colourInterpolator.setCurrentTime(static_cast<int>(value * interpolationTime));

    auto colour = colourInterpolator.currentValue().value<QColor>();

    return colour.rgb();
}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::paintGraph(
        Nedrysoft::RouteAnalyser::PingData *pingData,
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const {

    auto thisRect = option.rect.adjusted(xOffset, 0, -xOffset, 0);
    auto startPoint = QPointF();
    auto endPoint = QPointF();
    auto colourFactor = NormalColourFactor;
    auto smoothGradient = true;

    auto graphMaxLatency = pingData->tableModel()->property("graphMaxLatency").toDouble();

    if (index.row() & 1) {
        colourFactor = NormalColourFactor+alternateRowFactor;
    }

    painter->save();

    // set the clipping rect to a rounding rectangle, this looks much better on dark mode than hard corners

    auto tableView = qobject_cast<const QTableView *>(option.widget);

    auto clippingRect = option.widget->contentsRect();

    QPainterPath clippingPath;

    // adjust the left hand side of the rectangle to be the the graph column and adjust the height to account for
    // the header height.  Our clipping path extends over the whole graph area.

    clippingRect.setLeft(option.rect.left()+xOffset);
    clippingRect.adjust(0,0,0, -tableView->horizontalHeader()->height());

    clippingPath.addRoundedRect(clippingRect, roundedRectangleRadius, roundedRectangleRadius);

    painter->setClipPath(clippingPath);

    auto lowStop = m_lowRangeLatency.count() / graphMaxLatency;
    auto midStop = m_midRangeLatency.count() / graphMaxLatency;

    auto blankRect = option.rect;

    blankRect.setRight(thisRect.left());

    if (option.state & QStyle::State_Selected) {
        auto brush = QBrush();

        if (option.state & QStyle::State_Active) {
            brush = option.palette.brush(QPalette::Active, QPalette::Highlight);
            colourFactor = ActiveSelectedColourFactor;
        } else {
            brush = option.palette.brush(QPalette::Inactive, QPalette::Highlight);
            colourFactor = InactiveSelectedColourFactor;
        }

        painter->fillRect(blankRect, brush);
    }

    auto rect = thisRect;

    QLinearGradient graphGradient = QLinearGradient(QPoint(rect.left(), rect.y()), QPoint(rect.right(), rect.y()));

    if (lowStop > 1) {
        graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()).darker(colourFactor));
        graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()).darker(colourFactor));
    } else {
        if (midStop > 1) {
            if (lowStop < 1) {
                graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()).darker(colourFactor));
                graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMidColour()).darker(colourFactor));
            }
        } else {
            graphGradient.setColorAt(0, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()).darker(colourFactor));
            graphGradient.setColorAt(lowStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMidColour()).darker(colourFactor));
            graphGradient.setColorAt(midStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMaxColour()).darker(colourFactor));
            graphGradient.setColorAt(1, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMaxColour()).darker(colourFactor));
        }
    }

    if (!smoothGradient) {
        graphGradient.setColorAt(lowStop, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMidColour()).darker(colourFactor));
        graphGradient.setColorAt(lowStop - tinyNumber, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMinColour()).darker(colourFactor));
        graphGradient.setColorAt(midStop - tinyNumber, QColor(Nedrysoft::RouteAnalyser::ColourManager::getMidColour()).darker(colourFactor));
    }

    painter->fillRect(rect, graphGradient);

    auto endRect = rect;
    auto floatingPointRect = QRectF(rect);

    endRect.setLeft(rect.right());
    endRect.setRight(option.rect.right());

    painter->fillRect(endRect, QBrush(graphGradient.stops().last().second));

    // draw low and mid stop point lines if visible

    if (lowStop < 1) {
        startPoint = QPointF(floatingPointRect.left() + ( lowStop * floatingPointRect.width()),
                             floatingPointRect.top());
        endPoint = QPointF(floatingPointRect.left() + ( lowStop * floatingPointRect.width()),
                           floatingPointRect.bottom());
    }

    auto pen = QPen(Qt::DashLine);

    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        pen.setColor(Qt::black);
    } else {
        pen.setColor(Qt::lightGray);
    }

    double dashLength = 0;

    for (auto sectionLength : pen.dashPattern()) {
        dashLength += sectionLength;
    }

    pen.setDashOffset(fmod(option.rect.top(), dashLength));

    painter->setPen(pen);

    painter->drawLine(startPoint, endPoint);

    if (midStop < 1) {
        startPoint = QPointF(floatingPointRect.left() + ( midStop * floatingPointRect.width()),
                             floatingPointRect.top());
        endPoint = QPointF(floatingPointRect.left() + ( midStop * floatingPointRect.width()),
                           floatingPointRect.bottom());
    }

    painter->drawLine(startPoint, endPoint);

    // draw hop graph item

    if (pingData->hopValid()) {
        auto centrePoint = QPointF();

        // this is a valid hop, so draw accordingly

        auto currentLatency = pingData->latency(Nedrysoft::RouteAnalyser::PingData::CurrentLatency);
        auto minimumLatency = pingData->latency(Nedrysoft::RouteAnalyser::PingData::MinimumLatency);
        auto maximumLatency = pingData->latency(Nedrysoft::RouteAnalyser::PingData::MaximumLatency);

        if (( minimumLatency >= 0 ) && ( maximumLatency >= 0 )) {
            // draw min/max latency timeline

            painter->setPen(Qt::gray);

            startPoint.setX(thisRect.left() + ( thisRect.width() * ( minimumLatency / graphMaxLatency )));
            startPoint.setY(thisRect.center().y());

            endPoint.setX(thisRect.left() + ( thisRect.width() * ( maximumLatency / graphMaxLatency )));
            endPoint.setY(thisRect.center().y());

            painter->drawLine(startPoint, endPoint);

            startPoint.setX(thisRect.left() + ( thisRect.width() * ( minimumLatency / graphMaxLatency )));
            startPoint.setY(thisRect.center().y() - 3);

            endPoint.setX(thisRect.left() + ( thisRect.width() * ( minimumLatency / graphMaxLatency )));
            endPoint.setY(thisRect.center().y() + 3);

            painter->drawLine(startPoint, endPoint);

            startPoint.setX(thisRect.left() + ( thisRect.width() * ( maximumLatency / graphMaxLatency )));
            endPoint.setX(thisRect.left() + ( thisRect.width() * ( maximumLatency / graphMaxLatency )));

            painter->drawLine(startPoint, endPoint);
        }

        if (currentLatency >= 0) {
            // draw current latency mark

            painter->setPen(Qt::blue);

            centrePoint.setX(thisRect.left() + (( thisRect.width() * ( currentLatency / graphMaxLatency ))));
            centrePoint.setY(thisRect.center().y());

            startPoint = centrePoint + QPoint(-CurrentLatencyLength, -CurrentLatencyLength);
            endPoint = centrePoint + QPoint(CurrentLatencyLength, CurrentLatencyLength);

            painter->drawLine(startPoint, endPoint);

            startPoint = centrePoint + QPoint(CurrentLatencyLength, -CurrentLatencyLength);
            endPoint = centrePoint + QPoint(-CurrentLatencyLength, CurrentLatencyLength);

            painter->drawLine(startPoint, endPoint);
        }
    }

    if (pingData->tableModel()->property("showHistorical").toBool()) {
        drawLatencyLine(Nedrysoft::RouteAnalyser::PingData::HistoricalLatency, pingData, painter, option, index,
                        QPen(Qt::darkGray, 2, Qt::DotLine));
    }

    drawLatencyLine(Nedrysoft::RouteAnalyser::PingData::AverageLatency, pingData, painter, option, index,
                    QPen(Qt::red, 1, Qt::SolidLine));

    painter->restore();
}

void Nedrysoft::RouteAnalyser::RouteTableItemDelegate::drawLatencyLine(
        int field,
        Nedrysoft::RouteAnalyser::PingData *pingData,
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index,
        const QPen &pen) const {

    auto thisRect = option.rect.adjusted(xOffset, 0, -xOffset, 0);
    auto nextRect = thisRect;
    auto previousRect = thisRect;
    auto startPoint = QPointF();
    auto endPoint = QPointF();

    auto graphMaxLatency = pingData->tableModel()->property("graphMaxLatency").toDouble();
    const auto tableView = qobject_cast<const QTableView *>(option.widget);

    auto previousData = getSiblingData(index, -1, tableView, previousRect);
    auto nextData = getSiblingData(index, 1, tableView, nextRect);

    painter->save();

    if (previousData) {
        previousRect.adjust(xOffset, 0, -xOffset, 0);
    }

    if (nextData) {
        nextRect.adjust(xOffset, 0, -xOffset, 0);
    }

    if (pingData->hopValid()) {
        auto centrePoint = QPointF();

        painter->setPen(pen);

        if (previousData) {
            // there is a valid previous hop, so draw the line from the previous hop to this hop

            startPoint.setX(
                    previousRect.left() + ( previousRect.width() * ( previousData->latency(field) / graphMaxLatency )));
            startPoint.setY(previousRect.center().y());

            endPoint.setX(thisRect.left() + ( thisRect.width() * ( pingData->latency(field) / graphMaxLatency )));
            endPoint.setY(thisRect.center().y());

            painter->drawLine(startPoint, endPoint);
        }

        if (nextData) {
            // there is a valid next hop, so draw the line this hop to the next hop

            startPoint.setX(thisRect.left() + ( thisRect.width() * ( pingData->latency(field) / graphMaxLatency )));
            startPoint.setY(thisRect.center().y());

            endPoint.setX(nextRect.left() + ( nextRect.width() * ( nextData->latency(field) / graphMaxLatency )));
            endPoint.setY(nextRect.center().y());

            painter->drawLine(startPoint, endPoint);
        }

        centrePoint.setX(thisRect.left() + ( thisRect.width() * ( pingData->latency(field) / graphMaxLatency )));
        centrePoint.setY(thisRect.center().y());

        painter->drawEllipse(centrePoint, AverageLatencyRadius, AverageLatencyRadius);
    } else {
        if (( previousData ) && ( nextData )) {
            // this is a invalid hop

            painter->setPen(pen);

            startPoint.setX(
                    previousRect.left() + ( previousRect.width() * ( previousData->latency(field) / graphMaxLatency )));
            startPoint.setY(previousRect.center().y());

            endPoint.setX(nextRect.left() + ( nextRect.width() * ( nextData->latency(field) / graphMaxLatency )));
            endPoint.setY(nextRect.center().y());

            painter->drawLine(startPoint, endPoint);
        }
    }

    painter->restore();
}

auto Nedrysoft::RouteAnalyser::RouteTableItemDelegate::getSibling(
        QModelIndex modelIndex,
        int adjustment) const -> QModelIndex {

    while (true) {
        modelIndex = modelIndex.siblingAtRow(modelIndex.row() + adjustment);

        if (!modelIndex.isValid()) {
            break;
        }

        auto pingData = modelIndex.siblingAtColumn(0).data(Qt::UserRole + 1).value<PingData *>();

        if (pingData->hopValid()) {
            break;
        }
    }

    return modelIndex;
}

auto Nedrysoft::RouteAnalyser::RouteTableItemDelegate::getSiblingData(
        QModelIndex modelIndex,
        int adjustment,
        const QTableView *tableView,
        QRect &rect) const -> Nedrysoft::RouteAnalyser::PingData * {

    auto nextModelIndex = getSibling(modelIndex, adjustment);

    if (nextModelIndex.isValid()) {
        rect = tableView->visualRect(nextModelIndex);

        return nextModelIndex.siblingAtColumn(0).data(Qt::UserRole + 1).value<Nedrysoft::RouteAnalyser::PingData *>();
    }

    return nullptr;
}
