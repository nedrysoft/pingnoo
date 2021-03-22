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

#include "PingData.h"

#include "RouteTableItemDelegate.h"
#include "SDK/IPlot.h"

#include <QHeaderView>
#include <QStandardItemModel>
#include <QTableWidget>
#include <chrono>

Nedrysoft::RouteAnalyser::PingData::PingData(QStandardItemModel *tableModel, int hop, bool hopValid) :
        m_tableModel(tableModel),
        m_customPlot(nullptr),
        m_jitterPlot(nullptr),
        m_replyPacketCount(0),
        m_timeoutPacketCount(0),
        m_hop(hop),
        m_hopValid(hopValid),
        m_count(0),
        m_currentLatency(seconds_double(-1)),
        m_maximumLatency(seconds_double(-1)),
        m_minimumLatency(seconds_double(-1)),
        m_averageLatency(seconds_double(-1)),
        m_historicalLatency(seconds_double(-1)) {

}

auto Nedrysoft::RouteAnalyser::PingData::runningAverage(double previousAverage, double value, double n) -> double {
    return (previousAverage*(n-1)+value)/n;
}

auto Nedrysoft::RouteAnalyser::PingData::updateModel() -> void {
    if (m_tableModel) {
        auto topLeft = m_tableModel->index(0, 0);
        auto bottomRight = topLeft.sibling(m_tableModel->rowCount() - 1, m_tableModel->columnCount() - 1);

        m_tableModel->dataChanged(topLeft, bottomRight);
    }
}

auto Nedrysoft::RouteAnalyser::PingData::setHop(int hop) -> void {
    m_hop = hop;

    if (m_tableModel) {
        updateModel();
    }
}

auto Nedrysoft::RouteAnalyser::PingData::hop() -> int {
    return m_hop;
}

auto Nedrysoft::RouteAnalyser::PingData::setHostAddress(const QString &hostAddress) -> void {
    m_hostAddress = hostAddress;

    if (m_tableModel) {
        updateModel();
    }
}

auto Nedrysoft::RouteAnalyser::PingData::hostAddress() -> QString {
    return m_hostAddress;
}

auto Nedrysoft::RouteAnalyser::PingData::setHostName(const QString &hostName) -> void {
    m_hostName = hostName;

    if (m_tableModel) {
        updateModel();
    }
}

auto Nedrysoft::RouteAnalyser::PingData::hostName() -> QString {
    return m_hostName;
}

auto Nedrysoft::RouteAnalyser::PingData::packetLoss() -> double {
    return (static_cast<double>(m_timeoutPacketCount)/
            static_cast<double>(m_replyPacketCount+m_timeoutPacketCount))*100.0;
}

auto Nedrysoft::RouteAnalyser::PingData::updateItem(Nedrysoft::Core::PingResult result) -> void {
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

    /*auto packetLoss = (static_cast<double>(m_timeoutPacketCount)/
                static_cast<double>(m_replyPacketCount+m_timeoutPacketCount))*100.0;*/

    for (auto plot : m_plots) {
        auto requestTime = std::chrono::duration<double>(result.requestTime().time_since_epoch());

        plot->update(requestTime.count(), result.roundTripTime().count());
    }

    if (m_tableModel) {
        updateModel();
    }
}

auto Nedrysoft::RouteAnalyser::PingData::setCustomPlot(QCustomPlot *customPlot) -> void {
    m_customPlot = customPlot;
}

auto Nedrysoft::RouteAnalyser::PingData::customPlot() -> QCustomPlot * {
    return m_customPlot;
}

auto Nedrysoft::RouteAnalyser::PingData::location() -> QString {
    return m_location;
}

auto Nedrysoft::RouteAnalyser::PingData::setLocation(const QString &location) -> void {
    m_location = location;

    if (m_tableModel) {
        updateModel();
    }
}

auto Nedrysoft::RouteAnalyser::PingData::hopValid() -> bool {
    return m_hopValid;
}

auto Nedrysoft::RouteAnalyser::PingData::setHopValid(bool hopValid) -> void {
    m_hopValid = hopValid;

    if (m_tableModel) {
        updateModel();
    }
}

auto Nedrysoft::RouteAnalyser::PingData::setHistoricalLatency(std::chrono::duration<double> latency) -> void {
    m_historicalLatency = latency;

    if (m_tableModel) {
        updateModel();
    }
}

auto Nedrysoft::RouteAnalyser::PingData::latency(int field) -> double {
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

auto Nedrysoft::RouteAnalyser::PingData::tableModel() -> QStandardItemModel * {
    return m_tableModel;
}

auto Nedrysoft::RouteAnalyser::PingData::count() -> unsigned long  {
    return m_count;
}

auto Nedrysoft::RouteAnalyser::PingData::setPlots(QList<Nedrysoft::RouteAnalyser::IPlot *> plots) -> void {
    m_plots = plots;
}
