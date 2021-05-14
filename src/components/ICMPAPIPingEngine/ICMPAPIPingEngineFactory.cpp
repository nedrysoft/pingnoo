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

#include "ICMPAPIPingEngineFactory.h"

#include "ICMPAPIPingEngine.h"

class Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactoryData {

    public:
        ICMPAPIPingEngineFactoryData(Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory *parent) :
                m_pingEngineFactory(parent) {

        }

        friend class ICMPAPIPingEngineFactory;

    private:
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory *m_pingEngineFactory;

        QList<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *> m_engineList;
};

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::ICMPAPIPingEngineFactory() :
        d(std::make_shared<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactoryData>(this)) {

}

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::~ICMPAPIPingEngineFactory() {
    for (auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }

    d.reset();
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::createEngine(
        Nedrysoft::Core::IPVersion ipVersion) -> Nedrysoft::RouteAnalyser::IPingEngine * {

    auto engineInstance = new Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine(ipVersion);

    d->m_engineList.append(engineInstance);

    return engineInstance;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::description() -> QString {
    return tr("ICMPAPI");
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::priority() -> double {
    return 1;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::available() -> bool {
    return true;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::saveConfiguration() -> QJsonObject {
    return ( QJsonObject());
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return ( false );
}
auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory::deleteEngine(
        Nedrysoft::RouteAnalyser::IPingEngine *engine) -> bool {

    auto pingEngine = qobject_cast<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *>(engine);

    if (d->m_engineList.contains(pingEngine)) {
        engine->stop();
        d->m_engineList.removeAll(pingEngine);
        pingEngine->deleteLater();
    }

    return true;
}