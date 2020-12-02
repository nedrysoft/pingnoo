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

QObject *Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::asQObject() {
    return ( this );
}

Nedrysoft::Pingnoo::IPingEngine *Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::createEngine() {
    auto engineInstance = new Nedrysoft::Pingnoo::ICMPAPIPingEngine;

    d->m_engineList.append(engineInstance);

    return ( engineInstance );
}

QJsonObject Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::saveConfiguration() {
    return ( QJsonObject());
}

bool Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::loadConfiguration(QJsonObject configuration) {
    Q_UNUSED(configuration)

    return ( false );
}
