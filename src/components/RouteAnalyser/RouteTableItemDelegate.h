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

#ifndef NEDRYSOFT_ROUTEANALYSER_ROUTETABLEITEMDELEGATE_H
#define NEDRYSOFT_ROUTEANALYSER_ROUTETABLEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <chrono>
#include <cmath>

class QTableView;

namespace Nedrysoft::RouteAnalyser {
    class PingData;

    /**
     * @brief       Route table graph item delegate
     *
     * @details     Delegate used to draw the graph item in the
     *              route table
     *
     */
    class RouteTableItemDelegate :
            public QStyledItemDelegate {

        public:

            /**
             * @brief       Constructor
             *
             */
            RouteTableItemDelegate(QWidget *parent = 0);

            /**
             * @brief       paint implementation to draw the graph item
             *
             */
            void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

        private:

            /**
             * @brief       Finds a route item sibling
             *
             * @details     Finds the previous or next valid item in the route list, a valid
             *              item is a hop that responded to the ping, ignores items that did
             *              not respond.  Will return invalid model indexes if there is not a
             *              previous/next item that is valid.
             *
             * @param[in]   modelIndex          the starting model index
             * @param[in]   adjustment          set to 1 to look for the next valud, -1 for previous
             *
             * @return      The model index of the next/previous item (or invalid)
             *
             */
            QModelIndex getSibling(QModelIndex modelIndex, int adjustment) const;

            /**
             * @brief       Gets the route sibling data
             *
             * @details     calls getSibling, but returns the PingData associated with the sibling
             *              and the visual rectangle as well.
             *
             * @param[in]   modelIndex          the starting model index
             * @param[in]   adjustment          set to 1 to look for the next valud, -1 for previous
             * @param[in]   tableView           view associated with this
             * @param[out]  rect                the visual rectangle of the item
             *
             * @return The model index of the next/previous item (or invalid)
             *
             */
            Nedrysoft::RouteAnalyser::PingData *
            getSiblingData(QModelIndex modelIndex, int adjustment, const QTableView *tableView, QRect &rect) const;

            /**
             * @brief       Paints the graph column
             *
             * @param[in]   pingData            the data for the item
             * @param[in]   painter             the QPainter to draw to
             * @param[in]   option              the painter options
             * @param[in]   index               the model index of the cell
             *
             */
            void paintGraph(Nedrysoft::RouteAnalyser::PingData *pingData, QPainter *painter,
                            const QStyleOptionViewItem &option, const QModelIndex &index) const;

            /**
             * @brief       Paints the background of a cell
             *
             * @param[in]   pingData            the data for the item
             * @param[in]   painter             the QPainter to draw to
             * @param[in]   option              the painter options
             * @param[in]   index               the model index of the cell
             *
             */
            void paintBackground(Nedrysoft::RouteAnalyser::PingData *pingData, QPainter *painter,
                                 const QStyleOptionViewItem &option, const QModelIndex &index) const;

            /**
             * @brief       Paints the location column
             *
             * @param[in]   pingData            the data for the item
             * @param[in]   painter             the QPainter to draw to
             * @param[in]   option              the painter options
             * @param[in]   index               the model index of the cell
             *
             */
            void paintLocation(Nedrysoft::RouteAnalyser::PingData *pingData, QPainter *painter,
                               const QStyleOptionViewItem &option, const QModelIndex &index) const;

            /**
             * @brief       Paints an invalid hop cell
             *
             * @param[in]   pingData            the data for the item
             * @param[in]   painter             the QPainter to draw to
             * @param[in]   option              the painter options
             * @param[in]   index               the model index of the cell
             *
             */
            void paintInvalidHop(Nedrysoft::RouteAnalyser::PingData *pingData, QPainter *painter,
                                 const QStyleOptionViewItem &option, const QModelIndex &index) const;

            /**
             * @brief       Paints the bubble for hops
             *
             * @param[in]   pingData            the data for the item
             * @param[in]   painter             the QPainter to draw to
             * @param[in]   option              the painter options
             * @param[in]   index               the model index of the cell
             * @param[in]   bubbleColor         the colour to draw in
             *
             */
            void paintBubble(Nedrysoft::RouteAnalyser::PingData *pingData, QPainter *painter,
                             const QStyleOptionViewItem &option, const QModelIndex &index, QRgb bubbleColour) const;

            /**
             * @brief       Paints the hop column
             *
             * @param[in]   pingData            the data for the item
             * @param[in]   painter             the QPainter to draw to
             * @param[in]   option              the painter options
             * @param[in]   index               the model index of the cell
             *
             */
            void paintHop(Nedrysoft::RouteAnalyser::PingData *pingData, QPainter *painter,
                          const QStyleOptionViewItem &option, const QModelIndex &index) const;

            /**
             * @brief       getInterpolatedColour
             *
             * @details     Takes a list of <double, QColor> items and a value
             *              and returns the colour for that point
             *
             * @param[in]   keyFrames           the key/colour pair for interpolation
             * @param[in]   value               the point to get the colour for
             *
             * @return      the colour
             *
             */
            QRgb getInterpolatedColour(const QMap<double, QRgb> &keyFrames, double value) const;

            /**
             * @brief       Paints text in a cell
             *
             * @param[in]   text                the text to paint
             * @param[in]   painter             the QPainter to draw to
             * @param[in]   option              the painter options
             * @param[in]   index               the model index of the cell
             * @param[in]   alignment           the text alignment flags
             * @param[in]   flags               controls how the text is drawn
             *
             */
            void paintText(const QString &text, QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index, int alignment = Qt::AlignLeft | Qt::AlignVCenter,
                           int flags = 0) const;

            /**
             * @brief       Draws a latency line on the graph for the given data points in the given colour
             *
             * @param[in]   field               the data field to use for drawing
             * @param[in]   pingData            the data for the item
             * @param[in]   painter             the QPainter to draw to
             * @param[in]   option              the painter options
             * @param[in]   index               the model index of the cell
             * @param[in]   pen                 the pen to use to draw the line
             *
             */
            void drawLatencyLine(int field, Nedrysoft::RouteAnalyser::PingData *pingData, QPainter *painter,
                                 const QStyleOptionViewItem &option, const QModelIndex &index, const QPen &pen) const;

        private:
            std::chrono::duration<double> m_lowRangeLatency = {};            //! The lowest latency seen by this item
            std::chrono::duration<double> m_midRangeLatency = {};            //! The highest latencty seen by this item
    };
}

#endif // NEDRYSOFT_ROUTEANALYSER_ROUTETABLEITEMDELEGATE_H
