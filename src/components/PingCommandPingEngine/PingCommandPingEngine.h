/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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
            auto setInterval(std::chrono::milliseconds interval) -> bool override;

            /**
             * @brief       Returns the measurement interval.
             *
             * @see         Nedrysoft::Core::IPingEngine::interval
             *
             * @returns     returns the measurement interval.
             */
            auto interval() -> std::chrono::milliseconds override;

            /**
             * @brief       Sets the reply timeout for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::setTimeout
             *
             * @param[in]   timeout the amount of time before we consider that the packet was lost.
             *
             * @returns     true on success; otherwise false.
             */
            auto setTimeout(std::chrono::milliseconds timeout) -> bool override;

            /**
             * @brief       Starts ping operations for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::start
             *
             * @returns     true on success; otherwise false.
             */
            auto start() -> bool override;

            /**
             * @brief       Stops ping operations for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::stop
             *
             * @returns     true on success; otherwise false.
             */
            auto stop() -> bool override;

            /**
             * @brief       Adds a ping target to this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::addTarget
             *
             * @param[in]   hostAddress the host address of the ping target.
             *
             * @returns     returns a pointer to the created ping target.
             */
            auto addTarget(QHostAddress hostAddress) -> Nedrysoft::Core::IPingTarget * override;

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
            auto addTarget(QHostAddress hostAddress, int ttl) -> Nedrysoft::Core::IPingTarget * override;

            /**
             * @brief       Removes a ping target from this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::addTarget
             *
             * @param[in]   target the ping target to remove.
             *
             * @returns     true on success; otherwise false.
             */
            auto removeTarget(Nedrysoft::Core::IPingTarget *target) -> bool override;

            /**
             * @brief       Gets the epoch for this engine instance.
             *
             * @see         Nedrysoft::Core::IPingEngine::epoch
             *
             * @returns     the time epoch.
             */
            auto epoch() -> std::chrono::system_clock::time_point override;

            /**
             * @brief       Returns the list of ping targets for the engine.
             *
             * @returns     a QList containing the list of targets.
             */
            auto targets() -> QList<Nedrysoft::Core::IPingTarget *> override;

        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @see         Nedrysoft::Core::IConfiguration::saveConfiguration
             *
             * @returns     the JSON configuration.
             */
            auto saveConfiguration() -> QJsonObject override;

            /**
             * @brief       Loads the configuration.
             *
             * @see         Nedrysoft::Core::IConfiguration::loadConfiguration
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            auto loadConfiguration(QJsonObject configuration) -> bool override;

        private:
            auto emitResult(Nedrysoft::Core::PingResult pingResult) -> void;

            friend class PingCommandPingTarget;

        private:
            QList<PingCommandPingTarget *> m_pingTargets;

            std::chrono::milliseconds m_interval;
    };
}

#endif // NEDRYSOFT_PINGCOMMANDPINGENGINE_PINGCOMMANDPINGENGINE_H
