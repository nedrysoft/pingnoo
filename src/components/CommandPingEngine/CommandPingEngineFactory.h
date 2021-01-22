/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 22/01/2021.
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

#ifndef NEDRYSOFT_COMMANDPINGENGINE_COMMANDPINGENGINEFACTORY_H
#define NEDRYSOFT_COMMANDPINGENGINE_COMMANDPINGENGINEFACTORY_H

#include "ComponentSystem/IInterface.h"
#include "Core/IPingEngineFactory.h"

#include <memory>

namespace Nedrysoft::CommandPingEngine {
    class CommandPingEngine;

    /**
     * @brief       Factory class for CommandPingEngine
     *
     * @details     The factory class for creating instances of the CommandPingEngine type
     */
    class CommandPingEngineFactory :
            public Nedrysoft::Core::IPingEngineFactory {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingEngineFactory)

        public:
            /**
             * @brief       Constructs an CommandPingEngineFactory.
             */
            CommandPingEngineFactory();

            /**
             * @brief       Constructs the CommandPingEngineFactory.
             */
            ~CommandPingEngineFactory();

        public:
            /**
             * @brief       Creates a CommandPingEngine instance.
             *
             * @see         Nedrysoft::Core::IPingEngineFactory::createEngine
             *
             * @param[in]   version the IP version of the engine.
             *
             * @returns     the new CommandPingEngine instance.
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
    };
}


#endif // NEDRYSOFT_COMMANDPINGENGINE_COMMANDINGENGINEFACTORY_H
