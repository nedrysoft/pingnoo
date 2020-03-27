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

#ifndef FIZZYADE_ICMPPACKET_ICMPPACKET_H
#define FIZZYADE_ICMPPACKET_ICMPPACKET_H

#include <cstdint>
#include <vector>
#include <QDataStream>
#include <QHostAddress>
#if defined(Q_OS_WIN)
#include "windows_ip_icmp.h"
#endif

#if (defined(FIZZYADE_LIBRARY_ICMPPACKET_EXPORT))
#define FIZZYADE_ICMPPACKET_DLLSPEC Q_DECL_EXPORT
#else
#define FIZZYADE_ICMPPACKET_DLLSPEC Q_DECL_IMPORT
#endif

namespace FizzyAde::ICMPPacket
{
    enum IPVersion
    {
        V4 = 4,
        V6 = 6
    };

    enum ResultCode
    {
        Invalid = 0,
        EchoReply = 1,
        TimeExceeded = 2
    };

    class FIZZYADE_ICMPPACKET_DLLSPEC ICMPPacket
    {
    public:
        static ICMPPacket fromData(const QByteArray &dataBuffer, IPVersion version);
        static uint16_t checksum(void *buffer, int length);

        static QByteArray pingPacket(uint16_t id, uint16_t sequence, int payLoadLength, const QHostAddress &destinationAddress, FizzyAde::ICMPPacket::IPVersion version);

        FizzyAde::ICMPPacket::ResultCode resultCode();
        uint16_t id();
        uint16_t sequence();

    private:

        ICMPPacket();
        ICMPPacket(uint16_t id, uint16_t sequence, ResultCode resultCode);

        static ICMPPacket fromData_v4(const QByteArray &dataBuffer);
        static ICMPPacket fromData_v6(const QByteArray &dataBuffer);

        static QByteArray pingPacket_v6(uint16_t id, uint16_t sequence, int payLoadLength, const QHostAddress &destinationAddress);
        static QByteArray pingPacket_v4(uint16_t id, uint16_t sequence, int payLoadLength, const QHostAddress &destinationAddress);

        template <class T>
        static T read(QDataStream &dataStream)
        {
            T data;

            dataStream >> data;

            return(data);
        }

    private:
        ResultCode m_resultCode;
        uint16_t m_id;
        uint16_t m_sequence;
    };
}

#endif // FIZZYADE_ICMPPACKET_ICMPPACKET_H
