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

#include "RouteAnalyserWidget.h"

#include "BarChart.h"
#include "CPAxisTickerMS.h"
#include "Core/IGeoIPProvider.h"
#include "Core/IHostMasker.h"
#include "Core/IPingEngine.h"
#include "Core/IPingEngineFactory.h"
#include "Core/IPingTarget.h"
#include "Core/IRouteEngineFactory.h"
#include "LatencySettings.h"
#include "GraphLatencyLayer.h"
#include "PlotScrollArea.h"
#include "RouteTableItemDelegate.h"
#include "SDK/IPlotFactory.h"
#include "SDK/IPlot.h"

#include <QDateTime>
#include <QHostAddress>
#include <QHostInfo>
#include <QTimer>
#include <cassert>
#include <chrono>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

constexpr auto RoundTripGraph = 0;
constexpr std::chrono::duration<double> DefaultMaxLatency = 0.01s;
constexpr auto DefaultTimeWindow = 60.0*10;
constexpr auto DefaultGraphHeight = 300;
constexpr auto TableRowHeight = 20;
constexpr auto NoReplyColour = qRgb(255,0,0);
constexpr auto PlotMargins = QMargins(80, 20, 40, 40);

QMap< Nedrysoft::RouteAnalyser::PingData::Fields, QPair<QString, QString> > &Nedrysoft::RouteAnalyser::RouteAnalyserWidget::headerMap() {
    static QMap<Nedrysoft::RouteAnalyser::PingData::Fields, QPair<QString, QString> > map = QMap<Nedrysoft::RouteAnalyser::PingData::Fields, QPair<QString, QString> >
            {
                    {PingData::Fields::Hop,            {tr("Hop"),      "XXXXX"}},
                    {PingData::Fields::Count,          {tr("Count"),    "XXXXX"}},
                    {PingData::Fields::IP,             {tr("IP"),       "888.888.888.888"}},
                    {PingData::Fields::HostName,       {tr("Name"),     "XXXXXXXXXXX.XXXXXXXXXX.XXXXXXXXX.XXX"}},
                    {PingData::Fields::Location,       {tr("Location"), "XXXXXXXXXXXXXXXX"}},
                    {PingData::Fields::AverageLatency, {tr("Avg"),      "8888.888"}},
                    {PingData::Fields::CurrentLatency, {tr("Cur"),      "8888.888"}},
                    {PingData::Fields::MinimumLatency, {tr("Min"),      "8888.888"}},
                    {PingData::Fields::MaximumLatency, {tr("Max"),      "8888.888"}},
                    {PingData::Fields::PacketLoss,     {tr("Loss %"),   "8888.888"}},
                    {PingData::Fields::Graph,          {"",             ""}}
            };

    return map;
}

Nedrysoft::RouteAnalyser::RouteAnalyserWidget::RouteAnalyserWidget::RouteAnalyserWidget(
        QString targetHost,
        Nedrysoft::Core::IPVersion ipVersion,
        double interval,
        Nedrysoft::Core::IPingEngineFactory *pingEngineFactory,
        QWidget *parent) :

        QWidget(parent),
        m_routeGraphDelegate(nullptr),
        m_graphScaleMode(ScaleMode::None),
        m_viewportSize(DefaultTimeWindow),
        m_viewportPosition(1),
        m_startPoint(-1),
        m_endPoint(0) {

    auto latencySettings = Nedrysoft::RouteAnalyser::LatencySettings::getInstance();

    assert(latencySettings!=nullptr);

    auto routeEngines = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IRouteEngineFactory>();

    if (routeEngines.empty()) {
        return;
    }

    QMultiMap<double, Nedrysoft::Core::IRouteEngineFactory *> sortedRouteEngines;

    for(auto routeEngine : routeEngines) {
        sortedRouteEngines.insert(1-routeEngine->priority(), routeEngine);
    }

    auto routeEngine = sortedRouteEngines.first()->createEngine();

    if (routeEngine) {
        connect(
                routeEngine,
                &Nedrysoft::Core::IRouteEngine::result,
                this,
                &RouteAnalyserWidget::onRouteResult);

        routeEngine->findRoute(pingEngineFactory, targetHost, ipVersion);
    }

    m_routeGraphDelegate = new RouteTableItemDelegate;

    connect(latencySettings, &Nedrysoft::RouteAnalyser::LatencySettings::gradientChanged, [=](bool useGradient) {
        m_routeGraphDelegate->setGradientEnabled(useGradient);
    });

    m_routeGraphDelegate->setGradientEnabled(latencySettings->gradientFill());

    connect(this, &QObject::destroyed, m_routeGraphDelegate, [this](QObject *) {
        delete m_routeGraphDelegate;
    });

    m_pingEngineFactory = pingEngineFactory;
    m_interval = interval;

    m_scrollArea = new PlotScrollArea();

    m_splitter = new QSplitter(Qt::Vertical);

    m_scrollArea->setWidgetResizable(true);

    m_scrollArea->setWidget(new QWidget());
    m_scrollArea->widget()->setBackgroundRole(QPalette::Base);

    connect(m_scrollArea, &PlotScrollArea::didScroll, [=](void) {
        for (auto plot : m_plotList) {
            if (plot->isVisible()) {
                if (!plot->visibleRegion().isEmpty()) {
                    plot->replot();
                }
            }
        }
    });

    m_tableModel = new QStandardItemModel();

    m_tableModel->setColumnCount(headerMap().count());

    m_tableView = new QTableView();

    m_tableView->setModel(m_tableModel);
    m_tableView->setItemDelegate(m_routeGraphDelegate);
    m_tableView->setShowGrid(false);
    m_tableView->verticalHeader()->setVisible(false);

    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_tableView->horizontalHeader()->setMinimumSectionSize(-1);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->horizontalHeader()->setStretchLastSection(true);

    auto headerIterator = QMapIterator<Nedrysoft::RouteAnalyser::PingData::Fields, QPair<QString, QString> >(headerMap());

    while (headerIterator.hasNext()) {
        headerIterator.next();

        auto headerItem = new QStandardItem();

        auto pair = headerIterator.value();

        headerItem->setText(pair.first);

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
        auto maxWidth = qMax(
                m_tableView->fontMetrics().boundingRect(pair.first).width(),
                m_tableView->fontMetrics().boundingRect(pair.second).width() );
#else
        auto maxWidth = qMax(
                m_tableView->fontMetrics().horizontalAdvance(pair.first),
                m_tableView->fontMetrics().horizontalAdvance(pair.second) );
#endif

        m_tableModel->setHorizontalHeaderItem(static_cast<int>(headerIterator.key()), headerItem);

        m_tableView->horizontalHeader()->resizeSection(static_cast<int>(headerIterator.key()), maxWidth);
    }

    m_splitter->addWidget(m_tableView);
    m_splitter->addWidget(m_scrollArea);

    m_splitter->setStretchFactor(1, 2);

    auto verticalLayout = new QVBoxLayout();

    verticalLayout->setMargin(0);

    verticalLayout->addWidget(m_splitter);

    this->setLayout(verticalLayout);

    m_layerCleanupTimer = new QTimer();

    m_layerCleanupTimer->setInterval(1000);

    connect(m_layerCleanupTimer, &QTimer::timeout, [=]() {
        GraphLatencyLayer::removeUnused();
    });

    m_layerCleanupTimer->start();
}

Nedrysoft::RouteAnalyser::RouteAnalyserWidget::~RouteAnalyserWidget() {
    if (m_tableView) {
        delete m_tableView;
    }

    if (m_tableModel) {
        delete m_tableModel;
    }

    if (m_layerCleanupTimer) {
        delete m_layerCleanupTimer;
    }
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::onPingResult(Nedrysoft::Core::PingResult result) -> void {
    auto pingData = static_cast<PingData *>(result.target()->userData());
    QCustomPlot *customPlot;

    if (!pingData) {
        return;
    }

    customPlot = pingData->customPlot();

    if (!customPlot) {
        return;
    }

    switch (result.code()) {
        case Nedrysoft::Core::PingResult::ResultCode::Ok:
        case Nedrysoft::Core::PingResult::ResultCode::TimeExceeded: {
            QCPRange graphRange = customPlot->yAxis->range();
            auto requestTime = std::chrono::duration<double>(result.requestTime().time_since_epoch());

            customPlot->graph(RoundTripGraph)->addData(requestTime.count(), result.roundTripTime().count());

            if (m_startPoint == -1) {
                m_startPoint = requestTime.count();
            } else {
                if (requestTime.count() < m_startPoint) {
                    m_startPoint = requestTime.count();
                }
            }

            if (requestTime.count() > m_endPoint) {
                m_endPoint = requestTime.count();
            }

            updateRanges();

            Q_EMIT datasetChanged(m_startPoint, m_endPoint);

            pingData->updateItem(result);

            switch(m_graphScaleMode) {
                case ScaleMode::None: {
                    if (result.roundTripTime().count() > graphRange.upper) {
                        customPlot->yAxis->setRange(0, result.roundTripTime().count());
                    }

                    break;
                }

                case ScaleMode::Normalised:  {
                    auto graphMaxLatency = m_tableView->property("graphMaxLatency").toDouble();

                    if (graphMaxLatency>graphRange.upper) {
                        for (QCustomPlot *currentPlot : m_plotList) {
                            currentPlot->yAxis->setRange(0, graphMaxLatency);
                        }
                    }

                    break;
                }

                case ScaleMode::Fixed: {
                    // TODO: Fixed scaling, user sets the max value.
                    break;
                }
            }

            m_tableView->viewport()->update();

            break;
        }

        case Nedrysoft::Core::PingResult::ResultCode::NoReply: {
            auto requestTime = std::chrono::duration<double>(result.requestTime().time_since_epoch());

            QCPBars *barChart = m_barCharts[customPlot];

            barChart->addData(requestTime.count(), 1);

            pingData->updateItem(result);

            break;
        }
    }
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::onRouteResult(
        const QHostAddress &routeHostAddress,
        const Nedrysoft::Core::RouteList route ) -> void {

    Nedrysoft::Core::IRouteEngine *routeEngine = qobject_cast<Nedrysoft::Core::IRouteEngine *>(this->sender());
    auto hop = 1;
    auto geoIP = Nedrysoft::ComponentSystem::getObject<Nedrysoft::Core::IGeoIPProvider>();

    SPDLOG_TRACE("Got route result");

    auto latencySettings = Nedrysoft::RouteAnalyser::LatencySettings::getInstance();

    assert(latencySettings!=nullptr);

    if (routeEngine) {
        disconnect(
                routeEngine,
                &Nedrysoft::Core::IRouteEngine::result,
                this,
                &RouteAnalyserWidget::onRouteResult );
    }

    if (!m_pingEngineFactory) {
        return;
    }

    if (routeHostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
        m_pingEngine = m_pingEngineFactory->createEngine(Nedrysoft::Core::IPVersion::V4);
    } else if (routeHostAddress.protocol() == QAbstractSocket::IPv6Protocol) {
        m_pingEngine = m_pingEngineFactory->createEngine(Nedrysoft::Core::IPVersion::V6);
    } else {
        return;
    }

    auto interval = std::chrono::duration<double, std::ratio<1, 1> >(m_interval);

    m_pingEngine->setInterval(std::chrono::duration_cast<std::chrono::milliseconds>(interval));

    connect(m_pingEngine,
            &Nedrysoft::Core::IPingEngine::result,
            this,
            &RouteAnalyserWidget::onPingResult );

    auto verticalLayout = new QVBoxLayout();

    for (const QHostAddress &host : route) {
        if (!host.isNull()) {
            auto hostAddress = host.toString();
            auto hostName = QHostInfo::fromName(host.toString()).hostName();

            auto maskedHostName = hostName;
            auto maskedHostAddress = hostAddress;

            for (auto masker : Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IHostMasker>()) {
                masker->mask(hop, hostName, hostAddress, maskedHostName, maskedHostAddress);
            }

            auto customPlot = new QCustomPlot();

            customPlot->addLayer("newBackground", customPlot->layer("grid"), QCustomPlot::limBelow);

            auto latencyLayer = new GraphLatencyLayer(customPlot);

            m_backgroundLayers.append(latencyLayer);

            latencyLayer->setGradientEnabled(latencySettings->gradientFill());

            connect(
                    latencySettings,
                    &Nedrysoft::RouteAnalyser::LatencySettings::gradientChanged, [=](bool useGradient) {

                latencyLayer->setGradientEnabled(useGradient);
            });

            customPlot->setCurrentLayer("main");

            customPlot->setMinimumHeight(DefaultGraphHeight);

            customPlot->addGraph();

            // the timeout bar chart uses axis 2 which is a unit axis.  This means it will always draw to the top
            // of the axis independently of the main axis which may scale up/down depending on latency.

            customPlot->yAxis2->setRange(0,1);
            customPlot->yAxis2->setVisible(true);

            auto barChart = new BarChart(customPlot->xAxis, customPlot->yAxis2);

            barChart->setWidthType(QCPBars::wtPlotCoords);
            barChart->setBrush(QColor(NoReplyColour));
            barChart->setPen(QPen(QColor(NoReplyColour)));

            m_barCharts[customPlot] = barChart;

            customPlot->yAxis->ticker()->setTickCount(1);

            QSharedPointer<CPAxisTickerMS> msTicker(new CPAxisTickerMS);

            customPlot->yAxis->setTicker(msTicker);
            customPlot->yAxis->setLabel(tr("Latency (ms)"));
            customPlot->yAxis->setRange(0, DefaultMaxLatency.count());

            QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);

            auto locale = QLocale::system();

            dateTicker->setDateTimeFormat(
                    locale.timeFormat(QLocale::LongFormat).remove("t").trimmed() +
                    "\n" +
                    locale.dateFormat(QLocale::ShortFormat) );

            customPlot->xAxis->setTicker(dateTicker);
            customPlot->xAxis->setRange(
                    static_cast<double>(QDateTime::currentDateTime().toSecsSinceEpoch()),
                    static_cast<double>(QDateTime::currentDateTime().toSecsSinceEpoch() + m_viewportSize) );

            customPlot->graph(RoundTripGraph)->setLineStyle(QCPGraph::lsStepCenter);

            customPlot->setBackground(this->palette().brush(QPalette::Base));
            customPlot->xAxis->setLabelColor(this->palette().color(QPalette::Text));
            customPlot->yAxis->setLabelColor(this->palette().color(QPalette::Text));
            customPlot->xAxis->setTickLabelColor(this->palette().color(QPalette::Text));
            customPlot->yAxis->setTickLabelColor(this->palette().color(QPalette::Text));

            customPlot->replot();

            /**
             * scroll wheel events, by default QCustomPlot does not propagate these so this code ensures that they cause
             * the scroll area to scroll.
             */

            connect(customPlot, &QCustomPlot::mouseWheel, [this](QWheelEvent *event) {
                m_scrollArea->verticalScrollBar()->setValue(
                        m_scrollArea->verticalScrollBar()->value() - event->angleDelta().y() );
            });

            /**
             *  mouse over event
             */

            auto graphLine = new QCPItemStraightLine(customPlot);

            graphLine->setPen(QPen(Qt::darkGray, 2, Qt::DotLine));

            m_graphLines[customPlot] = graphLine;

            connect(customPlot, &QCustomPlot::mouseMove,
                    [this, customPlot, graphLine, maskedHostName](QMouseEvent *event) {
                        auto x = customPlot->xAxis->pixelToCoord(event->pos().x());
                        auto foundRange = false;
                        auto dataRange = customPlot->graph(RoundTripGraph)->data()->keyRange(foundRange);

                        graphLine->point1->setCoords(x, 0);
                        graphLine->point2->setCoords(x, 1);

                        customPlot->replot();

                        if (( foundRange ) &&
                            ( x >= dataRange.lower ) &&
                            ( x <= dataRange.upper )) {
                            auto valueString = QString();
                            /*auto valueResultRange = customPlot->graph(RoundTripGraph)->data()->valueRange(
                                    foundRange,
                                    QCP::sdBoth,
                                    QCPRange(x - 1, x +1) );*/

                            for (auto currentItem = 0; currentItem < m_tableModel->rowCount(); currentItem++) {
                                auto pingData = m_tableModel->item(
                                        currentItem,
                                        0 )->data().value<Nedrysoft::RouteAnalyser::PingData *>();

                                auto valueRange = QCPRange(x - 1, x + 1);

                                if (pingData->customPlot()) {
                                    auto tempResultRange = pingData->customPlot()->graph(
                                            RoundTripGraph)->data()->valueRange(foundRange, QCP::sdBoth, valueRange);

                                    auto seconds = std::chrono::duration<double>(tempResultRange.upper);

                                    pingData->setHistoricalLatency(seconds);
                                } else {
                                    pingData->setHistoricalLatency(std::chrono::duration<double>(-1));

                                    auto topLeft = m_tableModel->index(0, 0);
                                    auto bottomRight = topLeft.sibling(m_tableModel->rowCount() - 1,
                                                                       m_tableModel->columnCount() - 1);

                                    m_tableModel->dataChanged(topLeft, bottomRight);
                                }
                            }

                            this->m_tableModel->setProperty("showHistorical", true);

                            /*
                            auto seconds = std::chrono::duration<double>(valueResultRange.upper);

                            if (seconds < std::chrono::seconds(1)) {
                                auto milliseconds =
                                    std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(seconds);

                                valueString = QString(tr("%1ms")).arg(milliseconds.count(), 0, 'f', 2);
                            } else {
                                valueString = QString(tr("%1s")).arg(seconds.count(), 0, 'f', 2);
                            }

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
                            auto bottomRight = topLeft.sibling(
                                    m_tableModel->rowCount() - 1,
                                    m_tableModel->columnCount() - 1 );

                            m_tableModel->dataChanged(topLeft, bottomRight);
                        }
                    });

            customPlot->installEventFilter(this);

            m_plotList.append(customPlot);

            // add plot title.

            auto plotTitleLabel = new QLabel(QString(tr("Hop %1")).arg(hop) + " " + maskedHostName + " (" + maskedHostAddress + ")");

            QFont labelFont = plotTitleLabel->font();

            labelFont.setPointSize(16);

            plotTitleLabel->setFont(labelFont);

            plotTitleLabel->setAlignment(Qt::AlignHCenter);

            verticalLayout->addWidget(plotTitleLabel);

            // add any pre-plots.

            auto plotFactories = ComponentSystem::getObjects<Nedrysoft::RouteAnalyser::IPlotFactory>();

            QList<Nedrysoft::RouteAnalyser::IPlot *> plots;

            for (auto plotFactory : plotFactories) {
                auto plot = plotFactory->createPlot(PlotMargins);

                m_extraPlots.append(plot);

                plots.append(plot);

                verticalLayout->addWidget(plot->widget());
            }

            customPlot->axisRect()->setAutoMargins(QCP::msNone);
            customPlot->axisRect()->setMargins(PlotMargins);

            // add the main plot

            verticalLayout->addWidget(customPlot);

            auto pingTarget = m_pingEngine->addTarget(routeHostAddress, hop);

            auto pingData = new Nedrysoft::RouteAnalyser::PingData(m_tableModel, hop, true);

            auto tableItem = new QStandardItem(1, m_tableModel->columnCount());

            tableItem->setData(QVariant::fromValue<Nedrysoft::RouteAnalyser::PingData *>(pingData));

            m_tableModel->appendRow(tableItem);

            m_tableView->setRowHeight(tableItem->index().row(), TableRowHeight);

            connect(m_tableView, &QObject::destroyed, [pingData](QObject *) {
                delete pingData;
            });

            pingData->setPlots(plots);
            pingData->setCustomPlot(customPlot);
            pingData->setHostAddress(maskedHostAddress);
            pingData->setHostName(maskedHostName);

            pingTarget->setUserData(pingData);

            if (geoIP) {
                geoIP->lookup(hostAddress, [pingData](const QString &, const QVariantMap &result) mutable {
                    pingData->setLocation(result["country"].toString());
                });
            }
        } else {
            auto pingData = new Nedrysoft::RouteAnalyser::PingData(m_tableModel, hop, false);

            connect(m_tableView, &QObject::destroyed, [pingData](QObject *) {
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

    connect(this, &Nedrysoft::RouteAnalyser::RouteAnalyserWidget::filteredEvent, [=](QObject *watched, QEvent *event) {
        auto customPlot = qobject_cast<QCustomPlot *>(watched);

        auto line = m_graphLines[customPlot];

        if (event->type() == QEvent::PaletteChange) {
            customPlot->setBackground(this->palette().brush(QPalette::Base));

            customPlot->xAxis->setLabelColor(this->palette().color(QPalette::Text));
            customPlot->yAxis->setLabelColor(this->palette().color(QPalette::Text));
            customPlot->xAxis->setTickLabelColor(this->palette().color(QPalette::Text));
            customPlot->yAxis->setTickLabelColor(this->palette().color(QPalette::Text));

            QCPTextElement *textElement = qobject_cast<QCPTextElement *>(customPlot->plotLayout()->element(0, 0));

            if (textElement) {
                textElement->setTextColor(this->palette().color(QPalette::Text));
            }
        }

        if (( event->type() == QEvent::Enter ) ||
            ( event->type() == QEvent::Leave )) {

            /*m_pointInfoLabel->setText("");
            m_hopInfoLabel->setText("");
            m_hostInfoLabel->setText("");
            m_timeInfoLabel->setText("");*/

            line->setVisible(event->type() == QEvent::Enter);

            customPlot->replot();

            this->m_tableModel->setProperty("showHistorical", false);

            auto topLeft = m_tableModel->index(0, 0);
            auto bottomRight = topLeft.sibling(m_tableModel->rowCount() - 1, m_tableModel->columnCount() - 1);

            m_tableModel->dataChanged(topLeft, bottomRight);
        }
    });

    m_scrollArea->widget()->setLayout(verticalLayout);

    m_pingEngine->start();
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::eventFilter(QObject *watched, QEvent *event) -> bool {
    Q_EMIT filteredEvent(watched, event);

    return QWidget::eventFilter(watched, event);
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::paintEvent(QPaintEvent *paintEvent) -> void {
    QWidget::paintEvent(paintEvent);
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::setGradientEnabled(bool smoothGradient) -> void {
    for(auto layer : m_backgroundLayers) {
        layer->setGradientEnabled(smoothGradient);
    }

    if (m_routeGraphDelegate) {
        m_routeGraphDelegate->setGradientEnabled(smoothGradient);
    }
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::setViewportSize(double viewportSize) -> void {
    m_viewportSize = viewportSize;

    updateRanges();
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::viewportSize(void) -> double {
    return m_viewportSize;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::datasetSize(void) -> double {
    return m_endPoint - m_startPoint;
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::setViewportPosition(double position) -> void {
    m_viewportPosition = qMin(qMax(0.0, position), 1.0);

    updateRanges();
}

auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::viewportPosition() -> double {
    return m_viewportPosition;
}


auto Nedrysoft::RouteAnalyser::RouteAnalyserWidget::updateRanges() -> void {
    double diff = m_endPoint - m_startPoint;
    double max, min;
    double maxVisibleLatency = 0;

    // TODO: m_savedDiff should default to the viewport size

    min = m_startPoint;
    max = m_startPoint+m_viewportSize;

    if (m_viewportPosition == 1) {
        if (diff > m_viewportSize) {
            max = m_endPoint;
            min = max - m_viewportSize;
            m_savedDiff = diff-m_viewportSize;
        }
    } else {
        if (diff > m_viewportSize) {
            min = (m_savedDiff*m_viewportPosition)+m_startPoint;
            max = min + m_viewportSize;
        }
    }

    for (auto plot : m_plotList) {
        bool foundRange;

        plot->xAxis->setRange(min, max);

        QCPRange visibleKeyRange = plot->xAxis->range();

        QCPRange valueRange = plot->graph()->getValueRange(foundRange, QCP::sdBoth, visibleKeyRange);

        if (valueRange.upper>maxVisibleLatency) {
            maxVisibleLatency = valueRange.upper;
        }
    }

    for (auto plot : m_extraPlots) {
        plot->updateRange(min, max);
    }

    // TODO: go through the bar charts and set to maximum as well.

    for (auto plot : m_plotList) {
        if (m_graphScaleMode==ScaleMode::Normalised) {
            plot->graph(0)->valueAxis()->setRangeUpper(maxVisibleLatency);
        }

        if (plot->isVisible()) {
            if (!plot->visibleRegion().isEmpty()) {
                plot->replot(QCustomPlot::rpQueuedReplot);
            }
        }
    }
}
