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

#ifndef NEDRYSOFT_ROUTEENGINE_ROUTEENGINEFACTORY_H
#define NEDRYSOFT_ROUTEENGINE_ROUTEENGINEFACTORY_H

#include "ComponentSystem/IInterface.h"
#include "Core/IRouteEngineFactory.h"

#include <memory>

namespace Nedrysoft::RouteEngine {
    class RouteEngineFactoryData;

    class RouteEngine;

    /**
     * @brief       The RouteEngineFactory class provides a factory for the RouteEngine class.
     */
    class RouteEngineFactory :
            public Nedrysoft::Core::IRouteEngineFactory {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IRouteEngineFactory)

        public:
            /**
             * @brief       Constructs a new RouteEngineFactory.
             */
            RouteEngineFactory();

            /**
             * @brief       Destroys the RouteEngineFactory.
             */
            ~RouteEngineFactory();

        public:
            /**
             * @brief       Creates a route engine instance.
             *
             * @details     Creates and returns a route engine instance.  The instance is owned by the factory
             *              and is responsible for its life cycle.
             *
             * @returns     the route engine instance.
             */
            auto createEngine() -> Nedrysoft::Core::IRouteEngine * override;

            /**
             * @brief       Returns the descriptive name of the route engine.
             *
             * @brief       Returns the description of the route engine, used to display the name of the engine
             *              to the user.
             *
             * @returns     the descriptive name of the route engine.
             */
            auto description() -> QString override;

        /**
             * @brief       Priority of the route engine.  The priority is 0=lowest, 1=highest.  This allows
             *              the application to provide a default engine per platform.
             *
             * @returns     the priority.
             */
            auto priority() -> double override;

        private:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @returns     the JSON configuration.
             */
            auto saveConfiguration() -> QJsonObject override;

            /**
             * @brief       Loads the configuration.
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            auto loadConfiguration(QJsonObject configuration) -> bool override;

        protected:
            std::shared_ptr<RouteEngineFactoryData> d;
    };
}


#endif // NEDRYSOFT_ROUTEENGINE_ROUTEENGINEFACTORY_H
