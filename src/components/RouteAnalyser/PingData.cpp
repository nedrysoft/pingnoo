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

#include "PingData.h"
#include "RouteTableItemDelegate.h"
#include <chrono>
#include <QObject>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QStandardItemModel>

FizzyAde::RouteAnalyser::PingData::PingData(QStandardItemModel *tableModel, int hop, bool hopValid)
{
    m_jitterPlot = nullptr;
    m_hop = hop;
    m_hopValid = hopValid;
    m_tableModel = tableModel;
    m_count = 0;

    m_currentLatency = seconds_double(-1);
    m_maximumLatency = seconds_double(-1);
    m_minimumLatency = seconds_double(-1);
    m_averageLatency = seconds_double(-1);
    m_historicalLatency = seconds_double(-1);

    m_customPlot = nullptr;

    m_replyPacketCount = 0;
    m_timeoutPacketCount = 0;
}

double FizzyAde::RouteAnalyser::PingData::runningAverage(double previousAverage, double value, double n)
{
    return (previousAverage*(n-1)+value)/n;
}

void FizzyAde::RouteAnalyser::PingData::updateModel()
{
    if (m_tableModel) {
        auto topLeft = m_tableModel->index(0, 0);
        auto bottomRight = topLeft.sibling(m_tableModel->rowCount()-1, m_tableModel->columnCount()-1);

        m_tableModel->dataChanged(topLeft, bottomRight);
    }
}

void FizzyAde::RouteAnalyser::PingData::setHop(int hop)
{
    m_hop = hop;

    if (m_tableModel) {
        updateModel();
    }
}

int FizzyAde::RouteAnalyser::PingData::hop()
{
    return(m_hop);
}

void FizzyAde::RouteAnalyser::PingData::setHostAddress(QString hostAddress)
{
    m_hostAddress = std::move(hostAddress);

    if (m_tableModel) {
        updateModel();
    }
}

QString FizzyAde::RouteAnalyser::PingData::hostAddress()
{
    return(m_hostAddress);
}

void FizzyAde::RouteAnalyser::PingData::setHostName(QString hostName)
{
    m_hostName = std::move(hostName);

    if (m_tableModel) {
        updateModel();
    }
}

QString FizzyAde::RouteAnalyser::PingData::hostName()
{
    return(m_hostName);
}

double FizzyAde::RouteAnalyser::PingData::packetLoss()
{
    return((static_cast<double>(m_timeoutPacketCount)/static_cast<double>(m_replyPacketCount+m_timeoutPacketCount))*100.0);
}

void FizzyAde::RouteAnalyser::PingData::updateItem(FizzyAde::Core::PingResult result)
{
    double packetLoss;

    m_count = result.sampleNumber();

    if (result.code()==FizzyAde::Core::PingResult::NoReply) {
        m_timeoutPacketCount++;

        if (m_tableModel) {
            updateModel();
        }

        return;
    }

    m_currentLatency = result.roundTripTime();

    if (m_minimumLatency<seconds_double(0)) {
        m_minimumLatency = m_currentLatency;
    }

    if (m_maximumLatency<seconds_double(0)) {
        m_maximumLatency = m_currentLatency;
    }

    if (m_currentLatency<m_minimumLatency) {
        m_minimumLatency = m_currentLatency;
    }

    if (m_currentLatency>m_maximumLatency) {
        m_maximumLatency = m_currentLatency;
    }

    if (m_maximumLatency.count()>m_tableModel->property("graphMaxLatency").toDouble()) {
        if (m_tableModel) {
            m_tableModel->setProperty("graphMaxLatency", QVariant(m_maximumLatency.count()));
        }

        auto headerItem = m_tableModel->horizontalHeaderItem(Graph);

        headerItem->setTextAlignment(Qt::AlignRight);

        headerItem->setText(QString(QObject::tr("%1 ms")).arg(std::chrono::duration_cast<std::chrono::milliseconds>(m_currentLatency).count()));
    }

    if (m_minimumLatency.count()<m_tableModel->property("graphMinLatency").toDouble()) {
        if (m_tableModel) {
            m_tableModel->setProperty("graphMinLatency", QVariant(m_minimumLatency.count()));
        }
    }

    if (m_averageLatency<seconds_double(0)) {
        m_averageLatency = m_currentLatency;
    } else {
        m_averageLatency = seconds_double(runningAverage(m_averageLatency.count(), m_currentLatency.count(), static_cast<double>(m_replyPacketCount)+1.0));
    }

    m_replyPacketCount++;

    packetLoss = (static_cast<double>(m_timeoutPacketCount)/static_cast<double>(m_replyPacketCount+m_timeoutPacketCount))*100.0;

    if (m_tableModel) {
        updateModel();
    }
}

void FizzyAde::RouteAnalyser::PingData::setCustomPlot(QCustomPlot *customPlot)
{
    m_customPlot = customPlot;
}

QCustomPlot *FizzyAde::RouteAnalyser::PingData::customPlot()
{
    return(m_customPlot);
}

void FizzyAde::RouteAnalyser::PingData::setJitterPlot(QCustomPlot *jitterPlot)
{
    m_jitterPlot = jitterPlot;
}

QCustomPlot *FizzyAde::RouteAnalyser::PingData::jitterPlot()
{
    return(m_jitterPlot);
}

QString FizzyAde::RouteAnalyser::PingData::location()
{
    return(m_location);
}

void FizzyAde::RouteAnalyser::PingData::setLocation(const QString &location)
{
    m_location = location;

    if (m_tableModel) {
        updateModel();
    }
}

bool FizzyAde::RouteAnalyser::PingData::hopValid()
{
    return(m_hopValid);
}

void FizzyAde::RouteAnalyser::PingData::setHopValid(bool hopValid)
{
    m_hopValid = hopValid;

    if (m_tableModel) {
        updateModel();
    }
}

void FizzyAde::RouteAnalyser::PingData::setHistoricalLatency(std::chrono::duration<double> latency)
{
    m_historicalLatency = latency;

    if (m_tableModel) {
        updateModel();
    }
}

double FizzyAde::RouteAnalyser::PingData::latency(int field)
{
    switch(field) {
    case MinimumLatency: {
        return(m_minimumLatency.count());
    }
    case MaximumLatency: {
        return(m_maximumLatency.count());
    }
    case CurrentLatency: {
        return(m_currentLatency.count());
    }
    case AverageLatency: {
        return(m_averageLatency.count());
    }
    case HistoricalLatency: {
        return(m_historicalLatency.count());
    }
    }

    return(0);
}

QStandardItemModel *FizzyAde::RouteAnalyser::PingData::tableModel()
{
    return(m_tableModel);
}

unsigned long FizzyAde::RouteAnalyser::PingData::count()
{
    return(m_count);
}
