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

#ifndef FIZZYADE_CORE_IPINGENGINEFACTORY_H
#define FIZZYADE_CORE_IPINGENGINEFACTORY_H

#include "Core.h"
#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "IConfiguration.h"

namespace FizzyAde::Core
{
    class IPingEngine;

    /**
     * Interface definition of a ping engine
     *
     * An engine implements the logic of transmitting, receiving
     * and associating replies to ping requests, it then signals
     * when a ping result is available
     *
     */

    class FIZZYADE_CORE_DLLSPEC IPingEngineFactory :
        public FizzyAde::ComponentSystem::IInterface,
        public FizzyAde::Core::IConfiguration
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::ComponentSystem::IInterface)
        Q_INTERFACES(FizzyAde::Core::IConfiguration)

    public:
        virtual ~IPingEngineFactory() {}

        /**
         * Creates a IPingEngine instance
         *
         * @return returns instance
         *
         */
        virtual FizzyAde::Core::IPingEngine *createEngine(FizzyAde::Core::IPVersion version) = 0;

        /**
         * Returns the description of the ping engine
         *
         * @return description
         *
         */
        virtual QString description() = 0;
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IPingEngineFactory, "com.fizzyade.core.IPingEngineFactory/1.0.0")

#endif // FIZZYADE_CORE_IPINGENGINEFACTORY_H
