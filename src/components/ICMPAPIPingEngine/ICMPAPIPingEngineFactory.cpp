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

class Nedrysoft::Pingnoo::ICMPAPIPingEngineFactoryData {

    public:
        ICMPAPIPingEngineFactoryData(Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory *parent) :
                m_pingEngineFactory(parent) {

        }

        friend class ICMPAPIPingEngineFactory;

    private:
        Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory *m_pingEngineFactory;

        QList<Nedrysoft::Pingnoo::ICMPAPIPingEngine *> m_engineList;
};

Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::ICMPAPIPingEngineFactory() :
        d(std::make_shared<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactoryData>(this)) {

}

Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::~ICMPAPIPingEngineFactory() {
    for (auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }

    delete d;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::asQObject() -> QObject * {
    return ( this );
}

auto Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::createEngine() -> Nedrysoft::Pingnoo::IPingEngine * {
    auto engineInstance = new Nedrysoft::Pingnoo::ICMPAPIPingEngine;

    d->m_engineList.append(engineInstance);

    return ( engineInstance );
}

auto Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::saveConfiguration() -> QJsonObject {
    return ( QJsonObject());
}

auto Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return ( false );
}
