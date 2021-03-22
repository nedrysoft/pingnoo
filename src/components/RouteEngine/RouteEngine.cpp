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

constexpr auto DefaultDiscoveryTimeout = 3s;
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

    m_timeoutTimer = new QTimer();

    m_timeoutTimer->setInterval(DefaultDiscoveryTimeout);
    m_timeoutTimer->setSingleShot(true);

    m_timeoutTimer->start();

    m_pingEngine = m_pingEngineFactory->createEngine(m_ipVersion);

    m_pingEngine->setInterval(1s);

    for (int hop=1; hop<MaxRouteHops; hop++) {
        m_pingEngine->addTarget(m_targetAddresses.at(0), hop);

        m_replyMap[hop] = Nedrysoft::Core::PingResult();
    }

    m_replyHop = MaxRouteHops;

    connect(m_pingEngine, &Nedrysoft::Core::IPingEngine::result, [=](Nedrysoft::Core::PingResult result) {
        if (!m_timeoutTimer->isActive()) {
            return;
        }

        if ((result.target()->ttl()>m_replyHop)) {
            return;
        }

        auto existingEntry = m_replyMap.value(result.target()->ttl());

        if (result.code()==Nedrysoft::Core::PingResult::ResultCode::Ok) {
            if (m_replyMap.contains(result.target()->ttl())) {
                if (existingEntry.code()==result.code()) {
                    return;
                } else if (result.code()==existingEntry.code()) {
                    return;
                }

                m_replyMap[result.target()->ttl()] = result;
            }

            m_replyMap.insert(result.target()->ttl(), result);

            m_replyHop = result.target()->ttl();
        } else  if (result.code()==Nedrysoft::Core::PingResult::ResultCode::TimeExceeded) {
            if (m_replyMap.contains(result.target()->ttl())) {
                if (existingEntry.code()==result.code()) {
                    return;
                } else if (existingEntry.code()==Nedrysoft::Core::PingResult::ResultCode::Ok) {
                    return;
                }

                m_replyMap[result.target()->ttl()] = result;
            }

            m_replyMap.insert(result.target()->ttl(), result);

        } else  if (result.code()==Nedrysoft::Core::PingResult::ResultCode::NoReply) {
            if (m_replyMap.contains(result.target()->ttl())) {
                if (existingEntry.code()==result.code()) {
                    return;
                } else if (existingEntry.code()==Nedrysoft::Core::PingResult::ResultCode::NoReply) {
                    return;
                }

                m_replyMap[result.target()->ttl()] = result;
            }

            m_replyMap.insert(result.target()->ttl(), result);
        }
    });

    connect(m_timeoutTimer, &QTimer::timeout, [&]() {
        auto route = Nedrysoft::Core::RouteList();

        for (auto hop=1; hop<=m_replyHop; hop++) {
            if (m_replyMap[hop].code()==Nedrysoft::Core::PingResult::ResultCode::Ok) {
                route.append(m_replyMap[hop].hostAddress());

                break;
            } else  if (m_replyMap[hop].code()==Nedrysoft::Core::PingResult::ResultCode::TimeExceeded) {
                route.append(m_replyMap[hop].hostAddress());
            } else  if (m_replyMap[hop].code()==Nedrysoft::Core::PingResult::ResultCode::NoReply) {
                route.append(QHostAddress());
            }
        }

        SPDLOG_TRACE(QString("Route to %1 (%2) completed, total of %3 hops.")
                             .arg(m_host)
                             .arg(m_targetAddresses[0].toString())
                             .arg(route.length())
                             .toStdString() );

        Q_EMIT result(m_targetAddresses[0], route);

        m_pingEngine->stop();

        // TODO: can't delete as it will cause a crash, so need to add a deleteEngine() functio
        // to the factory to remove the engine from its internal list and delete it.
    });

    m_pingEngine->start();
}
