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

#ifndef FIZZYADE_PINGNOO_ICMPAPIPINGENGINEFACTORY_H
#define FIZZYADE_PINGNOO_ICMPAPIPINGENGINEFACTORY_H

#include "ICMPAPIPingEngineSpec.h"
#include "Core/IPingEngineFactory.h"
#include "ComponentSystem/IInterface.h"

namespace FizzyAde::Pingnoo {

    class ICMPAPIPingEngineFactoryData;
    class ICMPPingEngine;

    /**
     * Factory class for ICMPPingEngine
     *
     * Creates instances of ICMPPingEngine
     *
     */

    class ICMPAPIPingEngineFactory : public QObject, public FizzyAde::Core::IPingEngineFactory
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IPingEngineFactory)

    public:
        ICMPAPIPingEngineFactory();
        ~ICMPAPIPingEngineFactory();

        /**
         * @sa IPingEngineFactory
         */
        virtual FizzyAde::Core::IPingEngine *createEngine();

        /**
         * @sa IConfiguration
         */
        virtual QJsonObject saveConfiguration();
        virtual bool loadConfiguration(QJsonObject configuration);

    protected:
        std::shared_ptr<ICMPAPIAPIPingEngineFactoryData> d;
    };
}


#endif // FIZZYADE_PINGNOO_ICMPAPIPINGENGINEFACTORY_H
