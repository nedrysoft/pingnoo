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

#ifndef FIZZYADE_CORE_IROUTEENGINEFACTORY_H
#define FIZZYADE_CORE_IROUTEENGINEFACTORY_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "Core/Core.h"
#include "IConfiguration.h"

namespace FizzyAde::Core
{
    class IRouteEngine;

    /**
     * @brief       Interface definition of a route engine
     *
     * @details     An engine implements the logic of determining the route
     *              to a target host.
     *
     */
    class FIZZYADE_CORE_DLLSPEC IRouteEngineFactory :
        public FizzyAde::ComponentSystem::IInterface,
        public FizzyAde::Core::IConfiguration
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::ComponentSystem::IInterface)
        Q_INTERFACES(FizzyAde::Core::IConfiguration)

    public:
        /**
          * @brief       Constructor
          *
          */
        virtual ~IRouteEngineFactory() = default;

        /**
         * @brief       Creates a a route engine instance
         *
         * @details     Creates and returns a IRouteEngine instance that this engine
         *              manages.
         *
         * @return      new instance of the route engine
         *
         */
        virtual FizzyAde::Core::IRouteEngine *createEngine() = 0;

        /**
         * @brief       Returns the description of the route engine
         *
         * @return      descriptive name of the route engine
         *
         */
        virtual QString description() = 0;
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IRouteEngineFactory, "com.fizzyade.core.IRouteEngineFactory/1.0.0")

#endif // FIZZYADE_CORE_IROUTEENGINEFACTORY_H
