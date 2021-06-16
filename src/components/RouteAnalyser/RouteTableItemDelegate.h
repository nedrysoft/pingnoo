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

#ifndef PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTETABLEITEMDELEGATE_H
#define PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTETABLEITEMDELEGATE_H

#include "PingData.h"
#include <QMap>
#include <QStyledItemDelegate>
#include <cmath>

class QStandardItem;
class QTableView;

namespace Nedrysoft { namespace RouteAnalyser {
    class PingData;

    /**
     * @brief       The RouteTableItemDelegate class provides a item delegate to draw a hop in a QTableView.
     */
    class RouteTableItemDelegate :
            public QStyledItemDelegate {

        private:
            class MaximumValue {
                public:
                    PingData *m_currentData;
            };


        public:
            /**
             * @brief       Constructs a new RouteTableItemDelegate instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            RouteTableItemDelegate(QWidget *parent = 0);

            /**
             * @brief       Reimplements: paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)
             *
             * @param[in]   painter the QPainter to draw to.
             * @param[in]   option information about the item being drawn.
             * @param[in]   index the index of the item in the model.
             */
            auto paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const -> void;

        private:

            /**
             * @brief       Returns a route item sibling.
             *
             * @details     Finds the previous or next valid item in the route list, a valid item is a hop that
             *              responded to the ping, ignores items that did not respond.  Will return invalid model
             *              index if there is not a previous/next item that is valid.
             *
             * @param[in]   modelIndex the starting model index.
             * @param[in]   adjustment set to 1 to look for the next valid, -1 for previous.
             *
             * @returns     The model index of the next/previous item. (or invalid)
             */
            auto getSibling(QModelIndex modelIndex, int adjustment) const -> QModelIndex;

            /**
             * @brief       Returns the route sibling data.
             *
             * @details     calls getSibling, but returns the PingData associated with the sibling and the visual
             *              rectangle as well.
             *
             * @param[in]   modelIndex the starting model index.
             * @param[in]   adjustment set to 1 to look for the next valid, -1 for previous.
             * @param[in]   tableView view associated with this.
             * @param[out]  rect the visual rectangle of the item.
             *
             * @returns     The ping data for the next/previous item. (or invalid)
             */
            auto getSiblingData(
                QModelIndex modelIndex,
                int adjustment,
                const QTableView *tableView,
                QRect &rect
            ) const -> Nedrysoft::RouteAnalyser::PingData *;

            /**
             * @brief       Paints the graph column.
             *
             * @param[in]   pingData the data for the item.
             * @param[in]   painter the QPainter to draw to.
             * @param[in]   option the painter options.
             * @param[in]   index the model index of the cell.
             */
            auto paintGraph(
                Nedrysoft::RouteAnalyser::PingData *pingData,
                QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index
            ) const -> void;

            /**
             * @brief       Paints the background of an item.
             *
             * @param[in]   pingData the data for the item.
             * @param[in]   painter the QPainter to draw to.
             * @param[in]   option the painter options.
             * @param[in]   index the model index of the cell.
             */
            auto paintBackground(
                Nedrysoft::RouteAnalyser::PingData *pingData,
                QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index
            ) const -> void;

            /**
             * @brief       Paints the location column.
             *
             * @param[in]   pingData the data for the item.
             * @param[in]   painter the QPainter to draw to.
             * @param[in]   option the painter options.
             * @param[in]   index the model index of the cell.
             */
            auto paintLocation(
                Nedrysoft::RouteAnalyser::PingData *pingData,
                QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index
            ) const -> void;

            /**
             * @brief       Paints an invalid hop cell.
             *
             * @param[in]   pingData the data for the item.
             * @param[in]   painter the QPainter to draw to.
             * @param[in]   option the painter options.
             * @param[in]   index the model index of the cell.
             */
            auto paintInvalidHop(
                Nedrysoft::RouteAnalyser::PingData *pingData,
                QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index
            ) const -> void;

            /**
             * @brief       Paints the bubble for hops.
             *
             * @param[in]   pingData the data for the item.
             * @param[in]   painter the QPainter to draw to.
             * @param[in]   option the painter options.
             * @param[in]   index the model index of the cell.
             * @param[in]   bubbleColour the colour to draw in.
             * @param[in]   bubbleWidth if non-zero, then this value is used as the width of the bubble.
             */
            auto paintBubble(
                Nedrysoft::RouteAnalyser::PingData *pingData,
                QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index,
                QRgb bubbleColour,
                int bubbleWidth=0
            ) const -> void;

            /**
             * @brief       Paints the hop column.
             *
             * @param[in]   pingData  the data for the item.
             * @param[in]   painter the QPainter to draw to.
             * @param[in]   option the painter options.
             * @param[in]   index the model index of the cell.
             */
            auto paintHop(Nedrysoft::RouteAnalyser::PingData *pingData,
                QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index
            ) const -> void;

            /**
             * @brief       Returns an interpolated colour.
             *
             * @details     Takes a list of <double, QColor> items and a value and returns the colour for that point.
             *
             * @param[in]   keyFrames the key/colour pair for interpolation.
             * @param[in]   value the point to get the colour for.
             *
             * @returns     the colour.
             */
            auto getInterpolatedColour(const QMap<double, QRgb> &keyFrames, double value) const -> QRgb;

            /**
             * @brief       Paints text in a cell.
             *
             * @param[in]   text the text to paint.
             * @param[in]   painter the QPainter to draw to.
             * @param[in]   option the painter options.
             * @param[in]   index the model index of the cell.
             * @param[in]   bold controls whether the text is drawn in bold.
             * @param[in]   alignment the text alignment flags.
             * @param[in]   flags controls how the text is drawn.
             */
            auto paintText(const QString &text,
                QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index,
                bool bold = false,
                int alignment = Qt::AlignLeft | Qt::AlignVCenter,
                int flags = 0
            ) const -> void;

            /**
             * @brief       Draws a latency line on the graph for the given data points in the given colour.
             *
             * @param[in]   field the data field to use for drawing.
             * @param[in]   pingData the data for the item.
             * @param[in]   painter  the QPainter to draw to.
             * @param[in]   option the painter options.
             * @param[in]   index the model index of the cell.
             * @param[in]   pen  the pen to use to draw the line.
             */
            auto drawLatencyLine(
                int field, Nedrysoft::RouteAnalyser::PingData *pingData,
                QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index,
                const QPen &pen
            ) const -> void;

        private:
            QMap<Nedrysoft::RouteAnalyser::PingData::Fields, QPersistentModelIndex *> m_maximumMap;

    };
}}

#endif // PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTETABLEITEMDELEGATE_H
