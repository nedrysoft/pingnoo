/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
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

#include "CommandPingEngine.h"

#include <QMap>
#include <QMutex>
#include <QThread>
#include <chrono>
#include <cstdint>

using namespace std::chrono_literals;

constexpr auto DefaultReceiveTimeout = 3s;
constexpr auto DefaultTerminateThreadTimeout = 5s;

Nedrysoft::CommandPingEngine::CommandPingEngine::CommandPingEngine(Nedrysoft::Core::IPVersion version) {

}

Nedrysoft::CommandPingEngine::CommandPingEngine::~CommandPingEngine() {
}

auto Nedrysoft::CommandPingEngine::CommandPingEngine::addTarget(QHostAddress hostAddress) -> Nedrysoft::Core::IPingTarget * {
}

auto Nedrysoft::CommandPingEngine::CommandPingEngine::addTarget(
        QHostAddress hostAddress,
        int ttl ) -> Nedrysoft::Core::IPingTarget * {

}

auto Nedrysoft::CommandPingEngine::CommandPingEngine::removeTarget(Nedrysoft::Core::IPingTarget *target) -> bool {
    Q_UNUSED(target)

    return true;
}

auto Nedrysoft::CommandPingEngine::CommandPingEngine::start() -> bool {
    return true;
}

auto Nedrysoft::CommandPingEngine::CommandPingEngine::stop() -> bool {
    return true;
}
