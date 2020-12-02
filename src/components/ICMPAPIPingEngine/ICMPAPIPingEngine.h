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

#ifndef NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINE_H
#define NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINE_H

#include "ICMPAPIPingEngineSpec.h"
#include "Core/IPingEngine.h"

namespace Nedrysoft::Pingnoo {
    class ICMPAPIPingEngineData;

    class ICMPPingItem;

    /**
     * IPingEngine implementation for Windows ICMP API
     *
     * Implements the IPingEngine interface to implement a ping engine
     * that uses the windows ICMP api
     *
     */

    class ICMPAPIPingEngine :
            public QObject,
            public Nedrysoft::Core::IPingEngine {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingEngine)

        public:
            /**
             * Default constructor
             */
            ICMPAPIPingEngine();

            /**
             * @sa IPingEngine
             */
            virtual bool setInterval(std::chrono::milliseconds interval);

            virtual bool setTimeout(std::chrono::milliseconds timeout);

            virtual bool start();

            virtual bool stop();

            virtual Nedrysoft::Core::IPingTarget *addTarget(QHostAddress hostAddress);

            virtual Nedrysoft::Core::IPingTarget *addTarget(QHostAddress hostAddress, int ttl);

            virtual bool removeTarget(Nedrysoft::Core::IPingTarget *target);

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
            //void timeoutRequests(void);

            /**
             * Adds a ping request to the engine so it can be tracked
             *
             * @param[in] pingItem the item to be tracked
             */
            //void addRequest(FZICMPPingItem *pingItem);

            /**
             * Removes a tracked request and deletes the item
             *
             * @param[in] pingItem the item to be removed
             */
            //void removeRequest(FZICMPPingItem *pingItem);

            /**
             * Finds a tracked request by id
             *
             * The id is (icmp_id<<16) | icmp_sequence_id
             *
             * @param[in] id the request to find
             * @return returns the request if it exists, otherwise NULL
             */
            //FZICMPPingItem *getRequest(uint32_t id);
            /*
                friend class FZICMPPingReceiver;
                friend class FZICMPPingTransmitter;
                friend class FZICMPPingTimeout;
            */
        protected:
            std::shared_ptr<ICMPAPIPingEngineData> d;
    };
}

#endif // NEDRYSOFT_PINGNOO_ICMPAPIPINGENGINE_H
