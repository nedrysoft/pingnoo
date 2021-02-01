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

#include "RouteEngineFactory.h"

#include "ICMPSocket/ICMPSocket.h"
#include "RouteEngine.h"

class Nedrysoft::RouteEngine::RouteEngineFactoryData {
    public:
        RouteEngineFactoryData(Nedrysoft::RouteEngine::RouteEngineFactory *parent) :
                m_factory(parent) {

        }

        friend class RouteEngineFactory;

    private:
        Nedrysoft::RouteEngine::RouteEngineFactory *m_factory;

        QList<Nedrysoft::RouteEngine::RouteEngine *> m_engineList;
};

Nedrysoft::RouteEngine::RouteEngineFactory::RouteEngineFactory() :
        d(std::make_shared<Nedrysoft::RouteEngine::RouteEngineFactoryData>(this)) {

}

Nedrysoft::RouteEngine::RouteEngineFactory::~RouteEngineFactory() {
    for (auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }
}

auto Nedrysoft::RouteEngine::RouteEngineFactory::createEngine() -> Nedrysoft::Core::IRouteEngine * {
    auto engineInstance = new Nedrysoft::RouteEngine::RouteEngine();

    d->m_engineList.append(engineInstance);

    return engineInstance;
}

auto Nedrysoft::RouteEngine::RouteEngineFactory::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::RouteEngine::RouteEngineFactory::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}

auto Nedrysoft::RouteEngine::RouteEngineFactory::description() -> QString {
    return tr("Route Engine");
}

auto Nedrysoft::RouteEngine::RouteEngineFactory::priority() -> double {
#if defined(Q_OS_LINUX)
    auto socket = Nedrysoft::ICMPSocket::ICMPSocket::createReadSocket(Nedrysoft::ICMPSocket::V4);

    if (socket) {
        delete socket;

        return 1;
    }

    return 0;
#endif
    return 1;
}


