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

#ifndef FIZZYADE_PINGNOO_ICMPAPIPINGTARGET_H
#define FIZZYADE_PINGNOO_ICMPAPIPINGTARGET_H

#include "ICMPAPIPingEngineSpec.h"
#include "Core/IPingTarget.h"
#if defined(Q_OS_WIN)
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

namespace FizzyAde::Pingnoo
{
    class ICMPAPIPingTargetData;
    class ICMPAPIPingEngine;

    /**
     * IPingTarget implementation for ICMPAPI
     *
     * Implements the IPingTarget interface to implement a ping target
     * that uses ICMP echo packets for measurements.
     *
     */

    class ICMPAPIPingTarget : public QObject, public FizzyAde::Core::IPingTarget
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IPingTarget)

    public:
        ICMPAPIPingTarget(FizzyAde::Pingnoo::ICMPAPIPingEngine *engine, QHostAddress hostAddress, int ttl=0);

        /**
         * @sa IPingTarget
         */
        virtual void setHostAddress(QHostAddress hostAddress);

        virtual QHostAddress hostAddress();

        virtual FizzyAde::Core::IPingEngine *engine();

        virtual void *userData() ;

        virtual void setUserData(void *data);

        /**
         * @sa IConfiguration
         */
        virtual QJsonObject saveConfiguration();
        virtual bool loadConfiguration(QJsonObject configuration);

    protected:

        /**
         * Returns a socket descriptor to be used to send an ICMP packet to the target
         *
         * @return the socket descriptor
         */
#if defined(Q_OS_UNIX)
        int socketDescriptor();
#elif defined(Q_OS_WIN)
        SOCKET socketDescriptor();
#endif

        /**
         * Returns the ICMP id used for this target
         *
         * @return the id
         */
        uint16_t id();

        friend class ICMPAPIPingTransmitter;

    protected:
        std::shared_ptr<ICMPAPIPingTargetData> d;
    };
}

#endif // FIZZYADE_PINGNOO_ICMPAPIPINGTARGET_H
