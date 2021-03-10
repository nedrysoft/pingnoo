/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 22/01/2021.
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

#include "PingCommandPingEngine.h"

#include "PingCommandPingTarget.h"

#include <QMutex>
#include <QThread>
#include <chrono>

using namespace std::chrono_literals;

constexpr auto DefaultReceiveTimeout = 3s;
constexpr auto DefaultTerminateThreadTimeout = 5s;
constexpr auto DefaultTTL = 64;

Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::PingCommandPingEngine(Nedrysoft::Core::IPVersion version) {
    Q_UNUSED(version)

}

Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::~PingCommandPingEngine() {

}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::addTarget(QHostAddress hostAddress) -> Nedrysoft::Core::IPingTarget * {
    return addTarget(hostAddress, DefaultTTL);
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::addTarget(
        QHostAddress hostAddress,
        int ttl ) -> Nedrysoft::Core::IPingTarget * {

    auto newTarget = new Nedrysoft::PingCommandPingEngine::PingCommandPingTarget(
            this,
            hostAddress,
            ttl );

    m_pingTargets.append(newTarget);

    return newTarget;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::removeTarget(Nedrysoft::Core::IPingTarget *target) -> bool {
    Q_UNUSED(target)

    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::start() -> bool {
    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::stop() -> bool {
    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::setInterval(std::chrono::milliseconds interval) -> bool {
    m_interval = interval;

    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::interval() -> std::chrono::milliseconds {
    return m_interval;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::setTimeout(std::chrono::milliseconds timeout) -> bool {
    Q_UNUSED(timeout)

    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::epoch() -> std::chrono::system_clock::time_point {
    return std::chrono::system_clock::now();
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::saveConfiguration() -> QJsonObject {
   return QJsonObject();
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::emitResult(Nedrysoft::Core::PingResult pingResult) -> void {
    Q_EMIT result(pingResult);
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::targets() -> QList<Nedrysoft::Core::IPingTarget *> {
    QList<Nedrysoft::Core::IPingTarget *> list;

    for (auto target : m_pingTargets) {
        list.append(target);
    }

    return list;
}
