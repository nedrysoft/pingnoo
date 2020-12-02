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

#ifndef NEDRYSOFT_CORE_IROUTEENGINEFACTORY_H
#define NEDRYSOFT_CORE_IROUTEENGINEFACTORY_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "Core/Core.h"
#include "IConfiguration.h"

namespace Nedrysoft::Core {
    class IRouteEngine;

    /**
     * @brief       Interface definition of a route engine factory
     *
     * @details     Creates instance of route engines.  Route engines are not instantiaed directly, instead the application finds all route engine factories
     *              and uses the selected route engine factory to create route engines.
     */
    class NEDRYSOFT_CORE_DLLSPEC IRouteEngineFactory :
            public Nedrysoft::ComponentSystem::IInterface,
            public Nedrysoft::Core::IConfiguration {
        Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)
            Q_INTERFACES(Nedrysoft::Core::IConfiguration)

        public:
            /**
             * @brief       Constructs a route engine factory.
             *
             * @details     Constructs a route engine factory.
             */
            virtual ~IRouteEngineFactory() = default;

            /**
             * @brief       Creates a route engine instance.
             *
             * @details     Creates and returns a route engine instance.  The instance is owned by the factory itself and is responsible for its life cycle.
             *
             * @return      new instance of the route engine,
             */
            virtual Nedrysoft::Core::IRouteEngine *createEngine() = 0;

            /**
             * @brief       Gets the description of the route engine.
             *
             * @brief       Gets the description of the route engine, used to display the name of the enine to the user.
             *
             * @return      descriptive name of the route engine.
             */
            virtual QString description() = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IRouteEngineFactory, "com.fizzyade.core.IRouteEngineFactory/1.0.0")

#endif // NEDRYSOFT_CORE_IROUTEENGINEFACTORY_H
