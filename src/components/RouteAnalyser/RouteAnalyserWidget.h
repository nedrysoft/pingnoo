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
        explicit RouteAnalyserWidget(QWidget *parent = nullptr);
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
    };
}

#endif // FIZZYADE_ROUTEANALYSER_ROUTEANALYSERWIDGET_H
