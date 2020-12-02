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

#include "Utils.h"
#include "ICMPPacket.h"
#include <cerrno>
#include <fcntl.h>
#include <cstdint>
#include <array>

#if defined(Q_OS_UNIX)

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstddef>

#endif
#if defined(Q_OS_WIN)
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "windows_ip_icmp.h"
#endif

#include <QtGlobal>
#include <QtEndian>
#include <QByteArray>
#include <QDataStream>
#include <gsl/gsl>

struct ipv6_header {
    unsigned int version: 4;
    unsigned int trafficClass: 8;
    unsigned int flowLabel: 20;
    uint16_t payloadLength;
    uint8_t nextHeader;
    uint8_t hopLimit;
    uint8_t sourceAddress[16];
    uint8_t destinationAddress[16];
};

struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t reserved;
};

struct ipv6_psuedo_header {
    struct in6_addr sourceAddress;
    struct in6_addr destinationAddress;
    uint32_t packetLength;
    uint8_t zero[3];
    uint8_t nextHeader;
};

struct icmp_v6 {
    ipv6_psuedo_header header;
    struct icmp icmp;
};

#define ICMP6_ECHO 128
#define ICMP6_ECHO_REPLY 129

Nedrysoft::ICMPPacket::ICMPPacket::ICMPPacket() {
    m_resultCode = Invalid;
    m_sequence = 0;
    m_id = 0;
}

Nedrysoft::ICMPPacket::ICMPPacket::ICMPPacket(uint16_t id, uint16_t sequence, ResultCode resultCode) {
    m_resultCode = resultCode;
    m_sequence = sequence;
    m_id = id;
}

Nedrysoft::ICMPPacket::ICMPPacket Nedrysoft::ICMPPacket::ICMPPacket::fromData(const QByteArray &dataBuffer, Nedrysoft::ICMPPacket::IPVersion version) {
    if (version == Nedrysoft::ICMPPacket::V4) {
        return fromData_v4(dataBuffer);
    } else if (version == Nedrysoft::ICMPPacket::V6) {
        return fromData_v6(dataBuffer);
    } else {
        return ICMPPacket();
    }
}

Nedrysoft::ICMPPacket::ICMPPacket Nedrysoft::ICMPPacket::ICMPPacket::fromData_v4(const QByteArray &dataBuffer) {
    unsigned char ip_vhl;
    unsigned char ip_vhl_tx;
    unsigned char ip_header_size;
    unsigned char ip_header_size_tx;
    uint16_t received_id = 0;
    uint16_t received_sequence = 0;
    constexpr unsigned int IP_HEADER_LENGTH_MASK = 0x0F;

    auto mainSpan = gsl::span<const unsigned char>(reinterpret_cast<const unsigned char *>(dataBuffer.data()),
                                                   dataBuffer.length());

    ip_vhl = mainSpan[0];
    ip_header_size = ( ip_vhl & IP_HEADER_LENGTH_MASK ) * sizeof(uint32_t);

    auto responseSpan = mainSpan.subspan(ip_header_size);

    auto icmp_response = reinterpret_cast<const struct icmp *>(responseSpan.data());

    if (icmp_response->icmp_code == ICMP_ECHOREPLY) {
        if (icmp_response->icmp_type == ICMP_ECHOREPLY) {
            received_id = qFromBigEndian<uint16_t>(icmp_response->icmp_hun.ih_idseq.icd_id);
            received_sequence = qFromBigEndian<uint16_t>(icmp_response->icmp_hun.ih_idseq.icd_seq);

            return ICMPPacket(received_id, received_sequence, EchoReply);
        }

        if (icmp_response->icmp_type == ICMP_TIMXCEED) {
            constexpr unsigned int IP_HEADER_OFFSET = 0x08;

            ip_vhl_tx = mainSpan[ip_header_size + IP_HEADER_OFFSET];
            ip_header_size_tx = ( ip_vhl_tx & IP_HEADER_LENGTH_MASK ) * sizeof(uint32_t);

            auto receivedRequestSpan = mainSpan.subspan(ip_header_size + IP_HEADER_OFFSET + ip_header_size_tx);

            auto received_icmp_request = reinterpret_cast<const struct icmp *>(receivedRequestSpan.data());

            received_id = qFromBigEndian<uint16_t>(received_icmp_request->icmp_hun.ih_idseq.icd_id);

            received_sequence = qFromBigEndian<uint16_t>(received_icmp_request->icmp_hun.ih_idseq.icd_seq);

            return ICMPPacket(received_id, received_sequence, TimeExceeded);
        }
    }

    return ICMPPacket();
}

Nedrysoft::ICMPPacket::ICMPPacket Nedrysoft::ICMPPacket::ICMPPacket::fromData_v6(const QByteArray &dataBuffer) {
    uint16_t received_id = 0;
    uint16_t received_sequence = 0;

    auto responseSpan = gsl::span<const unsigned char>(reinterpret_cast<const unsigned char *>(dataBuffer.data()),
                                                       dataBuffer.length());

    auto icmp_response = reinterpret_cast<const struct icmp *>(responseSpan.data());

    if (icmp_response->icmp_code == 0) {
        if (icmp_response->icmp_type == ICMP6_ECHO_REPLY) {
            received_id = qFromBigEndian<uint16_t>(icmp_response->icmp_hun.ih_idseq.icd_id);
            received_sequence = qFromBigEndian<uint16_t>(icmp_response->icmp_hun.ih_idseq.icd_seq);

            return ICMPPacket(received_id, received_sequence, EchoReply);
        }

        if (icmp_response->icmp_type == 3) {
            auto request_icmp_header = responseSpan.subspan(sizeof(icmp_header) + sizeof(ipv6_header));

            auto rx_icmp_request = reinterpret_cast<const struct icmp *>(request_icmp_header.data());

            received_id = qFromBigEndian<uint16_t>(rx_icmp_request->icmp_hun.ih_idseq.icd_id);
            received_sequence = qFromBigEndian<uint16_t>(rx_icmp_request->icmp_hun.ih_idseq.icd_seq);

            return ICMPPacket(received_id, received_sequence, TimeExceeded);
        }
    }

    return ICMPPacket();
}


uint16_t Nedrysoft::ICMPPacket::ICMPPacket::checksum(void *buffer, int length) {
    QByteArray dataArray(reinterpret_cast<char *>(buffer), length);
    QDataStream dataStream(dataArray);
    uint32_t checksum = 0;

    dataStream.setByteOrder(QDataStream::LittleEndian);

    while (!dataStream.atEnd()) {
        checksum += read<uint16_t>(dataStream);

        length -= 2;
    }

    checksum = ( checksum >> ( sizeof(uint16_t) * CHAR_BIT )) + ( checksum & UINT16_MAX );
    checksum += ( checksum >> ( sizeof(uint16_t) * CHAR_BIT ));

    return static_cast<uint16_t>(~checksum);
}

Nedrysoft::ICMPPacket::ResultCode Nedrysoft::ICMPPacket::ICMPPacket::resultCode() {
    return m_resultCode;
}

uint16_t Nedrysoft::ICMPPacket::ICMPPacket::id() {
    return m_id;
}

uint16_t Nedrysoft::ICMPPacket::ICMPPacket::sequence() {
    return m_sequence;
}

QByteArray Nedrysoft::ICMPPacket::ICMPPacket::pingPacket(uint16_t id, uint16_t sequence, int payLoadLength,
                                                         const QHostAddress &destinationAddress,
                                                         Nedrysoft::ICMPPacket::IPVersion version) {
    if (version == Nedrysoft::ICMPPacket::V4) {
        return pingPacket_v4(id, sequence, payLoadLength, destinationAddress);
    } else if (version == Nedrysoft::ICMPPacket::V6) {
        return pingPacket_v6(id, sequence, payLoadLength, destinationAddress);
    } else {
        return QByteArray();
    }
}

QByteArray Nedrysoft::ICMPPacket::ICMPPacket::pingPacket_v6(uint16_t id, uint16_t sequence, int payLoadLength,
                                                            const QHostAddress &destinationAddress) {
    QByteArray echoRequestBuffer(payLoadLength + sizeof(icmp_v6), 0);
    auto echoRequestLength = static_cast<int>(echoRequestBuffer.size());
    auto icmp_v6 = reinterpret_cast<struct icmp_v6 *>(echoRequestBuffer.data());
    auto icmp_request = &icmp_v6->icmp;

    memset(echoRequestBuffer.data(), 0, echoRequestBuffer.size());

    icmp_v6->header.packetLength = sizeof(icmp) + payLoadLength;
    icmp_v6->header.nextHeader = IPPROTO_ICMPV6;

    icmp_v6->header.sourceAddress.s6_addr[15] = 1;

    auto destinationAddressRaw = destinationAddress.toIPv6Address();

    memcpy(icmp_v6->header.destinationAddress.s6_addr, &destinationAddressRaw, 16);

    icmp_request->icmp_type = ICMP6_ECHO;
    icmp_request->icmp_code = 0;
    icmp_request->icmp_cksum = 0;
    icmp_request->icmp_hun.ih_idseq.icd_id = qToBigEndian<uint16_t>(id);
    icmp_request->icmp_hun.ih_idseq.icd_seq = qToBigEndian<uint16_t>(sequence);

    icmp_request->icmp_cksum = Nedrysoft::ICMPPacket::ICMPPacket::checksum(icmp_v6, echoRequestLength);

    return QByteArray(reinterpret_cast<char *>(icmp_request), icmp_v6->header.packetLength);
}

QByteArray Nedrysoft::ICMPPacket::ICMPPacket::pingPacket_v4(uint16_t id, uint16_t sequence, int payLoadLength,
                                                            const QHostAddress &destinationAddress) {
    Q_UNUSED(destinationAddress);
    QByteArray echoRequestBuffer(payLoadLength + sizeof(icmp), 0);
    auto echoRequestLength = static_cast<int>(echoRequestBuffer.size());
    auto icmp_request = reinterpret_cast<icmp *>(echoRequestBuffer.data());

    icmp_request->icmp_type = ICMP_ECHO;
    icmp_request->icmp_code = 0;
    icmp_request->icmp_cksum = 0;
    icmp_request->icmp_hun.ih_idseq.icd_id = qToBigEndian<uint16_t>(id);
    icmp_request->icmp_hun.ih_idseq.icd_seq = qToBigEndian<uint16_t>(sequence);

    icmp_request->icmp_cksum = Nedrysoft::ICMPPacket::ICMPPacket::checksum(icmp_request, echoRequestLength);

    return QByteArray(reinterpret_cast<const char *>(icmp_request), echoRequestLength);
}


