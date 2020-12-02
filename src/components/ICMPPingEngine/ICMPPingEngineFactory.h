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

#ifndef NEDRYSOFT_ICMPPINGENGINE_ICMPPINGENGINEFACTORY_H
#define NEDRYSOFT_ICMPPINGENGINE_ICMPPINGENGINEFACTORY_H

#include "ComponentSystem/IInterface.h"
#include "Core/IPingEngineFactory.h"

#include <memory>

namespace Nedrysoft::ICMPPingEngine {

    class ICMPPingEngineFactoryData;

    class ICMPPingEngine;

    /**
     * @brief       Factory class for ICMPPingEngine
     *
     * @details     The factory class for creating instances of the ICMPPingEngine type
     *
     */
    class ICMPPingEngineFactory :
            public Nedrysoft::Core::IPingEngineFactory {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingEngineFactory)

        public:
            /**
             * @brief       Constructor
             *
             */
            ICMPPingEngineFactory();

            /**
             * @brief       Destructor
             *
             */
            ~ICMPPingEngineFactory();

            /**
             * @sa          IPingEngineFactory
             *
             */
            virtual Nedrysoft::Core::IPingEngine *createEngine(Nedrysoft::Core::IPVersion version);

            virtual QString description();

            /**
             * @sa          IConfiguration
             *
             */
            virtual QJsonObject saveConfiguration();

            virtual bool loadConfiguration(QJsonObject configuration);

        protected:
            std::shared_ptr<ICMPPingEngineFactoryData> d;
    };
}


#endif // NEDRYSOFT_ICMPPINGENGINE_ICMPPINGENGINEFACTORY_H
