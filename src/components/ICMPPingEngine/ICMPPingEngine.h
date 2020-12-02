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

#ifndef NEDRYSOFT_ICMPPINGENGINE_ICMPPINGENGINE_H
#define NEDRYSOFT_ICMPPINGENGINE_ICMPPINGENGINE_H

#include "Core/IPingEngine.h"
#include "Core/IPingEngineFactory.h"
#include "ComponentSystem/IInterface.h"

namespace Nedrysoft::ICMPPingEngine {
    class ICMPPingEngineData;

    class ICMPPingItem;

    /**
     * @brief       IPingEngine implementation for ICMP
     *
     * @details     Implements the IPingEngine interface to implement a ping engine
     *              that uses ICMP echo packets for measurements.
     *
     * @see         Nedrysoft::ICMPPingEngine::ICCMPPingEngineFactory, Nedrysoft::ICMPPingEngine::ICMPPingEngineItem,
     *              Nedrysoft::ICMPPingEngine::ICMPPingEngineReceiver, Nedrysoft::ICMPPingEngine::ICMPPingEngine,
     *              Nedrysoft::ICMPPingEngine::ICMPTarget, Nedrysoft::ICMPPingEngine::ICMPPingTarget,
     *              Nedrysoft::ICMPPingEngine::ICMPPingEngineTransmitter
     */
    class ICMPPingEngine :
            public Nedrysoft::Core::IPingEngine {
        Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingEngine)

        public:
            /**
             * @brief       Constructor for ping engine.
             *
             * @details     Constructor for ping engine.
             *
             * @param[in]   version is the version of IP to use, Nedrysoft::Core::IPVersion::IPv4 or Nedrysoft::Core::IPVersion::IPv6.
             */
            ICMPPingEngine(Nedrysoft::Core::IPVersion version);

            /**
             * @brief       Destroys the ping engine.
             *
             * @copydoc     Destroys the ping engine.
             */
            ~ICMPPingEngine();

            /**
             * @copydoc     Nedrysoft::Core::IPingEngine
             */
            virtual bool setInterval(std::chrono::milliseconds interval);

            /**
             * @copydoc     Nedrysoft::Core::IPingEngine
             */
            virtual bool setTimeout(std::chrono::milliseconds timeout);

            /**
             * @copydoc     Nedrysoft::Core::IPingEngine
             */
            virtual bool start();

            /**
             * @copydoc     Nedrysoft::Core::IPingEngine
             */
            virtual bool stop();

            /**
             * @copydoc     Nedrysoft::Core::IPingEngine
             */
            virtual Nedrysoft::Core::IPingTarget *addTarget(QHostAddress hostAddress);

            /**
             * @copydoc     Nedrysoft::Core::IPingEngine
             */
            virtual Nedrysoft::Core::IPingTarget *addTarget(QHostAddress hostAddress, int ttl);

            /**
             * @copydoc     Nedrysoft::Core::IPingEngine
             */
            virtual bool removeTarget(Nedrysoft::Core::IPingTarget *target);

            /**
             * @copydoc     Nedrysoft::Core::IPingEngine
             */
            virtual std::chrono::system_clock::time_point epoch();

            /**
             * @copydoc     Nedrysoft::Core::IConfiguration
             */
            virtual QJsonObject saveConfiguration();

            /**
             * @copydoc     Nedrysoft::Core::IConfiguration
             */
            virtual bool loadConfiguration(QJsonObject configuration);

        protected:
            /**
             * @brief       Checks for any timed out requests and removes and notfiies about their removal.
             *
             * @details     Checks for any timed out requests and removes and notfiies about their removal.  This function is called from
             *              the timeout thread.
             *
             * @see         Nedrysoft::ICMPPingEngine::ICMPPingTimeout
             */
            void timeoutRequests(void);

            /**
             * @brief       Adds a ping request to the engine so it can be tracked.
             *
             * @details     Adds a ping request to the list of requests, the engine maintains a list of currently active requests and uses these
             *              to correlate responses and handle timeouts.
             *
             * @param[in]   pingItem is the item being tracked
             */
            void addRequest(Nedrysoft::ICMPPingEngine::ICMPPingItem *pingItem);

            /**
             * @brief       Removes a tracked request and deletes the item.
             *
             * @details     Removes a tracked request and deletes the item, when a ping response (either an echo reply or ttl exceeded) is received
             *              the request can be removed from the engine.
             *
             * @param[in]   pingItem is the item to be removed.
             */
            void removeRequest(Nedrysoft::ICMPPingEngine::ICMPPingItem *pingItem);

            /**
             * @brief       Gets a tracked request by id.
             *
             * @details     Finds the request by the id that was received in the packet, as the engine needs to figure out which response relates
             *              to a request to figure out the round trip time it uses this identifer which is constructed from the ICMP header
             *              in the following manner:
             *
             *                  (icmp_id<<16) | icmp_sequence_id
             *
             * @param[in]   id is the request to find.
             *
             * @return      returns the request if found; nullptr otherwise.
             */
            Nedrysoft::ICMPPingEngine::ICMPPingItem *getRequest(uint32_t id);

            /**
             * @brief       Sets the transmission epoch.
             *
             * @brief       Sets the transmission epoch, this is a timestamp that is used to calculate time difference when transmitting.
             *
             * @param[in]   epoch is the epoch.
             */
            void setEpoch(std::chrono::system_clock::time_point epoch);

            /**
             * @brief       Gets the IP version of the engine.
             *
             * @brief       Gets the IP version of the engine.  An engine should be created for a specific IP version, if multiple IP versions
             *              are required, then an engine for each version should be created.
             *
             * @return      the IP version.
             */
            Nedrysoft::Core::IPVersion version();

            friend class ICMPPingReceiver;

            friend class ICMPPingTransmitter;

            friend class ICMPPingTimeout;

        protected:
            std::shared_ptr<ICMPPingEngineData> d;
    };
}

#endif // NEDRYSOFT_ICMPPINGENGINE_ICMPPINGENGINE_H
