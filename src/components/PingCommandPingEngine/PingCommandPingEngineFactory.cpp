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

#include "PingCommandPingEngineFactory.h"

#include "PingCommandPingEngine.h"

Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::PingCommandPingEngineFactory() {

}

Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::~PingCommandPingEngineFactory() {

}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::createEngine(
        Nedrysoft::Core::IPVersion version ) -> Nedrysoft::Core::IPingEngine * {

    auto engineInstance = new Nedrysoft::PingCommandPingEngine::PingCommandPingEngine(version);

    return engineInstance;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::description() -> QString {
    return tr("Ping Executable");
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::priority() -> double {
#if defined(Q_OS_LINUX)
    return 0.1;
#else
    return 0;
#endif
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::available() -> bool {
#if defined(Q_OS_LINUX)
    return true;
#endif
    return false;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngineFactory::deleteEngine(
        Nedrysoft::Core::IPingEngine *engine) -> bool {

    return true;
}
