/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 22/01/2020.
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

#include "CommandRouteEngineFactory.h"

#include "CommandRouteEngine.h"

class Nedrysoft::CommandRouteEngine::CommandRouteEngineFactoryData {
    public:
        CommandRouteEngineFactoryData(Nedrysoft::CommandRouteEngine::CommandRouteEngineFactory *parent) :
                m_factory(parent) {

        }

        friend class CommandRouteEngineFactory;

    private:
        Nedrysoft::CommandRouteEngine::CommandRouteEngineFactory *m_factory;

        QList<Nedrysoft::CommandRouteEngine::CommandRouteEngine *> m_engineList;
};

Nedrysoft::CommandRouteEngine::CommandRouteEngineFactory::CommandRouteEngineFactory() :
        d(std::make_shared<Nedrysoft::CommandRouteEngine::CommandRouteEngineFactoryData>(this)) {

}

Nedrysoft::CommandRouteEngine::CommandRouteEngineFactory::~CommandRouteEngineFactory() {
    for (auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }
}

auto Nedrysoft::CommandRouteEngine::CommandRouteEngineFactory::createEngine() -> Nedrysoft::Core::IRouteEngine * {
    auto engineInstance = new Nedrysoft::CommandRouteEngine::CommandRouteEngine();

    d->m_engineList.append(engineInstance);

    return engineInstance;
}

auto Nedrysoft::CommandRouteEngine::CommandRouteEngineFactory::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::CommandRouteEngine::CommandRouteEngineFactory::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}

auto Nedrysoft::CommandRouteEngine::CommandRouteEngineFactory::description() -> QString {
    return tr("Ping Command Route Engine");
}

