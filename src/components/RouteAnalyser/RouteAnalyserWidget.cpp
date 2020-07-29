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

#include "RouteAnalyserWidget.h"
#include "QCustomPlot/qcustomplot.h"
#include "ComponentSystem/IComponentManager.h"
#include "Core/PingResult.h"
#include "Core/IPingTarget.h"
#include "Core/IPingEngine.h"
#include "Core/IPingEngineFactory.h"
#include "Core/IRouteEngine.h"
#include "Core/IRouteEngineFactory.h"
#include "Core/IHostMasker.h"
#include "Core/IGeoIPProvider.h"
#include "Core/IContextManager.h"
#include "RouteTableItemDelegate.h"
#include "PingData.h"
#include "CPAxisTickerMS.h"
#include "GraphLatencyLayer.h"
#include <QDebug>
#include <QHostAddress>
#include <QHostInfo>
#include <QPersistentModelIndex>
#include <QTableWidgetItem>
#include <QSpacerItem>
#include <QDateTime>
#include <QPdfWriter>
#include <QRegularExpression>
#include <QLocale>
#include <QVariant>
#include <QVBoxLayout>
#include <QLabel>
#include <QSslSocket>
#include <QDnsLookup>

using namespace std::chrono_literals;

constexpr auto RoundTripGraph = 0;
constexpr auto TimeoutGraph = 1;
constexpr std::chrono::duration<double> DefaultMaxLatency = 0.01s;
constexpr std::chrono::duration<double> DefaultTimeWindow = 10min;
constexpr auto DefaultGraphHeight = 300;
constexpr auto TableRowHeight = 20;

QMap<int, QPair<QString, QString> > &FizzyAde::RouteAnalyser::RouteAnalyserWidget::headerMap()
{
    static QMap<int, QPair<QString, QString> > &map = *new QMap<int, QPair<QString, QString> >
    {
        {FizzyAde::RouteAnalyser::PingData::Hop, {tr("Hop"),"XXXXX"}},
        {FizzyAde::RouteAnalyser::PingData::Count, {tr("Count"),"XXXXX"}},
        {FizzyAde::RouteAnalyser::PingData::IP, {tr("IP"),"888.888.888.888"}},
        {FizzyAde::RouteAnalyser::PingData::HostName, {tr("Name"),"XXXXXXXXXXX.XXXXXXXXXX.XXXXXXXXX.XXX"}},
        {FizzyAde::RouteAnalyser::PingData::Location, {tr("Location"),"XXXXXXXXXXXXXXXX"}},
        {FizzyAde::RouteAnalyser::PingData::AverageLatency, {tr("Avg"),"8888.888"}},
        {FizzyAde::RouteAnalyser::PingData::CurrentLatency, {tr("Cur"),"8888.888"}},
        {FizzyAde::RouteAnalyser::PingData::MinimumLatency, {tr("Min"),"8888.888"}},
        {FizzyAde::RouteAnalyser::PingData::MaximumLatency, {tr("Max"),"8888.888"}},
        {FizzyAde::RouteAnalyser::PingData::PacketLoss, {tr("Loss %"),"8888.888"}},
        {FizzyAde::RouteAnalyser::PingData::Graph, {"",""}}
    };

    return map;
}

FizzyAde::RouteAnalyser::RouteAnalyserWidget::RouteAnalyserWidget::RouteAnalyserWidget(QString targetHost, FizzyAde::Core::IPVersion ipVersion, double interval, FizzyAde::Core::IPingEngineFactory *pingEngineFactory, QWidget *parent) : QWidget(parent)
{
    auto maskerConfig = QString(R"|({"id":"FizzyAde::RegExHostMasker::RegExHostMasker","matchItems":[{"matchExpression":"([0-9]{1,3})\\.([0-9]{1,3})-([0-9]{1,3})-([0-9]{1,3})\\.(?<domain>static.virginmediabusiness\\.co\\.uk)","matchFlags":20,"matchHopString":"","matchReplacementString":"<hidden>.[domain]"},{"matchExpression":"([0-9]{1,3})\\.([0-9]{1,3})-([0-9]{1,3})-([0-9]{1,3})\\.(?<domain>static.virginmediabusiness\\.co\\.uk)","matchFlags":12,"matchHopString":"","matchReplacementString":"<hidden>"},{"matchExpression":"(?<host>(.+))\\.fizzyade\\.(?<domain>(.+))","matchFlags":20,"matchHopString":"","matchReplacementString":"[host].<hidden>.[domain]"},{"matchExpression":"^(?<host>tunnel[0-9]*)\.(?<domain>tunnel.tserv[0-9]*.lon[0-9]*.ipv6.he.net)$","matchFlags":20,"matchHopString":"","matchReplacementString":"<hidden>.[domain]"},{"matchExpression":"^(?<host>tunnel[0-9]*)\.(?<domain>tunnel.tserv[0-9]*.lon[0-9]*.ipv6.he.net)$","matchFlags":12,"matchHopString":"","matchReplacementString":"<hidden>"}]})|");

    auto hostMaskers = FizzyAde::ComponentSystem::getObjects<FizzyAde::Core::IHostMasker>();

    for (auto hostMasker : hostMaskers) {
        auto doc = QJsonDocument::fromJson(maskerConfig.toLatin1());

        hostMasker->loadConfiguration(doc.object());
    }

    auto routeEngineFactory = FizzyAde::ComponentSystem::getObject<FizzyAde::Core::IRouteEngineFactory>();

    if (!routeEngineFactory) {
        return;
    }

    auto routeEngine = routeEngineFactory->createEngine();

    if (routeEngine) {
        connect(routeEngine, &FizzyAde::Core::IRouteEngine::result, this, &FizzyAde::RouteAnalyser::RouteAnalyserWidget::onRouteResult);

        routeEngine->findRoute(targetHost, ipVersion);
    }

    auto routeGraphDelegate = new FizzyAde::RouteAnalyser::RouteTableItemDelegate;

    connect(this, &QObject::destroyed, routeGraphDelegate, [routeGraphDelegate] (QObject *) {
        delete routeGraphDelegate;
    });

    m_pingEngineFactory = pingEngineFactory;
    m_interval = interval;

    m_scrollArea = new QScrollArea();

    m_splitter = new QSplitter(Qt::Vertical);

    m_scrollArea->setWidgetResizable(true);

    m_scrollArea->setWidget(new QWidget());
    m_scrollArea->widget()->setBackgroundRole(QPalette::Base);

    m_tableModel = new QStandardItemModel();

    m_tableModel->setColumnCount(headerMap().count());

    m_tableView = new QTableView();

    m_tableView->setModel(m_tableModel);
    m_tableView->setItemDelegate(routeGraphDelegate);
    m_tableView->setShowGrid(false);
    m_tableView->verticalHeader()->setVisible(false);

    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_tableView->horizontalHeader()->setMinimumSectionSize(-1);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->horizontalHeader()->setStretchLastSection(true);

    auto headerIterator = QMapIterator<int, QPair<QString, QString> >(headerMap());

    while(headerIterator.hasNext()) {
        headerIterator.next();

        auto headerItem = new QStandardItem();

        auto pair = headerIterator.value();

        headerItem->setText(pair.first);

        auto maxWidth = qMax(m_tableView->fontMetrics().horizontalAdvance(pair.first), m_tableView->fontMetrics().horizontalAdvance(pair.second));

        m_tableModel->setHorizontalHeaderItem(headerIterator.key(), headerItem);

        m_tableView->horizontalHeader()->resizeSection(headerIterator.key(), maxWidth);
    }

    m_splitter->addWidget(m_tableView);
    m_splitter->addWidget(m_scrollArea);

    m_splitter->setStretchFactor(1, 2);

    auto verticalLayout = new QVBoxLayout();

    verticalLayout->setMargin(0);

    verticalLayout->addWidget(m_splitter);

    this->setLayout(verticalLayout);
}

FizzyAde::RouteAnalyser::RouteAnalyserWidget::~RouteAnalyserWidget()
{
    delete m_tableView;
    delete m_tableModel;
}

void FizzyAde::RouteAnalyser::RouteAnalyserWidget::onPingResult(FizzyAde::Core::PingResult result)
{
    auto pingData = static_cast<FizzyAde::RouteAnalyser::PingData *>(result.target()->userData());
    QCustomPlot *customPlot;

    if (!pingData) {
        return;
    }

    customPlot = pingData->customPlot();

    if (!customPlot) {
        return;
    }

    switch(result.code()) {
        case FizzyAde::Core::PingResult::Ok:
        case FizzyAde::Core::PingResult::TimeExceeded: {
            QCPRange graphRange = customPlot->yAxis->range();
            auto requestTime = std::chrono::duration<double>(result.requestTime().time_since_epoch());

            customPlot->graph(RoundTripGraph)->addData(requestTime.count(), result.roundTripTime().count());
            customPlot->graph(TimeoutGraph)->addData(requestTime.count(), 0);

            pingData->updateItem(result);

            // individual vertical range

            if (result.roundTripTime().count()>graphRange.upper)
                customPlot->yAxis->setRange(0, result.roundTripTime().count());

            // normalised vertical range

            /*
            auto graphMaxLatency = m_tableView->property("graphMaxLatency").toDouble();

            if (graphMaxLatency>graphRange.upper) {
                for (QCustomPlot *customPlot : m_plotList) {
                    customPlot->yAxis->setRange(0, graphMaxLatency);
                    customPlot->replot();
                }
            }*/

            m_tableView->viewport()->update();

            break;
        }

        case FizzyAde::Core::PingResult::NoReply: {
            auto requestTime = std::chrono::duration<double>(result.requestTime().time_since_epoch());

            customPlot->graph(RoundTripGraph)->addData(requestTime.count(), 0);
            customPlot->graph(TimeoutGraph)->addData(requestTime.count(), 1);

            pingData->updateItem(result);

            break;
        }
    }

    customPlot->replot();
}

void FizzyAde::RouteAnalyser::RouteAnalyserWidget::onRouteResult(const QHostAddress &routeHostAddress, const FizzyAde::Core::RouteList &route)
{
    FizzyAde::Core::IRouteEngine *routeEngine = qobject_cast<FizzyAde::Core::IRouteEngine *>(this->sender());
    auto hop = 1;
    auto geoIP = FizzyAde::ComponentSystem::getObject<FizzyAde::Core::IGeoIPProvider>();

    if (routeEngine) {
        disconnect(routeEngine, &FizzyAde::Core::IRouteEngine::result, this, &FizzyAde::RouteAnalyser::RouteAnalyserWidget::onRouteResult);
    }

    if (!m_pingEngineFactory) {
        return;
    }

    if (routeHostAddress.protocol()==QAbstractSocket::IPv4Protocol) {
        m_pingEngine = m_pingEngineFactory->createEngine(FizzyAde::Core::V4);
    } else if (routeHostAddress.protocol()==QAbstractSocket::IPv6Protocol) {
        m_pingEngine = m_pingEngineFactory->createEngine(FizzyAde::Core::V6);
    }  else {
        return;
    }

    auto interval = std::chrono::duration<double, std::ratio<1,1> >(m_interval);

    m_pingEngine->setInterval(std::chrono::duration_cast<std::chrono::milliseconds>(interval));

    connect(m_pingEngine, &FizzyAde::Core::IPingEngine::result, this, &FizzyAde::RouteAnalyser::RouteAnalyserWidget::onPingResult);

    auto verticalLayout = new QVBoxLayout();

    for (const QHostAddress &host : route) {
        if (!host.isNull()) {
            auto hostAddress = host.toString();
            auto hostName = QHostInfo::fromName(host.toString()).hostName();

            auto maskedHostName = hostName;
            auto maskedHostAddress = hostAddress;

            for(auto masker : FizzyAde::ComponentSystem::getObjects<FizzyAde::Core::IHostMasker>()) {
                masker->mask(hop, hostName, hostAddress, maskedHostName, maskedHostAddress);
            }

            auto customPlot = new QCustomPlot();

            customPlot->addLayer("newBackground", customPlot->layer("grid"), QCustomPlot::limBelow);

            new GraphLatencyLayer(customPlot);

            customPlot->setCurrentLayer("main");

            customPlot->setMinimumHeight(DefaultGraphHeight);

            customPlot->setInteractions(QCP::iRangeDrag);
            customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

            customPlot->addGraph();
            customPlot->addGraph();

            customPlot->yAxis->ticker()->setTickCount(1);

            QSharedPointer<FizzyAde::RouteAnalyser::CPAxisTickerMS> msTicker(new FizzyAde::RouteAnalyser::CPAxisTickerMS);

            customPlot->yAxis->setTicker(msTicker);
            customPlot->yAxis->setLabel(tr("Latency (ms)"));
            customPlot->yAxis->setRange(0, DefaultMaxLatency.count());

            QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);

            auto locale = QLocale::system();

            dateTicker->setDateTimeFormat(locale.timeFormat(QLocale::LongFormat).remove("t").trimmed()+"\n"+locale.dateFormat(QLocale::ShortFormat));

            customPlot->xAxis->setTicker(dateTicker);
            customPlot->xAxis->setRange(QDateTime::currentDateTime().toSecsSinceEpoch(), QDateTime::currentDateTime().toSecsSinceEpoch()+DefaultTimeWindow.count());

            customPlot->graph(RoundTripGraph)->setLineStyle(QCPGraph::lsStepCenter);
            customPlot->graph(TimeoutGraph)->setBrush(QBrush(Qt::red));

            customPlot->setBackground(this->palette().brush(QPalette::Base));
            customPlot->xAxis->setLabelColor(this->palette().color(QPalette::Text));
            customPlot->yAxis->setLabelColor(this->palette().color(QPalette::Text));
            customPlot->xAxis->setTickLabelColor(this->palette().color(QPalette::Text));
            customPlot->yAxis->setTickLabelColor(this->palette().color(QPalette::Text));

            auto graphTitle = new QCPTextElement(customPlot, QString(tr("Hop %1")).arg(hop)+" "+maskedHostName+" ("+maskedHostAddress+")");

            graphTitle->setTextColor(this->palette().color(QPalette::Text));

            customPlot->plotLayout()->insertRow(0);
            customPlot->plotLayout()->addElement(0, 0, graphTitle);

            customPlot->replot();

            /**
             * scroll wheel events, by default QCustomPlot does not propogate these so this code ensures that they cause
             * the scrollarea to scroll.
             */

            connect(customPlot, &QCustomPlot::mouseWheel, [this] (QWheelEvent *event) {
                m_scrollArea->verticalScrollBar()->setValue(m_scrollArea->verticalScrollBar()->value() - event->angleDelta().y());
            });

            /**
             *  mouse over event
             */

            auto graphLine = new QCPItemStraightLine(customPlot);

            graphLine->setPen(QPen(Qt::darkGray, 2, Qt::DotLine));

            m_graphLines[customPlot] = graphLine;

            connect(customPlot, &QCustomPlot::mouseMove, [this, customPlot, graphLine, maskedHostName] (QMouseEvent *event) {
                auto x = customPlot->xAxis->pixelToCoord(event->pos().x());
                auto foundRange = false;
                auto dataRange = customPlot->graph(RoundTripGraph)->data()->keyRange(foundRange);

                graphLine->point1->setCoords(x, 0);
                graphLine->point2->setCoords(x, 1);

                customPlot->replot();

                if ( (foundRange) &&
                     (x>=dataRange.lower) &&
                     (x<=dataRange.upper) ) {
                    auto valueString = QString();
                    auto valueResultRange = customPlot->graph(RoundTripGraph)->data()->valueRange(foundRange, QCP::sdBoth, QCPRange(x-1, x+1));

                    for (auto currentItem = 0;currentItem<m_tableModel->rowCount();currentItem++) {
                        auto pingData = m_tableModel->item(currentItem, 0)->data().value<FizzyAde::RouteAnalyser::PingData *>();
                        auto valueRange = QCPRange(x-1, x+1);

                        if (pingData->customPlot()) {
                            auto tempResultRange = pingData->customPlot()->graph(RoundTripGraph)->data()->valueRange(foundRange, QCP::sdBoth, valueRange);

                            auto seconds = std::chrono::duration<double>(tempResultRange.upper);

                            pingData->setHistoricalLatency(seconds);
                        } else {
                            pingData->setHistoricalLatency(std::chrono::duration<double>(-1));

                            auto topLeft = m_tableModel->index(0, 0);
                            auto bottomRight = topLeft.sibling(m_tableModel->rowCount()-1, m_tableModel->columnCount()-1);

                            m_tableModel->dataChanged(topLeft, bottomRight);
                        }
                    }

                    this->m_tableModel->setProperty("showHistorical", true);

                    auto seconds = std::chrono::duration<double>(valueResultRange.upper);

                    if (seconds<std::chrono::seconds(1)) {
                        auto milliseconds = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(seconds);

                        valueString = QString(tr("%1ms")).arg(milliseconds.count(), 0, 'f', 2);
                    } else {
                        valueString = QString(tr("%1s")).arg(seconds.count(), 0, 'f', 2);
                    }

                    /*
                    auto dateTime = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(x));

                    m_pointInfoLabel->setText(FontAwesome::richText(QString("[fas fa-stopwatch] %1").arg(valueString)));
                    m_hopInfoLabel->setText(FontAwesome::richText(QString("[fas fa-project-diagram] %1 %2").arg(tr("hop")).arg(hop)));
                    m_hostInfoLabel->setText(FontAwesome::richText(QString("[fas fa-server] %1").arg(maskedHostName)));
                    m_timeInfoLabel->setText(FontAwesome::richText(QString("[far fa-calendar-alt] %1").arg(dateTime.toString())));
                    */
                } else {
                    /*
                    m_pointInfoLabel->setText("");
                    m_hopInfoLabel->setText("");
                    m_hostInfoLabel->setText("");
                    m_timeInfoLabel->setText("");
                    */

                    this->m_tableModel->setProperty("showHistorical", false);

                    auto topLeft = m_tableModel->index(0, 0);
                    auto bottomRight = topLeft.sibling(m_tableModel->rowCount()-1, m_tableModel->columnCount()-1);

                    m_tableModel->dataChanged(topLeft, bottomRight);
                }
            });

            customPlot->installEventFilter(this);

            m_plotList.append(customPlot);

            verticalLayout->addWidget(customPlot);

            auto pingTarget = m_pingEngine->addTarget(routeHostAddress, hop);

            auto pingData = new FizzyAde::RouteAnalyser::PingData(m_tableModel, hop, true);

            auto tableItem = new QStandardItem(1, m_tableModel->columnCount());

            tableItem->setData(QVariant::fromValue<FizzyAde::RouteAnalyser::PingData *>(pingData));

            m_tableModel->appendRow(tableItem);

            m_tableView->setRowHeight(tableItem->index().row(), TableRowHeight);

            connect(m_tableView, &QObject::destroyed, [pingData] (QObject *) {
                delete pingData;
            });

            pingData->setCustomPlot(customPlot);
            pingData->setHostAddress(maskedHostAddress);
            pingData->setHostName(maskedHostName);

            pingTarget->setUserData(pingData);

            if (geoIP) {
                geoIP->lookup(hostAddress, [pingData] (const QString &, const QVariantMap &result) mutable {
                    pingData->setLocation(result["country"].toString());
                });
            }
        } else {
            auto pingData = new FizzyAde::RouteAnalyser::PingData(m_tableModel, hop, false);

            connect(m_tableView, &QObject::destroyed, [pingData] (QObject *) {
                delete pingData;
            });

            pingData->setHostAddress("*");
            pingData->setHostName("*");

            auto tableItem = new QStandardItem(1, headerMap().count());
            auto itemData = QVariant();

            itemData.setValue(pingData);

            tableItem->setData(itemData);

            m_tableModel->appendRow(tableItem);

            m_tableView->setRowHeight(tableItem->index().row(), TableRowHeight);
        }

        hop++;
    }

    connect(this, &FizzyAde::RouteAnalyser::RouteAnalyserWidget::filteredEvent, [=] (QObject *watched, QEvent *event) {
        auto customPlot = qobject_cast<QCustomPlot *>(watched);

        auto line =  m_graphLines[customPlot];

        if (event->type()==QEvent::PaletteChange) {
            customPlot->setBackground(this->palette().brush(QPalette::Base));

            customPlot->xAxis->setLabelColor(this->palette().color(QPalette::Text));
            customPlot->yAxis->setLabelColor(this->palette().color(QPalette::Text));
            customPlot->xAxis->setTickLabelColor(this->palette().color(QPalette::Text));
            customPlot->yAxis->setTickLabelColor(this->palette().color(QPalette::Text));

            QCPTextElement *textElement = qobject_cast<QCPTextElement *>(customPlot->plotLayout()->element(0,0));

            if (textElement) {
                textElement->setTextColor(this->palette().color(QPalette::Text));
            }
        }

        if ( (event->type()==QEvent::Enter) ||
             (event->type()==QEvent::Leave) ) {

            /*m_pointInfoLabel->setText("");
            m_hopInfoLabel->setText("");
            m_hostInfoLabel->setText("");
            m_timeInfoLabel->setText("");*/

            line->setVisible(event->type()==QEvent::Enter);

            customPlot->replot();

            this->m_tableModel->setProperty("showHistorical", false);

            auto topLeft = m_tableModel->index(0, 0);
            auto bottomRight = topLeft.sibling(m_tableModel->rowCount()-1, m_tableModel->columnCount()-1);

            m_tableModel->dataChanged(topLeft, bottomRight);
        }
    });

    m_scrollArea->widget()->setLayout(verticalLayout);

    for(auto sourcePlot : m_plotList) {
        connect(sourcePlot->xAxis, qOverload<const QCPRange &>(&QCPAxis::rangeChanged), [this, sourcePlot] (const QCPRange &range) {
            auto newRange = range;
            auto epoch = std::chrono::duration<double>(m_pingEngine->epoch().time_since_epoch());

            if (newRange.lower<epoch.count()) {
                newRange = QCPRange(epoch.count(), epoch.count()+DefaultTimeWindow.count());

                sourcePlot->xAxis->setRange(newRange);
            }

            for (auto targetPlot : m_plotList) {
                if (sourcePlot==targetPlot) {
                    continue;
                }

                targetPlot->xAxis->setRange(newRange);

                if (!targetPlot->visibleRegion().isNull()) {
                    targetPlot->replot();
                }
            }
        });
    }
}

bool FizzyAde::RouteAnalyser::RouteAnalyserWidget::eventFilter(QObject *watched, QEvent *event)
{
    emit filteredEvent(watched, event);

    return QWidget::eventFilter(watched, event);
}
