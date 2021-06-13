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

#ifndef PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTEANALYSERWIDGET_H
#define PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTEANALYSERWIDGET_H

#pragma warning(push)
#pragma warning(disable : 4996)

#include "IRouteEngine.h"
#include "PingData.h"
#include "PingResult.h"
#include "QCustomPlot/qcustomplot.h"

#include <QMap>
#include <QPair>
#include <QWidget>

#pragma warning(pop)

namespace Nedrysoft { namespace Core {
    class IHostMasker;
}}

class QTableView;
class QStandardItemModel;
class QSplitter;
class QScrollArea;
class Timer;

namespace Nedrysoft { namespace RouteAnalyser {
    class GraphLatencyLayer;
    class IPingEngine;
    class IPingEngineFactory;
    class PlotScrollArea;
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

        private:
            enum class ScaleMode {
                None,
                Normalised,
                Fixed
            };

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
            explicit RouteAnalyserWidget(
                QString targetHost,
                Nedrysoft::Core::IPVersion ipVersion,
                int interval,
                Nedrysoft::RouteAnalyser::IPingEngineFactory *pingEngineFactory,
                QWidget *parent = nullptr
            );

            /**
             * @brief       Destroys the RouteAnalyserWidget.
             */
            ~RouteAnalyserWidget();

            /**
             * @brief       Called when a ping result is available.
             *
             * @param[in]   result the PingResult contains the timing information for the ping.
             */
            Q_SLOT void onPingResult(Nedrysoft::RouteAnalyser::PingResult result);

            /**
             * @brief       Called when a ping route is available.
             *
             * @details     returns the list of hops to the destination.
             *
             * @param[in]   routeHostAddress the intended target of the route analysis.
             * @param[in]   route the route that was discovered.
             */
            Q_SLOT void onRouteResult(const QHostAddress routeHostAddress,
                                      const Nedrysoft::RouteAnalyser::RouteList route );

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
             * @brief       This signal is emitted when the dataset size changes.
             * @param[in]   start the star time point. (seconds from unix epoch)
             * @param[in]  end the end time point. (seconds from unix epoch)
             */
            Q_SIGNAL void datasetChanged(double start, double end);

            /**
             * @brief       Sets whether this instance draws with solid or gradient backgrounds on graphs.
             *
             * @param[in]   smoothGradient true if gradient; otherwise false.
             */
            auto setGradientEnabled(bool smoothGradient) -> void;

            /**
             * @brief       Sets the viewport size in milliseconds.
             *
             * @param[in]   viewportSize the viewport size.
             */
            auto setViewportSize(double viewportSize) -> void;

            /**
             * @brief       Sets the position of the viewport (0=start, 1=end)
             *
             * @param[in]   position the position of the viewport.
             */
            auto setViewportPosition(double position) -> void;

            /**
             * @brief       Gets the current position of the viewport
             *
             * @returns     the position.
             */
            auto viewportPosition() -> double;

            /**
             * @brief       Returns the current viewport size.
             *
             * @returns     the viewport size in milliseconds.
             */
            auto viewportSize(void) -> double;

            /**
             * @brief       Returns the size in seconds of the captured data.
             *
             * @returns     the size in seconds.
             */
            auto datasetSize(void) -> double;

        protected:
            /**
             * @brief       Reimplements: QObject::eventFilter(QObject *watched, QEvent *event).
             *
             * @details     Used to provide a signal which allows the processing of events.
             *
             * @param[in]   watched the object that caused the event.
             * @param[in]   event the event information.
             *
             * @returns     true if event was handled; otherwise false.
             */
            auto eventFilter(QObject *watched, QEvent *event) -> bool override;

            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @param[in]   paintEvent the event information.
             */
            auto paintEvent(QPaintEvent *paintEvent) -> void override;

            /**
             * @brief       Updates the ranges on the plots to match the viewport.
             */
            auto updateRanges() -> void;

            /**
             * @brief       A map containing the fields that are displayed on the list.
             *
             * @returns     the header map.
             */
            QMap<Nedrysoft::RouteAnalyser::PingData::Fields, QPair<QString, QString> > &headerMap();

        private:
            //! @cond

            QMap<Nedrysoft::RouteAnalyser::IPingTarget *, int> m_targetMap;
            QList<QCustomPlot *> m_plotList;
            QMap<QCustomPlot *, QCPItemStraightLine *> m_graphLines;
            QMap<QCustomPlot *, QCPBars *> m_barCharts;
            Nedrysoft::RouteAnalyser::IPingEngine *m_pingEngine = {};
            QStandardItemModel *m_tableModel;
            QTableView *m_tableView;
            QSplitter *m_splitter;
            PlotScrollArea *m_scrollArea;
            Nedrysoft::RouteAnalyser::IPingEngineFactory *m_pingEngineFactory;
            int m_interval;
            QList<Nedrysoft::RouteAnalyser::GraphLatencyLayer *> m_backgroundLayers;
            Nedrysoft::RouteAnalyser::RouteTableItemDelegate *m_routeGraphDelegate;
            ScaleMode m_graphScaleMode;
            QTimer *m_layerCleanupTimer;

            QList<Nedrysoft::RouteAnalyser::IPlot *> m_extraPlots;

            double m_viewportSize;
            double m_viewportPosition;
            double m_startPoint;
            double m_endPoint;
            double m_savedDiff;

            //! @endcond
    };
}}

#endif // PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTEANALYSERWIDGET_H
