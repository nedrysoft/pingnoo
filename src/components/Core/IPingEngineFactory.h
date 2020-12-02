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

#ifndef NEDRYSOFT_CORE_IPINGENGINEFACTORY_H
#define NEDRYSOFT_CORE_IPINGENGINEFACTORY_H

#include "ComponentSystem/IInterface.h"
#include "Core.h"
#include "CoreSpec.h"
#include "IConfiguration.h"

namespace Nedrysoft::Core {
    class IPingEngine;

    /**
     * @brief       Interface definition of a ping engine
     *
     * @details     An engine implements the logic of transmitting, receiving
     *              and associating replies to ping requests, it then signals
     *              when a ping result is available
     *
     */
    class NEDRYSOFT_CORE_DLLSPEC IPingEngineFactory :
            public Nedrysoft::ComponentSystem::IInterface,
            public Nedrysoft::Core::IConfiguration {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)
            Q_INTERFACES(Nedrysoft::Core::IConfiguration)

        public:
            virtual ~IPingEngineFactory() = default;

            /**
             * @brief       Creates a IPingEngine instance
             *
             * @param[in]   version     the IP version of the engine
             *
             * @return      the newly created ping engine instance
             *
             */
            virtual Nedrysoft::Core::IPingEngine *createEngine(Nedrysoft::Core::IPVersion version) = 0;

            /**
             * @brief       Gets the descriptive name of the ping engine factory
             *
             * @return      descriptive name of the ping engine
             *
             */
            virtual QString description() = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IPingEngineFactory, "com.nedrysoft.core.IPingEngineFactory/1.0.0")

#endif // NEDRYSOFT_CORE_IPINGENGINEFACTORY_H
