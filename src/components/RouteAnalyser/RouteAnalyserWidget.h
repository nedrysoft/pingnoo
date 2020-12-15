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

#ifndef NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSERWIDGET_H
#define NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSERWIDGET_H

#include "Core/PingResult.h"
#include "Core/IRouteEngine.h"
#include "QCustomPlot/qcustomplot.h"
#include <QWidget>
#include <QMap>
#include <QPair>

namespace Nedrysoft::Core {
    class IHostMasker;
    class IPingEngine;
    class IPingEngineFactory;
}

class QTableView;
class QStandardItemModel;
class QSplitter;
class QScrollArea;

namespace Nedrysoft::RouteAnalyser {
    class GraphLatencyLayer;
    class RouteTableItemDelegate;

    /**
     * @brief       The RouteAnalyserWidget class provides the main widget for a route analyser.
     *
     * @details     the widget which is displayed in an editor for a route analysis, includes the table view
     *              showing the current hop information and graphs for all hops which respond.
     */
    class RouteAnalyserWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new RouteAnalyserWidget with the given information.
             *
             * @param[in]   targetHost the host being analysed. (ip address or hostname)
             * @param[in]   ipVersion the version of ip to be used.
             * @param[in]   interval the interval between pings.
             * @param[in]   pingEngineFactory the ping engine factory to use.
             * @param[in]   parent the parent widget.
             */
            explicit RouteAnalyserWidget(QString targetHost, Nedrysoft::Core::IPVersion ipVersion, double interval,
                                         Nedrysoft::Core::IPingEngineFactory *pingEngineFactory,
                                         QWidget *parent = nullptr);

            /**
             * @brief       Destroys the RouteAnalyserWidget.
             */
            ~RouteAnalyserWidget();

            /**
             * @brief       Called when a ping result is available.
             *
             * @param[in]   result the PingResult contains the timing information for the ping.
             */
            Q_SLOT void onPingResult(Nedrysoft::Core::PingResult result);

            /**
             * @brief       Called when a ping route is available.
             *
             * @details     returns the list of hops to the destination.
             *
             * @param[in]   routeHostAddress the intended target of the route analysis.
             * @param[in]   route the route that was discovered.
             */
            Q_SLOT void onRouteResult(const QHostAddress &routeHostAddress, const Nedrysoft::Core::RouteList &route);

            /**
             * @brief       This signal is emitted when a watched event on a child fires.
             *
             * @details     used to get notifications of a palette change so that the graphs can be
             *              modified so that they remain legible.  Implemented to allow cross thread control
             *              changes.
             *
             * @param[in]   watched the object that was being watched.
             * @param[in]   event the event that was sent to the filter.
             */
            Q_SIGNAL void filteredEvent(QObject *watched, QEvent *event);

            /**
             * @brief       Sets whether this instance draws with solid or gradient backgrounds on graphs.
             *
             * @param[in]   useSmoothGradient true if gradient; otherwise false.
             */
            void setGradientEnabled(bool useSmoothGradient);

        protected:
            /**
             * @brief       Reimplements: QObject::eventFilter(QObject *watched, QEvent *event).
             *
             * @param[in]   watched the object that caused the event.
             * @param[in]   event the event information.
             *
             * @returns     true if event was handled; otherwise false.
             */
            bool eventFilter(QObject *watched, QEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @param[in]   event the event information.
             */
            void paintEvent(QPaintEvent *paintEvent) override;

            QMap<int, QPair<QString, QString> > &headerMap();

        private:
            QMap<Nedrysoft::Core::IPingTarget *, int> m_targetMap;
            QList<QCustomPlot *> m_plotList;
            QMap<QCustomPlot *, QCPItemStraightLine *> m_graphLines;
            Nedrysoft::Core::IPingEngine *m_pingEngine = {};
            QStandardItemModel *m_tableModel;
            QTableView *m_tableView;
            QSplitter *m_splitter;
            QScrollArea *m_scrollArea;
            Nedrysoft::Core::IPingEngineFactory *m_pingEngineFactory;
            double m_interval;
            QList<Nedrysoft::RouteAnalyser::GraphLatencyLayer *> m_backgroundLayers;
            Nedrysoft::RouteAnalyser::RouteTableItemDelegate *m_routeGraphDelegate;
    };
}

#endif // NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSERWIDGET_H
