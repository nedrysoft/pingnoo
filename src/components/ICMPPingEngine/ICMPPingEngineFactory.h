/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
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
     */
    class ICMPPingEngineFactory :
            public Nedrysoft::Core::IPingEngineFactory {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingEngineFactory)

        public:
            /**
             * @brief       Constructs an ICMPPingEngineFactory.
             */
            ICMPPingEngineFactory();

            /**
             * @brief       Constructs the ICMPPingEngineFactory.
             */
            ~ICMPPingEngineFactory();

        public:
            /**
             * @brief       Creates a ICMPAPIPingEngine instance.
             *
             * @see         Nedrysoft::Core::IPingEngineFactory::createEngine
             *
             * @param[in]   version the IP version of the engine.
             *
             * @returns     the new ICMPAPIPingEngine instance.
             */
            virtual auto createEngine(Nedrysoft::Core::IPVersion version) -> Nedrysoft::Core::IPingEngine *;

            /**
             * @brief       Returns the descriptive name of the factory.
             *
             * @returns     the descriptive name of the ping engine.
             */
            virtual auto description() -> QString;

        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @returns     the JSON configuration.
             */
            virtual auto saveConfiguration() -> QJsonObject;

            /**
             * @brief       Loads the configuration.
             *
             * @see         Nedrysoft::Core::IConfiguration::loadConfiguration
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            virtual auto loadConfiguration(QJsonObject configuration) -> bool;

        protected:
            std::shared_ptr<ICMPPingEngineFactoryData> d;
    };
}


#endif // NEDRYSOFT_ICMPPINGENGINE_ICMPPINGENGINEFACTORY_H
