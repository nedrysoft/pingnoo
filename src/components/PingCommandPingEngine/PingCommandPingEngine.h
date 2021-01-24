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

#ifndef NEDRYSOFT_PINGCOMMANDPINGENGINE_PINGCOMMANDPINGENGINE_H
#define NEDRYSOFT_PINGCOMMANDPINGENGINE_PINGCOMMANDPINGENGINE_H

#include "ComponentSystem/IInterface.h"
#include "Core/IPingEngine.h"
#include "Core/IPingEngineFactory.h"

#include <chrono>

namespace Nedrysoft::PingCommandPingEngine {
    class PingCommandPingTarget;

    /**
     * @brief       THe PingCommandPingEngine provides a command based ping engine implementation.
     */
    class PingCommandPingEngine :
            public Nedrysoft::Core::IPingEngine {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingEngine)

        public:
            /**
             * @brief       Constructs an PingCommandPingEngine for the given IP version.
             */
            PingCommandPingEngine(Nedrysoft::Core::IPVersion version);

            /**
             * @brief       Destroys the PingCommandPingEngine.
             */
            ~PingCommandPingEngine();

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
             * @brief       Returns the measurement interval.
             *
             * @see         Nedrysoft::Core::IPingEngine::interval
             *
             * @returns     returns the measurement interval.
             */
            virtual auto interval() -> std::chrono::milliseconds;

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
             * @brief       Adds a ping target to this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::addTarget
             *
             * @param[in]   hostAddress the host address of the ping target.
             * @param[in]   ttl the time to live to use.
             *
             * @returns     returns a pointer to the created ping target.
             */
            virtual auto addTarget(QHostAddress hostAddress, int ttl) -> Nedrysoft::Core::IPingTarget *;

            /**
             * @brief       Removes a ping target from this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::addTarget
             *
             * @param[in]   target the ping target to remove.
             *
             * @returns     true on success; otherwise false.
             */
            virtual auto removeTarget(Nedrysoft::Core::IPingTarget *target) -> bool;

            /**
             * @brief       Gets the epoch for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::epoch
             *
             * @returns     the time epoch.
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

        private:
            auto emitResult(Nedrysoft::Core::PingResult pingResult) -> void;

            friend class PingCommandPingTarget;

        private:
            QList<PingCommandPingTarget *> m_pingTargets;

            std::chrono::milliseconds m_interval;
    };
}

#endif // NEDRYSOFT_PINGCOMMANDPINGENGINE_PINGCOMMANDPINGENGINE_H
