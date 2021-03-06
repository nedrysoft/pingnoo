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

#ifndef NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINEFACTORY_H
#define NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINEFACTORY_H

#include "ComponentSystem/IInterface.h"
#include "Core/IPingEngineFactory.h"
#include "ICMPAPIPingEngineSpec.h"

namespace Nedrysoft::Pingnoo {

    class ICMPAPIPingEngineFactoryData;

    class ICMPPingEngine;

    /**
     * @brief       The ICMPAPIPingEngineFactory creates instances of the windows ICMPAPI ping engine.
     */
    class ICMPAPIPingEngineFactory :
            public Nedrysoft::Core::IPingEngineFactory {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingEngineFactory)

        public:
            /**
             * @brief       Constructs an ICMPAPIPingEngineFactory.
             */
            ICMPAPIPingEngineFactory();

            /**
             * @brief       Destroys the ICMPAPIPingEngineFactory.
             */
            ~ICMPAPIPingEngineFactory();

        public:
            /**
             * @brief       Creates a ICMPAPIPingEngine instance.
             *
             * @see         Nedrysoft::Core::IPingEngineFactory::createEngine.
             *
             * @param[in]   version the IP version of the engine.
             *
             * @returns     the new ICMPAPIPingEngine instance.
             */
            auto createEngine() -> Nedrysoft::Core::IPingEngine * override;

        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @returns     the JSON configuration.
             */
            auto saveConfiguration() -> QJsonObject override;

            /**
             * @brief       Loads the configuration.
             *
             * @see         Nedrysoft::Core::IConfiguration::loadConfiguration
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            auto loadConfiguration(QJsonObject configuration) -> bool override;

        protected:
            std::shared_ptr<ICMPAPIAPIPingEngineFactoryData> d;
    };
}


#endif // NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINEFACTORY_H
