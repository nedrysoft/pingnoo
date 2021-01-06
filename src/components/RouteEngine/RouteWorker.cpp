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

#include "RouteWorker.h"

#include "ICMPPacket/ICMPPacket.h"
#include "ICMPSocket/ICMPSocket.h"

#include <QHostAddress>
#include <QHostInfo>
#include <QRandomGenerator>
#include <QtEndian>
#include <cstdint>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

constexpr int TransmitRetries = 1;
constexpr int MaxRouteHops = 64;
constexpr auto DefaultReplyTimeout = 1s;
constexpr int PingPayloadLength = 52;

Nedrysoft::RouteEngine::RouteWorker::RouteWorker(Nedrysoft::Core::IPVersion ipVersion) :
        m_isRunning(false),
        m_ipVersion(ipVersion) {

}

Nedrysoft::RouteEngine::RouteWorker::~RouteWorker() = default;

auto Nedrysoft::RouteEngine::RouteWorker::ping_v4(
        const QHostAddress &hostAddress,
        int ttl,
        QHostAddress *returnAddress,
        bool *isComplete ) -> bool {

    if (!m_isRunning) {
        return false;
    }

    auto socket = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(ttl, Nedrysoft::ICMPSocket::V4);

    auto id = static_cast<uint16_t>((QRandomGenerator::global()->generate() % (UINT16_MAX-1))+1);

    for (uint16_t sequence=1; sequence<=TransmitRetries; sequence++) {
        if (!m_isRunning) {
            break;
        }

        SPDLOG_TRACE(QString("Sending ICMP echo request to %1 with TTL of %2 (id %3 seq %4)")
            .arg(hostAddress.toString())
            .arg(ttl)
            .arg(id)
            .arg(sequence).toStdString() );

        auto buffer = Nedrysoft::ICMPPacket::ICMPPacket::pingPacket(id, sequence, PingPayloadLength, hostAddress,
                                                                    Nedrysoft::ICMPPacket::V4);

        auto result = socket->sendto(buffer, hostAddress);

        if (result != buffer.length()) {
            SPDLOG_ERROR("There was an error sending the ICMP request.");

            continue;
        }

        QByteArray receiveBuffer;

        std::chrono::milliseconds responseTimeout = DefaultReplyTimeout;

        while (responseTimeout.count()>0) {
            auto startTime = std::chrono::system_clock::now();

            if (socket->recvfrom(receiveBuffer, *returnAddress, responseTimeout)==-1) {
                SPDLOG_TRACE(QString("Socket error while receiving response.").toStdString());

                break;
            }

            auto endTime = std::chrono::system_clock::now();

            auto responsePacket = Nedrysoft::ICMPPacket::ICMPPacket::fromData(receiveBuffer, Nedrysoft::ICMPPacket::V4);

            if (responsePacket.resultCode()!=Nedrysoft::ICMPPacket::Invalid) {
                if (( responsePacket.id()==id) && (responsePacket.sequence()==sequence)) {

                    SPDLOG_TRACE(QString("Received valid ICMP response (%1)").arg(responsePacket.resultCode()).toStdString());

                    if (responsePacket.resultCode()==Nedrysoft::ICMPPacket::EchoReply) {
                        *isComplete = true;
                    }

                    delete socket;

                    return true;
                } else {
                    SPDLOG_TRACE(QString("Received ICMP response with wrong id and/or sequence.").toStdString());
                }
            } else {
                SPDLOG_TRACE(QString("Invalid ICMP packet received.").toStdString());
            }

            std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime);

            responseTimeout-=diff;
        }
    }

    delete socket;

    return false;
}

auto Nedrysoft::RouteEngine::RouteWorker::ping_v6(
        const QHostAddress &hostAddress,
        int hopLimit,
        QHostAddress *returnAddress,
        bool *isComplete ) -> bool {

    if (!m_isRunning) {
        return false;
    }

    auto socket = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(hopLimit, Nedrysoft::ICMPSocket::V6);

    auto id = static_cast<uint16_t>((QRandomGenerator::global()->generate() % (UINT16_MAX-1))+1);
    uint16_t sequenceBase = static_cast<uint16_t>((QRandomGenerator::global()->generate() % (UINT16_MAX-1))+1);

    for (uint16_t sequence=1; sequence<=TransmitRetries; sequence++) {
        if (!m_isRunning) {
            break;
        }

        auto buffer = Nedrysoft::ICMPPacket::ICMPPacket::pingPacket(id, sequenceBase+sequence, PingPayloadLength, hostAddress,
                                                                    Nedrysoft::ICMPPacket::V6);

        auto result = socket->sendto(buffer, hostAddress);

        if (result != buffer.length()) {
            SPDLOG_ERROR("There was an error sending the ICMP request.");

            continue;
        }

        QByteArray receiveBuffer;

        std::chrono::milliseconds responseTimeout = DefaultReplyTimeout;

        while (responseTimeout.count()>0) {
            auto startTime = std::chrono::system_clock::now();

            if (socket->recvfrom(receiveBuffer, *returnAddress, responseTimeout)==-1) {
                break;
            }

            auto endTime = std::chrono::system_clock::now();

            auto responsePacket = Nedrysoft::ICMPPacket::ICMPPacket::fromData(receiveBuffer, Nedrysoft::ICMPPacket::V6);

            if (responsePacket.resultCode() != Nedrysoft::ICMPPacket::Invalid) {
                if ((responsePacket.id()==id) && (responsePacket.sequence()==sequenceBase+sequence)) {
                    if (responsePacket.resultCode()==Nedrysoft::ICMPPacket::EchoReply) {
                        *isComplete = true;
                    }

                    delete socket;

                    return true;
                }
            }

            std::chrono::milliseconds diff=std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime);

            responseTimeout-=diff;
        }
    }

    delete socket;

    return false;
}

auto Nedrysoft::RouteEngine::RouteWorker::doWork() -> void {
    auto addressList = QHostInfo::fromName(m_host).addresses();
    auto route = Nedrysoft::Core::RouteList();
    auto hopAddress = QHostAddress();
    auto isComplete = false;

    m_isRunning = true;

    if (!addressList.count()) {
        Q_EMIT result(QHostAddress(), Nedrysoft::Core::RouteList());

        SPDLOG_ERROR(QString("Failed to find address for %1.").arg(m_host).toStdString());

        return;
    }

    auto targetAddress = addressList.at(0);

    auto hop = 1;

    if (m_ipVersion == Nedrysoft::Core::IPVersion::V4) {
        if (targetAddress.protocol() != QAbstractSocket::IPv4Protocol) {
            SPDLOG_ERROR(QString("Socket IP version mismatch. (expecting QAbstractSocket::IPv4Protocol)")
                    .toStdString());

            return;
        }
    } else if (m_ipVersion == Nedrysoft::Core::IPVersion::V6) {
        if (targetAddress.protocol() != QAbstractSocket::IPv6Protocol) {
            SPDLOG_ERROR(QString("Socket IP version mismatch. (expecting QAbstractSocket::IPv6Protocol)")
                                  .toStdString());

            return;
        }
    } else {
        SPDLOG_ERROR(QString("Socket IP version unknown.)").toStdString());

        return;
    }

    route = Nedrysoft::Core::RouteList();

    while ((!isComplete) && (hop<=MaxRouteHops) && (m_isRunning)) {
        bool hopResponded = false;

        if (targetAddress.protocol() == QAbstractSocket::IPv4Protocol) {
            hopResponded = ping_v4(targetAddress, hop, &hopAddress, &isComplete);
        } else if (targetAddress.protocol() == QAbstractSocket::IPv6Protocol) {
            hopResponded = ping_v6(targetAddress, hop, &hopAddress, &isComplete);
        }

        SPDLOG_TRACE(QString("Hop %1 %2.")
                .arg(hop)
                .arg(hopResponded?(isComplete?"responded with echo":"responded with TTL exceeded"):"timed out")
                .toStdString());

        if (hopResponded) {
            route.append(hopAddress);
        } else {
            route.append(QHostAddress());
        }

        hop++;
    }

    if (isComplete) {
        SPDLOG_TRACE(QString("Route to %1 (%2) completed, total of %3 hops.")
                .arg(m_host)
                .arg(targetAddress.toString())
                .arg(hop)
                .toStdString() );

        Q_EMIT result(targetAddress, route);
    } else {
        SPDLOG_ERROR(QString("Failed to discover route to %1.").arg(m_host).toStdString());

        Q_EMIT result(targetAddress, Nedrysoft::Core::RouteList());
    }
}

auto Nedrysoft::RouteEngine::RouteWorker::setHost(QString host) -> void {
    m_host = std::move(host);
}
