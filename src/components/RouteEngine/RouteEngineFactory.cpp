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

#include "RouteEngineFactory.h"

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

Nedrysoft::Core::IRouteEngine *Nedrysoft::RouteEngine::RouteEngineFactory::createEngine() {
    auto engineInstance = new Nedrysoft::RouteEngine::RouteEngine();

    d->m_engineList.append(engineInstance);

    return engineInstance;
}

QJsonObject Nedrysoft::RouteEngine::RouteEngineFactory::saveConfiguration() {
    return QJsonObject();
}

bool Nedrysoft::RouteEngine::RouteEngineFactory::loadConfiguration(QJsonObject configuration) {
    Q_UNUSED(configuration)

    return false;
}

QString Nedrysoft::RouteEngine::RouteEngineFactory::description() {
    return tr("Route Engine");
}

