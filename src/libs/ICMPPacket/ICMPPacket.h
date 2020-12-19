/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
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

#ifndef NEDRYSOFT_ICMPPACKET_ICMPPACKET_H
#define NEDRYSOFT_ICMPPACKET_ICMPPACKET_H

#include <QDataStream>
#include <QHostAddress>
#include <cstdint>
#include <vector>

#if defined(Q_OS_WIN)
#include "windows_ip_icmp.h"
#endif

#if ( defined(NEDRYSOFT_LIBRARY_ICMPPACKET_EXPORT))
#define NEDRYSOFT_ICMPPACKET_DLLSPEC Q_DECL_EXPORT
#else
#define NEDRYSOFT_ICMPPACKET_DLLSPEC Q_DECL_IMPORT
#endif

namespace Nedrysoft::ICMPPacket {
    enum IPVersion {
        Unknown = 0,
        V4 = 4,
        V6 = 6
    };

    enum ResultCode {
        Invalid = 0,
        EchoReply = 1,
        TimeExceeded = 2
    };

    /**
     * @brief       THe ICMPPacket class provides functions to decode and encode ICMP packets.
     */
    class NEDRYSOFT_ICMPPACKET_DLLSPEC ICMPPacket {
        public:
            /**
             * @brief       Creates an ICMP packet from raw data.
             *
             * @param[in]   dataBuffer the raw icmp packet.
             * @param[in]   version version of ICMP packet we are expecting.
             *
             * @returns     the decoded packet.
             */
            static ICMPPacket fromData(const QByteArray &dataBuffer, IPVersion version);

            /**
             * @brief       Calculate ICMP crc16 from raw data.
             *
             * @param[in]   buffer the raw icmp packet.
             * @param[in]   length the length of the packet.
             *
             * @returns     the crc16 result.
             */
            static uint16_t checksum(void *buffer, int length);

            /**
             * @brief       Create a ping request packet.
             *
             * @note        Depending on platform the id/sequence fields may be overwritten by the operating system.
             *
             * @param[in]   id the packet id.
             * @param[in]   sequence the packet sequence.
             * @param[in]   payloadLength the length of the payload.
             * @param[in]   destinationAddress the address of the target.
             * @param[in]   version the ip version of the icmp packet.
             *
             * @returns     a QByteArray containing the created raw packet.
             */
            static QByteArray pingPacket(
                    uint16_t id,
                    uint16_t sequence,
                    int payLoadLength, const QHostAddress &destinationAddress,
                    Nedrysoft::ICMPPacket::IPVersion version );

            /**
             * @brief       Returns the result of a packet decode.
             *
             * @returns     the ResultCode value which indicates what was decoded.
             */
            Nedrysoft::ICMPPacket::ResultCode resultCode();

            /**
             * @brief       The id field of the ICMP packet.
             *
             * @returns     the icmp id.
             */
            uint16_t id();

            /**
             * @brief       The sequence field of the ICMP packet.
             *
             * @returns     the icmp sequence.
             */
            uint16_t sequence();

            /**
             * @brief       Cast to std::string operator.
             *
             * @returns     a formatted string showing the packet information.
             */
            operator std::string();

        private:
            /**
             * @brief       Constructs an ICMPPacket.
             */
            ICMPPacket();

            /**
             * @brief       Constructs an ICMPPacket with the given information.
             *
             * @param[in]   id the initial id to be used in the packet.
             * @param[in]   sequence the initial sequence to be used in the packet.
             * @param[in]   resultCode the initial result code.
             * @param[in]   ipVersion the IP version of the packet.
             */
            ICMPPacket(uint16_t id, uint16_t sequence, ResultCode resultCode, IPVersion ipVersion);

            /**
             * @brief       Decodes an ipv4 icmp packet for from raw data.
             *
             * @param[in]   dataBuffer the raw data.
             *
             * @returns     the decoded icmp packet.
             */
            static ICMPPacket fromData_v4(const QByteArray &dataBuffer);

            /**
             * @brief       Decodes a ipv6 icmp packet for from raw data.
             *
             * @param[in]   dataBuffer the raw data.
             *
             * @returns     the decoded icmp packet.
             */
            static ICMPPacket fromData_v6(const QByteArray &dataBuffer);

            /**
             * @brief       Creates an ipv6 icmp packet.
             *
             * @param[in]   id the id to be used in the packet.
             * @param[in]   sequence  the sequence to be used in the packet.
             * @param[in]   payLoadLength the payLoadLength to be used in the packet.
             * @param[in]   destinationAddress the destination address of the ping.
             *
             * @returns     a QByteArray containing the raw ipv4 icmp packet.
             */
            static QByteArray pingPacket_v6(
                    uint16_t id,
                    uint16_t sequence,
                    int payLoadLength,
                    const QHostAddress &destinationAddress );

            /**
             * @brief       Creates an ipv4 icmp packet.
             *
             * @param[in]   id the id to be used in the packet.
             * @param[in]   sequence the sequence to be used in the packet.
             * @param[in]   payLoadLength the payLoadLength to be used in the packet.
             * @param[in]   destinationAddress the destination address of the ping.
             *
             * @returns     a QByteArray containing the raw ipv6 icmp packet.
             */
            static QByteArray pingPacket_v4(
                    uint16_t id,
                    uint16_t sequence,
                    int payLoadLength,
                    const QHostAddress &destinationAddress );

            /**
             * @brief       Template to read raw data from a datastream.
             */
            template<class T>
            static T read(QDataStream &dataStream) {
                T data;

                dataStream >> data;

                return data;
            }

        private:
            ResultCode m_resultCode;
            uint16_t m_id;
            uint16_t m_sequence;
            IPVersion m_ipVersion;
    };
}

#endif // NEDRYSOFT_ICMPPACKET_ICMPPACKET_H
