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

#include "ICMPPacket.h"

#include "Utils.h"

#include <array>

#if defined(Q_OS_MACOS)
#include <netinet/ip.h>
#endif

#if defined(Q_OS_UNIX)
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#endif

#if defined(Q_OS_WIN)
#include <WS2tcpip.h>
#endif

#include <QDataStream>
#include <QtEndian>
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

struct ipv6_pseudo_header {
    struct in6_addr sourceAddress;
    struct in6_addr destinationAddress;
    uint32_t packetLength;
    uint8_t zero[3];
    uint8_t nextHeader;
};

struct icmp_v6 {
    ipv6_pseudo_header header;
    struct icmp icmp;
};

constexpr auto ICMP6_ECHO = 128;
constexpr auto  ICMP6_ECHO_REPLY = 129;

Nedrysoft::ICMPPacket::ICMPPacket::ICMPPacket() :
        m_resultCode(Invalid),
        m_id(0),
        m_sequence(0),
        m_ipVersion(Unknown) {

}

Nedrysoft::ICMPPacket::ICMPPacket::ICMPPacket(uint16_t id, uint16_t sequence, ResultCode resultCode, IPVersion ipVersion) :
        m_resultCode(resultCode),
        m_id(id),
        m_sequence(sequence),
        m_ipVersion(ipVersion) {

}

auto Nedrysoft::ICMPPacket::ICMPPacket::fromData(
        const QByteArray &dataBuffer,
        Nedrysoft::ICMPPacket::IPVersion version) -> Nedrysoft::ICMPPacket::ICMPPacket {

    if (version == Nedrysoft::ICMPPacket::V4) {
        return fromData_v4(dataBuffer);
    } else if (version == Nedrysoft::ICMPPacket::V6) {
        return fromData_v6(dataBuffer);
    } else {
        return ICMPPacket();
    }
}

auto Nedrysoft::ICMPPacket::ICMPPacket::fromData_v4(const QByteArray &dataBuffer) -> Nedrysoft::ICMPPacket::ICMPPacket {
    unsigned char ip_vhl;
    unsigned char ip_vhl_tx;
    unsigned char ip_header_size;
    unsigned char ip_header_size_tx;
    uint16_t received_id;
    uint16_t received_sequence;
    constexpr unsigned int IP_HEADER_LENGTH_MASK = 0x0F;

    auto mainSpan = gsl::span<const unsigned char>(
            reinterpret_cast<const unsigned char *>(dataBuffer.data()),
            dataBuffer.length() );

    ip_vhl = mainSpan[0];
    ip_header_size = ( ip_vhl & IP_HEADER_LENGTH_MASK ) * sizeof(uint32_t);

    auto responseSpan = mainSpan.subspan(ip_header_size);

    auto icmp_response = reinterpret_cast<const struct icmp *>(responseSpan.data());

    if (icmp_response->icmp_code == ICMP_ECHOREPLY) {
        if (icmp_response->icmp_type == ICMP_ECHOREPLY) {
            received_id = qFromBigEndian<uint16_t>(icmp_response->icmp_hun.ih_idseq.icd_id);
            received_sequence = qFromBigEndian<uint16_t>(icmp_response->icmp_hun.ih_idseq.icd_seq);

            return ICMPPacket(received_id, received_sequence, EchoReply, V4);
        }

        if (icmp_response->icmp_type == ICMP_TIMXCEED) {
            constexpr unsigned int IP_HEADER_OFFSET = 0x08;

            ip_vhl_tx = mainSpan[ip_header_size + IP_HEADER_OFFSET];
            ip_header_size_tx = ( ip_vhl_tx & IP_HEADER_LENGTH_MASK ) * sizeof(uint32_t);

            auto receivedRequestSpan = mainSpan.subspan(ip_header_size + IP_HEADER_OFFSET + ip_header_size_tx);

            auto received_icmp_request = reinterpret_cast<const struct icmp *>(receivedRequestSpan.data());

            received_id = qFromBigEndian<uint16_t>(received_icmp_request->icmp_hun.ih_idseq.icd_id);

            received_sequence = qFromBigEndian<uint16_t>(received_icmp_request->icmp_hun.ih_idseq.icd_seq);

            return ICMPPacket(received_id, received_sequence, TimeExceeded, V4);
        }
    }

    return ICMPPacket();
}

auto Nedrysoft::ICMPPacket::ICMPPacket::fromData_v6(const QByteArray &dataBuffer) -> Nedrysoft::ICMPPacket::ICMPPacket {
    uint16_t received_id;
    uint16_t received_sequence;

    auto responseSpan = gsl::span<const unsigned char>(
            reinterpret_cast<const unsigned char *>(dataBuffer.data()),
            dataBuffer.length() );

    auto icmp_response = reinterpret_cast<const struct icmp *>(responseSpan.data());

    if (icmp_response->icmp_code == 0) {
        if (icmp_response->icmp_type == ICMP6_ECHO_REPLY) {
            received_id = qFromBigEndian<uint16_t>(icmp_response->icmp_hun.ih_idseq.icd_id);
            received_sequence = qFromBigEndian<uint16_t>(icmp_response->icmp_hun.ih_idseq.icd_seq);

            return ICMPPacket(received_id, received_sequence, EchoReply, V6);
        }

        if (icmp_response->icmp_type == 3) {
            auto request_icmp_header = responseSpan.subspan(sizeof(icmp_header) + sizeof(ipv6_header));

            auto rx_icmp_request = reinterpret_cast<const struct icmp *>(request_icmp_header.data());

            received_id = qFromBigEndian<uint16_t>(rx_icmp_request->icmp_hun.ih_idseq.icd_id);
            received_sequence = qFromBigEndian<uint16_t>(rx_icmp_request->icmp_hun.ih_idseq.icd_seq);

            return ICMPPacket(received_id, received_sequence, TimeExceeded, V6);
        }
    }

    return ICMPPacket();
}

auto Nedrysoft::ICMPPacket::ICMPPacket::checksum(void *buffer, int length) -> uint16_t {
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

auto Nedrysoft::ICMPPacket::ICMPPacket::resultCode() -> Nedrysoft::ICMPPacket::ResultCode {
    return m_resultCode;
}

auto Nedrysoft::ICMPPacket::ICMPPacket::id() -> uint16_t {
    return m_id;
}

auto Nedrysoft::ICMPPacket::ICMPPacket::sequence() -> uint16_t {
    return m_sequence;
}

auto Nedrysoft::ICMPPacket::ICMPPacket::pingPacket(
        uint16_t id,
        uint16_t sequence,
        int payLoadLength,
        const QHostAddress &destinationAddress,
        Nedrysoft::ICMPPacket::IPVersion version) -> QByteArray {

    if (version == Nedrysoft::ICMPPacket::V4) {
        return pingPacket_v4(id, sequence, payLoadLength, destinationAddress);
    } else if (version == Nedrysoft::ICMPPacket::V6) {
        return pingPacket_v6(id, sequence, payLoadLength, destinationAddress);
    } else {
        return QByteArray();
    }
}

auto Nedrysoft::ICMPPacket::ICMPPacket::pingPacket_v6(
        uint16_t id,
        uint16_t sequence,
        int payLoadLength,
        const QHostAddress &destinationAddress) -> QByteArray {

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

auto Nedrysoft::ICMPPacket::ICMPPacket::pingPacket_v4(
        uint16_t id,
        uint16_t sequence,
        int payLoadLength,
        const QHostAddress &destinationAddress) -> QByteArray {

    Q_UNUSED(destinationAddress)

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

Nedrysoft::ICMPPacket::ICMPPacket::operator std::string() {
    QString returnString;
    QString ipVersionString;
    QString resultCodeString;

    switch(m_ipVersion) {
        case V4: {
            ipVersionString = "IPv4";
            break;
        }

        case V6: {
            ipVersionString = "IPv6";
            break;
        }

        default: {
            ipVersionString = "IPv?";
            break;
        }
    }

    switch(m_resultCode) {
        case Invalid: {
            resultCodeString = "Invalid";
            break;
        }
        case EchoReply: {
            resultCodeString = "Echo Reply";
            break;
        }
        case TimeExceeded: {
            resultCodeString = "Time Exceeded";
            break;
        }

        default: {
            resultCodeString = QString("Unknown (%1)").arg(m_resultCode);
            break;
        }
    }

    returnString = QString("id=0x%1 seq=0x%2 %3 result=%4")
            .arg(m_id, 4, 16, QLatin1Char('0'))
            .arg(m_sequence, 4, 16, QLatin1Char('0'))
            .arg(ipVersionString)
            .arg(resultCodeString);

    return returnString.toStdString();
}

