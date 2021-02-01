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

#ifndef NEDRYSOFT_PINGNOO_ICMPAPIPINGTARGET_H
#define NEDRYSOFT_PINGNOO_ICMPAPIPINGTARGET_H

#include "Core/IPingTarget.h"
#include "ICMPAPIPingEngineSpec.h"

#if defined(Q_OS_WIN)
#include <WS2tcpip.h>
#include <WinSock2.h>
#endif

namespace Nedrysoft::Pingnoo {
    class ICMPAPIPingTargetData;

    class ICMPAPIPingEngine;

    /**
     * @brief       The ICMPAPIPingTarget describes a host target.
     *
     * @details     A ping target is used by an Nedrysoft::Core::IPingEngine to keep track of destinations to be pinged.
     */
    class ICMPAPIPingTarget :
            public Nedrysoft::Core::IPingTarget {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IPingTarget)

        public:
            /**
             * @brief       Constructs a ICMPAPIPingTarget for the given engine with the supplied host and ttl.
             *
             * @param[in]   engine the ping engine to be associated with this target.
             * @param[in]   hostAddress the target of the ping.
             * @param[in]   ttl the TTL to be used in the ping.
             */
            ICMPAPIPingTarget(Nedrysoft::Pingnoo::ICMPAPIPingEngine *engine, const QHostAddress &hostAddress, int ttl = 0);

        public:
            /**
             * @brief       Sets the target host address.
             *
             * @see         Nedrysoft::Core::IPingTarget::setHostAddress
             *
             * @param[in]   hostAddress the host address to be pinged.
             */
            setHostAddress(const QHostAddress &hostAddress) -> void override;

            /**
             * @brief       Returns the host address for this target.
             *
             * @see         Nedrysoft::Core::IPingTarget::hostAddress
             *
             * @returns     the host address for this target.
             */
            hostAddress() -> QHostAddress override;

            /**
             * @brief       Returns the Nedrysoft::Core::IPingEngine that created this target.
             *
             * @see         Nedrysoft::Core::IPingTarget::engine
             *
             * @returns     the Nedrysoft::Core::IPingEngine instance.
             */
            auto engine() -> Nedrysoft::Core::IPingEngine * override;

            /**
             * @brief       Returns the user data attached to this target.
             *
             * @see         Nedrysoft::Core::IPingTarget::userData
             *
             * @returns     the user data.
             */
            auto userData() -> void * override;

            /**
             * @brief       Sets the user data attached to this target.
             *
             * @see         Nedrysoft::Core::IPingTarget::setUserData
             *
             * @param[in]   data the user data.
             */
            auto setUserData(void *data) -> void override;

            /**
             * @brief       Returns the TTL of this target.
             *
             * @see         Nedrysoft::Core::IPingTarget::ttl
             *
             * @returns     the ttl value.
             */
            auto ttl() -> uint16_t override;

        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @returns     the JSON configuration.
             */
            auto saveConfiguration() -> QJsonObject override;

            /**
             * @brief       Loads the configuration.
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            auto loadConfiguration(QJsonObject configuration) -> bool override;

        protected:
            /**
             * @brief       Returns a socket descriptor to be used to send an ICMP packet to the target.
             *
             * @returns     the socket descriptor.
             */
            auto socketDescriptor() -> SOCKET;

            /**
             * @brief       Returns the ICMP id used for this target.
             *
             * @returns     the id.
             */
            auto id() -> uint16_t;

            friend class ICMPAPIPingTransmitter;

        protected:
            std::shared_ptr<ICMPAPIPingTargetData> d;
    };
}

#endif // NEDRYSOFT_PINGNOO_ICMPAPIPINGTARGET_H
