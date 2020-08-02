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

    /**
     * @brief           ICMP packet class
     *
     * @details         decodes ICMP packets
     *
     */
    class FIZZYADE_ICMPPACKET_DLLSPEC ICMPPacket
    {
    public:
        /**
         * @brief           Creates a packet from raw data
         *
         * @param[in]       dataBuffer              the raw icmp packet
         * @param[in]       version                 version of ICMP packet we are expecting
         *
         * @return          decoded packet
         *
         */
        static ICMPPacket fromData(const QByteArray &dataBuffer, IPVersion version);

        /**
         * @brief           Calculate ICMP crc16 from raw data
         *
         * @param[in]       buffer                  the raw icmp packet
         * @param[in]       length                  length of the packet
         *
         * @return          the crc16 result
         *
         */
        static uint16_t checksum(void *buffer, int length);

        /**
         * @brief           Create a ping request packet
         *
         * @note            Depending on platform the id/sequence fields may be overwritten by the
         *                  operating system.
         *
         * @param[in]       id                      the packet id
         * @param[in]       sequence                the packet sequence
         * @param[in]       payloadLength           the length of the payload
         * @param[in]       destinationAddress      the address of the target
         * @param[in]       version                 the ip version of the icmp packet
         *
         * @return          QByteArray containing the created raw packet
         *
         */
        static QByteArray pingPacket(uint16_t id, uint16_t sequence, int payLoadLength, const QHostAddress &destinationAddress, FizzyAde::ICMPPacket::IPVersion version);

        /**
         * @brief           The result of a packet decode
         *
         * @return          a ResultCode value which indicates what was decoded
         *
         */
        FizzyAde::ICMPPacket::ResultCode resultCode();

        /**
         * @brief           The id
         *
         * @return          the icmp id of the packet
         *
         */
        uint16_t id();

        /**
         * @brief           The sequence
         *
         * @return          the icmp sequence of the packet
         *
         */
        uint16_t sequence();

    private:
        /**
         * @brief Constructor
         *
         */
        ICMPPacket();

        /**
         * @brief Constructor
         *
         * @details         Constructs a packet the the given information
         *
         * @param[in]       id                  the initial id to be used in the packet
         * @param[in]       sequence            the initial sequence to be used in the packet
         * @param[in]       resultCode          the initial result code
         *
         */
        ICMPPacket(uint16_t id, uint16_t sequence, ResultCode resultCode);

        /**
         * @brief Decode a ipv4 icmp packet for from raw data
         *
         * @param[in]       dataBuffer          the raw data
         *
         * @return          the decoded icmp packet
         *
         */
        static ICMPPacket fromData_v4(const QByteArray &dataBuffer);

        /**
         * @brief Decode a ipv6 icmp packet for from raw data
         *
         * @param[in]       dataBuffer          the raw data
         *
         * @return          the decoded icmp packet
         *
         */
        static ICMPPacket fromData_v6(const QByteArray &dataBuffer);

        /**
         * @brief Create a ipv4 icmp packet
         *
         * @param[in]       id                  the id to be used in the packet
         * @param[in]       sequence            the sequence to be used in the packet
         * @param[in]       payLoadLength       the payLoadLength to be used in the packet
         * @param[in]       destinationAddress  the destination address of the ping
         *
         * @return          QByteArray containing the raw ipv4 icmp packet
         *
         */
        static QByteArray pingPacket_v6(uint16_t id, uint16_t sequence, int payLoadLength, const QHostAddress &destinationAddress);

        /**
         * @brief Create a ipv6 icmp packet
         *
         * @param[in]       id                  the id to be used in the packet
         * @param[in]       sequence            the sequence to be used in the packet
         * @param[in]       payLoadLength       the payLoadLength to be used in the packet
         * @param[in]       destinationAddress  the destination address of the ping
         *
         * @return          QByteArray containing the raw ipv6 icmp packet
         *
         */
        static QByteArray pingPacket_v4(uint16_t id, uint16_t sequence, int payLoadLength, const QHostAddress &destinationAddress);

        /**
         * @brief           Template to read raw data from a datastream
         *
         */
        template <class T>
        static T read(QDataStream &dataStream)
        {
            T data;

            dataStream >> data;

            return data;
        }

    private:
        ResultCode m_resultCode;
        uint16_t m_id;
        uint16_t m_sequence;
    };
}

#endif // FIZZYADE_ICMPPACKET_ICMPPACKET_H
