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

#ifndef NEDRYSOFT_CORE_IPINGENGINEFACTORY_H
#define NEDRYSOFT_CORE_IPINGENGINEFACTORY_H

#include "ComponentSystem/IInterface.h"
#include "Core.h"
#include "CoreSpec.h"
#include "IConfiguration.h"

namespace Nedrysoft::Core {
    class IPingEngine;

    /**
     * @brief       The IPingEngineFactory Interface describes a factory for Nedrysoft::Core::IPingEngine instances.
     *
     * @details     The ping engine factory is responsible for creating instances of ping engines.
     */
    class NEDRYSOFT_CORE_DLLSPEC IPingEngineFactory :
            public Nedrysoft::ComponentSystem::IInterface,
            public Nedrysoft::Core::IConfiguration {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)
            Q_INTERFACES(Nedrysoft::Core::IConfiguration)

        public:
            /**
             * @brief       Destroys the IPingEngineFactory.
             */
            virtual ~IPingEngineFactory() = default;

            /**
             * @brief       Creates a IPingEngine instance.
             *
             * @param[in]   version the IP version of the engine.
             *
             * @returns     the new Nedrysoft::Core::IPingEngine instance.
             */
            virtual Nedrysoft::Core::IPingEngine *createEngine(Nedrysoft::Core::IPVersion version) = 0;

            /**
             * @brief       Returns the descriptive name of the factory.
             *
             * @returns     the descriptive name of the ping engine.
             */
            virtual QString description() = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IPingEngineFactory, "com.nedrysoft.core.IPingEngineFactory/1.0.0")

#endif // NEDRYSOFT_CORE_IPINGENGINEFACTORY_H
