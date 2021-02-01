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

#ifndef NEDRYSOFT_CORE_IPINGTARGET_H
#define NEDRYSOFT_CORE_IPINGTARGET_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"
#include "IConfiguration.h"

#include <QHostAddress>

namespace Nedrysoft::Core {
    class IPingEngine;

    /**
     * @brief       The IPingTarget interface describes a host target for a ping engine.
     *
     * @details     A ping target is used by an Nedrysoft::Core::IPingEngine to keep track of destinations to be pinged.
     */
    class NEDRYSOFT_CORE_DLLSPEC IPingTarget :
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
            virtual ~IPingTarget() = default;

            /**
             * @brief       Sets the target host address.
             *
             * @param[in]   hostAddress the host address to be pinged.
             */
            virtual auto setHostAddress(QHostAddress hostAddress) -> void = 0;

            /**
             * @brief       Returns the host address for this target.
             *
             * @returns     the host address for this target.
             */
            virtual auto hostAddress() -> QHostAddress = 0;

            /**
             * @brief       Returns the Nedrysoft::Core::IPingEngine that created this target
             *
             * @returns     the Nedrysoft::Core::IPingEngine instance.
             */
            virtual auto engine() -> IPingEngine * = 0;

            /**
             * @brief       Returns the user data attached to this target.
             *
             * @returns     the user data.
             */
            virtual auto userData() -> void * = 0;

            /**
             * @brief       Sets the user data attached to this target.
             *
             * @param[in]   data the user data.
             */
            virtual auto setUserData(void *data) -> void = 0;

            /**
             * @brief       Returns the TTL of this target.
             *
             * @returns     the ttl value.
             */
            virtual auto ttl() -> uint16_t = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IPingTarget, "com.nedrysoft.core.IPingTarget/1.0.0")

#endif // NEDRYSOFT_CORE_IPINGTARGET_H
