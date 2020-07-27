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

#ifndef FIZZYADE_ROUTEANALYSER_ROUTEANALYSERWIDGET_H
#define FIZZYADE_ROUTEANALYSER_ROUTEANALYSERWIDGET_H

#include "Core/PingResult.h"
#include "Core/IRouteEngine.h"
#include "QCustomPlot/qcustomplot.h"
#include <QWidget>
#include <QMap>
#include <QPair>

namespace FizzyAde::Core
{
    class IHostMasker;
    class IPingEngine;
    class IPingEngineFactory;
}

class QTableView;
class QStandardItemModel;
class QSplitter;
class QScrollArea;

namespace FizzyAde::RouteAnalyser
{
    class RouteAnalyserWidget :
        public QWidget
    {
        Q_OBJECT
    public:
        explicit RouteAnalyserWidget(QString targetHost, FizzyAde::Core::IPVersion ipVersion, double interval, FizzyAde::Core::IPingEngineFactory *pingEngineFactory, QWidget *parent = nullptr);
        ~RouteAnalyserWidget();

    public slots:
        void onPingResult(FizzyAde::Core::PingResult result);
        void onRouteResult(const QHostAddress &routeHostAddress, const FizzyAde::Core::RouteList &route);

    signals:
        void filteredEvent(QObject *watched, QEvent *event);

    protected:
        QMap<int, QPair<QString, QString> > &headerMap();

        virtual bool eventFilter(QObject *watched, QEvent *event);

    private:
        QMap<FizzyAde::Core::IPingTarget *, int> m_targetMap;
        QList<QCustomPlot *> m_plotList;
        QMap<QCustomPlot *, QCPItemStraightLine *> m_graphLines;
        FizzyAde::Core::IPingEngine *m_pingEngine = {};
        QStandardItemModel *m_tableModel;
        QTableView *m_tableView;
        QSplitter *m_splitter;
        QScrollArea *m_scrollArea;
        FizzyAde::Core::IPingEngineFactory *m_pingEngineFactory;
        double m_interval;
    };
}

#endif // FIZZYADE_ROUTEANALYSER_ROUTEANALYSERWIDGET_H
