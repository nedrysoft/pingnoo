/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 28/03/2021.
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

#include "RouteEngineWorker.h"

#include <IPingEngine>
#include <IPingEngineFactory>
#include "spdlog.h"

#include <QHostInfo>

constexpr auto DefaultDiscoveryTimeout = 1.0;
constexpr auto MaxRouteHops = 64;

Nedrysoft::RouteEngine::RouteEngineWorker::RouteEngineWorker(
        QString host,
        Nedrysoft::RouteAnalyser::IPingEngineFactory *pingEngineFactory,
        Nedrysoft::Core::IPVersion ipVersion ) :
            m_host(host),
            m_ipVersion(ipVersion),
            m_pingEngineFactory(pingEngineFactory),
            m_isRunning(false),
            m_maximumHops(MaxRouteHops) {

}

Nedrysoft::RouteEngine::RouteEngineWorker::~RouteEngineWorker() {
    if (m_isRunning) {
        m_isRunning = false;

        thread()->quit();
    }
}

auto Nedrysoft::RouteEngine::RouteEngineWorker::doWork() -> void {
    int totalHops = -1;
    m_isRunning = true;

    auto pingEngine = m_pingEngineFactory->createEngine(m_ipVersion);

    auto targetAddresses = QHostInfo::fromName(m_host).addresses();

    if (!targetAddresses.count()) {
        Q_EMIT result(QHostAddress(), Nedrysoft::RouteAnalyser::RouteList(), true, -1, m_maximumHops);

        SPDLOG_ERROR(QString("Failed to find address for %1.").arg(m_host).toStdString());

        this->deleteLater();

        return;
    }

    auto route = Nedrysoft::RouteAnalyser::RouteList();

    auto pingResult = pingEngine->singleShot(
        targetAddresses.at(0),
        m_maximumHops,
        DefaultDiscoveryTimeout
    );

    if (pingResult.code()==Nedrysoft::RouteAnalyser::PingResult::ResultCode::Ok) {
        totalHops = pingResult.hops();
    }

    for (int hop=1;hop<MaxRouteHops;hop++) {
        if (!m_isRunning) {
            m_pingEngineFactory->deleteEngine(pingEngine);

            return;
        }

        pingResult = pingEngine->singleShot(targetAddresses.at(0), hop, DefaultDiscoveryTimeout);

        if (pingResult.code()==Nedrysoft::RouteAnalyser::PingResult::ResultCode::Ok) {
            route.append(pingResult.hostAddress());
            break;
        } else  if (pingResult.code()==Nedrysoft::RouteAnalyser::PingResult::ResultCode::TimeExceeded) {
            route.append(pingResult.hostAddress());
        } else  {
            route.append(QHostAddress());
        }

        Q_EMIT result(targetAddresses[0], route, false, totalHops, m_maximumHops);
    };

    SPDLOG_TRACE(QString("Route to %1 (%2) completed, total of %3 hops.")
                         .arg(m_host)
                         .arg(m_targetAddresses[0].toString())
                         .arg(route.length())
                         .toStdString() );

    /**
     * we emit the final hop twice, oncce with completed set to false then true, this ensures that the
     * behaviour to listeners is the same for each hop, i.e every hop gets a result signal with completed
     * set to false, without the extra emit the final hop would behave differently.
     */

    Q_EMIT result(targetAddresses[0], route, false, totalHops, m_maximumHops);
    Q_EMIT result(targetAddresses[0], route, true, totalHops, m_maximumHops);

    m_pingEngineFactory->deleteEngine(pingEngine);

    this->deleteLater();
}
