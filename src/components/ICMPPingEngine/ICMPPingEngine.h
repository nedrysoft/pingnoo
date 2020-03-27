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

#ifndef FIZZYADE_ICMPPINGENGINE_ICMPPINGENGINE_H
#define FIZZYADE_ICMPPINGENGINE_ICMPPINGENGINE_H

#include "Core/IPingEngine.h"
#include "Core/IPingEngineFactory.h"
#include "ComponentSystem/IInterface.h"

namespace FizzyAde::ICMPPingEngine
{
    class ICMPPingEngineData;
    class ICMPPingItem;

    /**
     * IPingEngine implementation for ICMP
     *
     * Implements the IPingEngine interface to implement a ping engine
     * that uses ICMP echo packets for measurements.
     *
     */

    class ICMPPingEngine :
        public FizzyAde::Core::IPingEngine
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IPingEngine)

    public:
        /**
         * constructor
         *
         * @param version Whether this is IPv4 or IPv6
         */
        ICMPPingEngine(FizzyAde::Core::IPVersion version);

        /**
         * Default destructor
         */
        ~ICMPPingEngine();

        /**
         * @sa IPingEngine
         */
        virtual bool setInterval(std::chrono::milliseconds interval);
        virtual bool setTimeout(std::chrono::milliseconds timeout);

        virtual bool start();
        virtual bool stop();

        virtual FizzyAde::Core::IPingTarget *addTarget(QHostAddress hostAddress);
        virtual FizzyAde::Core::IPingTarget *addTarget(QHostAddress hostAddress, int ttl);
        virtual bool removeTarget(FizzyAde::Core::IPingTarget *target);
        virtual std::chrono::system_clock::time_point epoch();

        /**
         * @sa IConfiguration
         */
        virtual QJsonObject saveConfiguration();
        virtual bool loadConfiguration(QJsonObject configuration);

    protected:
        /**
         * Deletes timed out requests and emits timeout results
         *
         * Called from the timeout thread.
         *
         * @sa FZICMPPingTimeout
         */
        void timeoutRequests(void);

        /**
         * Adds a ping request to the engine so it can be tracked
         *
         * @param[in] pingItem the item to be tracked
         */
        void addRequest(FizzyAde::ICMPPingEngine::ICMPPingItem *pingItem);

        /**
         * Removes a tracked request and deletes the item
         *
         * @param[in] pingItem the item to be removed
         */
        void removeRequest(FizzyAde::ICMPPingEngine::ICMPPingItem *pingItem);

        /**
         * Finds a tracked request by id
         *
         * The id is (icmp_id<<16) | icmp_sequence_id
         *
         * @param[in] id the request to find
         * @return returns the request if it exists, otherwise NULL
         */
        FizzyAde::ICMPPingEngine::ICMPPingItem *getRequest(uint32_t id);

        /**
         * Sets the transmission epoch
         *
         * @param[in] epoch the epoch
         */
        void setEpoch(std::chrono::system_clock::time_point epoch);

        /**
         * Returns the IP version of the engine
         *
         * @return IP version
         */
        FizzyAde::Core::IPVersion version();

        friend class ICMPPingReceiver;
        friend class ICMPPingTransmitter;
        friend class ICMPPingTimeout;

    protected:
        std::shared_ptr<ICMPPingEngineData> d;
    };
}

#endif // FIZZYADE_ICMPPINGENGINE_ICMPPINGENGINE_H
