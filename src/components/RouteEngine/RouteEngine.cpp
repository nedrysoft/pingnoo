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

#include "RouteEngine.h"

#include "Core/IPingEngine.h"
#include "Core/IPingEngineFactory.h"
#include "Core/IPingTarget.h"

#include <QHostInfo>
#include <QThread>
#include <QTimer>

#include <cassert>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

constexpr auto DefaultDiscoveryTimeout = 1s;
constexpr auto MaxRouteHops = 64;

Nedrysoft::RouteEngine::RouteEngine::RouteEngine() :
        m_ipVersion(Nedrysoft::Core::IPVersion::V4),
        m_pingEngineFactory(nullptr),
        m_pingEngine(nullptr),
        m_timeoutTimer(nullptr) {

}

auto Nedrysoft::RouteEngine::RouteEngine::findRoute(
        Nedrysoft::Core::IPingEngineFactory *engineFactory,
        QString host,
        Nedrysoft::Core::IPVersion ipVersion) -> void {

    m_host = host;
    m_ipVersion = ipVersion;

    m_pingEngineFactory = engineFactory;

    m_targetAddresses = QHostInfo::fromName(m_host).addresses();

    auto hopAddress = QHostAddress();

    assert(m_pingEngineFactory!=nullptr);

    if (!m_targetAddresses.count()) {
        Q_EMIT result(QHostAddress(), Nedrysoft::Core::RouteList());

        SPDLOG_ERROR(QString("Failed to find address for %1.").arg(m_host).toStdString());

        return;
    }

    auto thread = QThread::create([=]() {
        m_pingEngine = m_pingEngineFactory->createEngine(m_ipVersion);

        auto route = Nedrysoft::Core::RouteList();

        for (int hop=1;hop<64;hop++) {
            auto result = m_pingEngine->singleShot(m_targetAddresses.at(0), hop);

            if (result.code()==Nedrysoft::Core::PingResult::ResultCode::Ok) {
                route.append(result.hostAddress());
                break;
            } else  if (result.code()==Nedrysoft::Core::PingResult::ResultCode::TimeExceeded) {
                route.append(result.hostAddress());
            } else  {
                route.append(QHostAddress());
            }
        };

        SPDLOG_TRACE(QString("Route to %1 (%2) completed, total of %3 hops.")
                             .arg(m_host)
                             .arg(m_targetAddresses[0].toString())
                             .arg(route.length())
                             .toStdString() );

        Q_EMIT result(m_targetAddresses[0], route);

        m_pingEngineFactory->deleteEngine(m_pingEngine);

        QThread::currentThread()->deleteLater();
    });

    thread->start();
}
