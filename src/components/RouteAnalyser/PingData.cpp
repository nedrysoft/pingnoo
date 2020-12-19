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

#include <QDebug>
#include <QHeaderView>
#include <QObject>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <chrono>

Nedrysoft::RouteAnalyser::PingData::PingData(QStandardItemModel *tableModel, int hop, bool hopValid) :
        m_jitterPlot(nullptr),
        m_hop(hop),
        m_hopValid(hopValid),
        m_tableModel(tableModel),
        m_count(0),
        m_currentLatency(seconds_double(-1)),
        m_maximumLatency(seconds_double(-1)),
        m_minimumLatency(seconds_double(-1)),
        m_averageLatency(seconds_double(-1)),
        m_historicalLatency(seconds_double(-1)),
        m_customPlot(nullptr),
        m_replyPacketCount(0),
        m_timeoutPacketCount(0) {

}

double Nedrysoft::RouteAnalyser::PingData::runningAverage(double previousAverage, double value, double n) {
    return (previousAverage*(n-1)+value)/n;
}

void Nedrysoft::RouteAnalyser::PingData::updateModel() {
    if (m_tableModel) {
        auto topLeft = m_tableModel->index(0, 0);
        auto bottomRight = topLeft.sibling(m_tableModel->rowCount() - 1, m_tableModel->columnCount() - 1);

        m_tableModel->dataChanged(topLeft, bottomRight);
    }
}

void Nedrysoft::RouteAnalyser::PingData::setHop(int hop) {
    m_hop = hop;

    if (m_tableModel) {
        updateModel();
    }
}

int Nedrysoft::RouteAnalyser::PingData::hop() {
    return m_hop;
}

void Nedrysoft::RouteAnalyser::PingData::setHostAddress(QString hostAddress) {
    m_hostAddress = std::move(hostAddress);

    if (m_tableModel) {
        updateModel();
    }
}

QString Nedrysoft::RouteAnalyser::PingData::hostAddress() {
    return m_hostAddress;
}

void Nedrysoft::RouteAnalyser::PingData::setHostName(QString hostName) {
    m_hostName = std::move(hostName);

    if (m_tableModel) {
        updateModel();
    }
}

QString Nedrysoft::RouteAnalyser::PingData::hostName() {
    return m_hostName;
}

double Nedrysoft::RouteAnalyser::PingData::packetLoss() {
    return ( static_cast<double>(m_timeoutPacketCount) /
             static_cast<double>(m_replyPacketCount + m_timeoutPacketCount)) * 100.0;
}

void Nedrysoft::RouteAnalyser::PingData::updateItem(Nedrysoft::Core::PingResult result) {
    double packetLoss;

    m_count = result.sampleNumber();

    if (result.code() == Nedrysoft::Core::PingResult::ResultCode::NoReply) {
        m_timeoutPacketCount++;

        if (m_tableModel) {
            updateModel();
        }

        return;
    }

    m_currentLatency = result.roundTripTime();

    if (m_minimumLatency < seconds_double(0)) {
        m_minimumLatency = m_currentLatency;
    }

    if (m_maximumLatency < seconds_double(0)) {
        m_maximumLatency = m_currentLatency;
    }

    if (m_currentLatency < m_minimumLatency) {
        m_minimumLatency = m_currentLatency;
    }

    if (m_currentLatency > m_maximumLatency) {
        m_maximumLatency = m_currentLatency;
    }

    if (m_maximumLatency.count() > m_tableModel->property("graphMaxLatency").toDouble()) {
        if (m_tableModel) {
            m_tableModel->setProperty("graphMaxLatency", QVariant(m_maximumLatency.count()));
        }

        auto headerItem = m_tableModel->horizontalHeaderItem(static_cast<int>(Fields::Graph));

        headerItem->setTextAlignment(Qt::AlignRight);

        headerItem->setText(QString(QObject::tr("%1 ms")).arg(
                std::chrono::duration_cast<std::chrono::milliseconds>(m_currentLatency).count()));
    }

    if (m_minimumLatency.count() < m_tableModel->property("graphMinLatency").toDouble()) {
        if (m_tableModel) {
            m_tableModel->setProperty("graphMinLatency", QVariant(m_minimumLatency.count()));
        }
    }

    if (m_averageLatency < seconds_double(0)) {
        m_averageLatency = m_currentLatency;
    } else {
        m_averageLatency = seconds_double(
                runningAverage(
                        m_averageLatency.count(),
                        m_currentLatency.count(),
                        static_cast<double>(m_replyPacketCount) + 1.0) );
    }

    m_replyPacketCount++;

    packetLoss = (static_cast<double>(m_timeoutPacketCount)/
                static_cast<double>(m_replyPacketCount+m_timeoutPacketCount))*100.0;

    if (m_tableModel) {
        updateModel();
    }
}

void Nedrysoft::RouteAnalyser::PingData::setCustomPlot(QCustomPlot *customPlot) {
    m_customPlot = customPlot;
}

QCustomPlot *Nedrysoft::RouteAnalyser::PingData::customPlot() {
    return m_customPlot;
}

void Nedrysoft::RouteAnalyser::PingData::setJitterPlot(QCustomPlot *jitterPlot) {
    m_jitterPlot = jitterPlot;
}

QCustomPlot *Nedrysoft::RouteAnalyser::PingData::jitterPlot() {
    return m_jitterPlot;
}

QString Nedrysoft::RouteAnalyser::PingData::location() {
    return m_location;
}

void Nedrysoft::RouteAnalyser::PingData::setLocation(const QString &location) {
    m_location = location;

    if (m_tableModel) {
        updateModel();
    }
}

bool Nedrysoft::RouteAnalyser::PingData::hopValid() {
    return m_hopValid;
}

void Nedrysoft::RouteAnalyser::PingData::setHopValid(bool hopValid) {
    m_hopValid = hopValid;

    if (m_tableModel) {
        updateModel();
    }
}

void Nedrysoft::RouteAnalyser::PingData::setHistoricalLatency(std::chrono::duration<double> latency) {
    m_historicalLatency = latency;

    if (m_tableModel) {
        updateModel();
    }
}

double Nedrysoft::RouteAnalyser::PingData::latency(int field) {
    switch (static_cast<Fields>(field)) {
        case Fields::MinimumLatency: {
            return m_minimumLatency.count();
        }

        case Fields::MaximumLatency: {
            return m_maximumLatency.count();
        }

        case Fields::CurrentLatency: {
            return m_currentLatency.count();
        }

        case Fields::AverageLatency: {
            return m_averageLatency.count();
        }

        case Fields::HistoricalLatency: {
            return m_historicalLatency.count();
        }

        default: {
            break;
        }
    }

    return 0;
}

QStandardItemModel *Nedrysoft::RouteAnalyser::PingData::tableModel() {
    return m_tableModel;
}

unsigned long Nedrysoft::RouteAnalyser::PingData::count() {
    return m_count;
}
