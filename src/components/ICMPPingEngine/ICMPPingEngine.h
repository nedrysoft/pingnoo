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

#ifndef PINGNOO_COMPONENTS_ICMPPINGENGINE_ICMPPINGENGINE_H
#define PINGNOO_COMPONENTS_ICMPPINGENGINE_ICMPPINGENGINE_H

#include <IInterface>
#include <IPingEngine>
#include <IPingEngineFactory>
#include <QElapsedTimer>
#include <QDateTime>
#include <memory>

namespace Nedrysoft { namespace ICMPPingEngine {
    class ICMPPingEngineData;
    class ICMPPingTransitter;
    class ICMPPingItem;

    /**
     * @brief       THe ICMPPingEngine provides a ICMP socket ping engine implementation.
     */
    class ICMPPingEngine :
            public Nedrysoft::RouteAnalyser::IPingEngine {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::RouteAnalyser::IPingEngine)

        public:
            /**
             * @brief       Constructs an ICMPPingEngine for the given IP version.
             */
            explicit ICMPPingEngine(Nedrysoft::Core::IPVersion version);

            /**
             * @brief       Destroys the ICMPPingEngine.
             */
            ~ICMPPingEngine();

            /**
             * @brief       Sets the measurement interval for this engine instance.
             *
             * @see         Nedrysoft::RouteAnalyser::IPingEngine::setInterval
             *
             * @param[in]   interval interval time in milliseconds.
             *
             * @returns     returns true on success; otherwise false.
             */
            auto setInterval(int interval) -> bool override;

            /**
             * @brief       Returns the interval set on the engine.
             *
             * @see         Nedrysoft::RouteAnalyser::IPingEngine::interval
             *
             * @returns     the interval time in milliseconds.
             */
            auto interval() -> int override;

            /**
             * @brief       Sets the reply timeout for this engine instance.
             *
             * @see         Nedrysoft::RouteAnalyser::IPingEngine::setTimeout
             *
             * @param[in]   timeout the number of milliseconds before we consider that the packet was lost.
             *
             * @returns     true on success; otherwise false.
             */
            auto setTimeout(int timeout) -> bool override;

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
             * @see         Nedrysoft::RouteAnalyser::IPingEngine::stop
             *
             * @returns     true on success; otherwise false.
             */
            auto stop() -> bool override;

            /**
             * @brief       Adds a ping target to this engine instance.
             *
             * @see         Nedrysoft::RouteAnalyser::IPingEngine::addTarget
             *
             * @param[in]   hostAddress the host address of the ping target.
             *
             * @returns     returns a pointer to the created ping target.
             */
            auto addTarget(QHostAddress hostAddress) -> Nedrysoft::RouteAnalyser::IPingTarget * override;

            /**
             * @brief       Adds a ping target to this engine instance.
             *
             * @see         Nedrysoft::RouteAnalyser::IPingEngine::addTarget
             *
             * @param[in]   hostAddress the host address of the ping target.
             * @param[in]   ttl the time to live to use.
             *
             * @returns     returns a pointer to the created ping target.
             */
            auto addTarget(QHostAddress hostAddress, int ttl) -> Nedrysoft::RouteAnalyser::IPingTarget * override;

            /**
             * @brief       Transmits a single ping.
             *
             * @note        This is a blocking function.
             *
             * @param[in]   hostAddress the target host address.
             * @param[in]   ttl time to live for this packet.
             * @param[in]   timeout time in seconds to wait for response.
             *
             * @returns     the result of the ping.
             */
            auto singleShot(
                QHostAddress hostAddress,
                int ttl,
                double timeout
            ) -> Nedrysoft::RouteAnalyser::PingResult override;

            /**
             * @brief       Removes a ping target from this engine instance.
             *
             * @see         Nedrysoft::RouteAnalyser::IPingEngine::addTarget
             *
             * @param[in]   target the ping target to remove.
             *
             * @returns     true on success; otherwise false.
             */
            auto removeTarget(Nedrysoft::RouteAnalyser::IPingTarget *target) -> bool override;

            /**
             * @brief       Gets the epoch for this engine instance.
             *
             * @see         Nedrysoft::RouteAnalyser::IPingEngine::epoch
             *
             * @returns     the time epoch.
             */
            auto epoch() -> QDateTime override;

            /**
             * @brief       Returns the list of ping targets for the engine.
             *
             * @returns     a QList containing the list of targets.
             */
            auto targets() -> QList<Nedrysoft::RouteAnalyser::IPingTarget *> override;

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
            /**
             * @brief       Called when a ICMP packet is available for processing.
             *
             * @param[in]   receiveTimer a timer started when the packet was received.
             * @param[in]   receiveBuffer the actual packet data.
             * @param[in]   receiveAddress the IP address that the response came from (may be different to target).
             */
            Q_SLOT void onPacketReceived(
                QElapsedTimer receiveTimer,
                QByteArray receiveBuffer,
                QHostAddress receiveAddress
            );

        protected:
            /**
             * @brief       Checks for any timed out requests and removes and signals that a timeout occurred.
             *
             * @see         Nedrysoft::ICMPPingEngine::ICMPPingTimeout
             */
            auto timeoutRequests(void) -> void;

            /**
             * @brief       Adds a ping request to the engine so it can be tracked.
             *
             * @details     Adds a ping request to the list of requests, the engine maintains a list of currently
             *              active requests and uses these to correlate responses and handle timeouts.
             *
             * @param[in]   pingItem the item being tracked.
             */
            auto addRequest(Nedrysoft::ICMPPingEngine::ICMPPingItem *pingItem) -> void;

            /**
             * @brief       Removes a tracked request and deletes the item.
             *
             * @details     Removes a tracked request and deletes the item, when a ping response (either an echo reply
             *              or ttl exceeded) is received the request can be removed from the engine.
             *
             * @param[in]   pingItem is the item to be removed.
             */
            auto removeRequest(Nedrysoft::ICMPPingEngine::ICMPPingItem *pingItem) -> void;

            /**
             * @brief       Returns a tracked request by id.
             *
             * @details     Finds the request by the id that was received in the packet, as the engine needs to
             *              figure out which response relates to a request to figure out the round trip time it uses
             *              this identifier which is constructed from the ICMP header in the following manner:
             *
             *                  (icmp_id<<16) | icmp_sequence_id
             *
             * @param[in]   id is the request to find.
             *
             * @returns     returns the request if found; nullptr otherwise.
             */
            auto getRequest(uint32_t id) -> Nedrysoft::ICMPPingEngine::ICMPPingItem *;

            /**
             * @brief       Sets the transmission epoch.
             *
             * @brief       Sets the transmission epoch, this is a timestamp that is used to calculate time difference
             *              when transmitting.
             *
             * @param[in]   epoch is the epoch.
             */
            auto setEpoch(QDateTime epoch) -> void;

            /**
             * @brief       Returns the IP version of the engine.
             *
             * @brief       Gets the IP version of the engine.  An engine should be created for a specific IP version,
             *              if multiple IP versions are required, then an engine for each version should be created.
             *
             * @returns     the IP version.
             */
            auto version() -> Nedrysoft::Core::IPVersion;

            /**
             * @brief       Stops all ping transmissions for this instance.
             *
             * @note        This controls the actual logic for stopping transmissions, it is called by the
             *              destructor and the stop() virtual function.  Virtual function should not be called
             *              by a destructor, so this acts as a shim.
             *
             * @returns     true if transmissions could be stopped; otherwise false.
             */
            auto doStop() -> bool;

            friend class ICMPPingTransmitter;
            friend class ICMPPingTimeout;
            friend class ICMPPingReceiverWorker;

        protected:
            //! @cond

            std::shared_ptr<ICMPPingEngineData> d;

            //! @endcond
    };
}}

#endif // PINGNOO_COMPONENTS_ICMPPINGENGINE_ICMPPINGENGINE_H
