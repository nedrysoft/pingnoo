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

#ifndef NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINE_H
#define NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINE_H

#include "ICMPAPIPingEngineSpec.h"
#include "Core/IPingEngine.h"

namespace Nedrysoft::Pingnoo {
    class ICMPAPIPingEngineData;

    class ICMPPingItem;

    /**
     * @brief       The ICMPAPIPingEngine provides An IPingEngine that uses the Windows ICMP API feature.
     */
    class ICMPAPIPingEngine :
            public Nedrysoft::Core::IPingEngine {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingEngine)

        public:
            /**
             * @brief       Constructs an ICMPAPIPingEngine.
             */
            ICMPAPIPingEngine();

            /**
             * @brief       Sets the measurement interval for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::setInterval
             *
             * @param[in]   interval interval time.
             *
             * @returns     returns true on success; otherwise false.
             */
            virtual auto setInterval(std::chrono::milliseconds interval) -> bool;

            /**
             * @brief       Sets the reply timeout for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::setTimeout
             *
             * @param[in]   timeout the amount of time before we consider that the packet was lost.
             *
             * @returns     true on success; otherwise false.
             */
            virtual auto setTimeout(std::chrono::milliseconds timeout) -> bool;

            /**
             * @brief       Starts ping operations for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::start
             *
             * @returns     true on success; otherwise false.
             */
            virtual auto start() -> bool;

            /**
             * @brief       Stops ping operations for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::stop
             *
             * @returns     true on success; otherwise false.
             */
            virtual auto stop() -> bool;

            /**
             * @brief       Adds a ping target to this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::addTarget
             *
             * @param[in]   hostAddress the host address of the ping target.
             *
             * @returns     returns a pointer to the created ping target.
             */
            virtual auto addTarget(QHostAddress hostAddress) -> Nedrysoft::Core::IPingTarget *;

            /**
             * @brief       Adds a ping target to this engine instance
             *
             * @see         Nedrysoft::Core::IPingEngine::addTarget
             *
             * @param[in]   hostAddress the host address of the ping target
             * @param[in]   ttl the time to live to use
             *
             * @returns     returns a pointer to the created ping target
             */
            virtual auto addTarget(QHostAddress hostAddress, int ttl) -> Nedrysoft::Core::IPingTarget *;

            /**
             * @brief       Removes a ping target from this engine instance
             *
             * @see         Nedrysoft::Core::IPingEngine::addTarget
             *
             * @param[in]   target the ping target to remove
             *
             * @returns     true on success; otherwise false.
             */
            virtual auto removeTarget(Nedrysoft::Core::IPingTarget *target) -> bool;

            /**
             * @brief       Gets the epoch for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::epoch
             *
             * @returns     the time epoch
             */
            virtual auto epoch() -> std::chrono::system_clock::time_point;

        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @see         Nedrysoft::Core::IConfiguration::saveConfiguration
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

        protected:
            std::shared_ptr<ICMPAPIPingEngineData> d;
    };
}

#endif // NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINE_H
