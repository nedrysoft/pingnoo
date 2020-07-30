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

#ifndef FIZZYADE_CORE_IPINGENGINE_H
#define FIZZYADE_CORE_IPINGENGINE_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "IConfiguration.h"
#include "PingResult.h"
#include <chrono>
#include <QHostAddress>

namespace FizzyAde::Core
{
    class IPingTarget;

    /**
     * Interface definition of a ping engine
     *
     * An engine implements the logic of transmitting, receiving
     * and associating replies to ping requests, it then signals
     * when a ping result is available
     *
     */

    class FIZZYADE_CORE_DLLSPEC IPingEngine :
        public FizzyAde::ComponentSystem::IInterface,
        public FizzyAde::Core::IConfiguration
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::ComponentSystem::IInterface)
        Q_INTERFACES(FizzyAde::Core::IConfiguration)

    public:
        virtual ~IPingEngine() {}

        /**
         * Sets the measurement interval for this engine instance
         *
         * @param[in]   interval        interval time
         *
         * @return      returns true on success, else false
         *
         */
        virtual bool setInterval(std::chrono::milliseconds interval) = 0;

        /**
         * Sets the reply timeout for this engine instance
         *
         * @param[in]   timeout         timeout time
         *
         * @return      true on success, else false
         *
         */
        virtual bool setTimeout(std::chrono::milliseconds timeout) = 0;

        /**
         * Starts ping operations for this engine instance
         *
         * @return      true on success, else false
         *
         */
        virtual bool start() = 0;

        /**
         * Stops ping operations for this engine instance
         *
         * @return      true on success, else false
         *
         */
        virtual bool stop() = 0;

        /**
         * Adds a ping target to this engine instance
         *
         * @param[in]   hostAddress     the host address of the ping target
         *
         * @return      returns a pointer to the created ping target
         *
         */
        virtual IPingTarget *addTarget(QHostAddress hostAddress) = 0;

        /**
         * Adds a ping target to this engine instance
         *
         * @param[in]   hostAddress     the host address of the ping target
         * @param[in]   ttl             the time to live to use
         *
         * @return returns a pointer to the created ping target
         *
         */
        virtual IPingTarget *addTarget(QHostAddress hostAddress, int ttl) = 0;

        /**
         * Removes a ping target from this engine instance
         *
         * @param[in]   target          the ping target to remove
         *
         * @return      true on success, else false
         *
         */
        virtual bool removeTarget(IPingTarget *target) = 0;

        /**
         * Returns the epoch
         *
         * @return      the time epoch
         *
         */
        virtual std::chrono::system_clock::time_point epoch() = 0;

        /**
         * Signal emitted to indicate the state of a ping request
         *
         * @param[in]   result          the result of a ping request
         *
         */
        Q_SIGNAL void result(FizzyAde::Core::PingResult result);
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IPingEngine, "com.fizzyade.core.IPingEngine/1.0.0")

#endif // FIZZYADE_CORE_IPINGENGINE_H
