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

#include "RouteWorker.h"
#include "ICMPPacket/ICMPPacket.h"
#include "ICMPSocket/ICMPSocket.h"
#include <cerrno>
#include <fcntl.h>
#include <cstdint>
#include <array>
#include <gsl/gsl>
#include <QRandomGenerator>
#include <QHostInfo>
#include <QHostAddress>
#include <QDebug>
#include <QtEndian>

using namespace std::chrono_literals;

constexpr int TransmitRetries = 1;
constexpr int MaxRouteHops = 64;
constexpr auto DefaultReplyTimeout = 1s;
constexpr int PingPayloadLength = 52;

Nedrysoft::RouteEngine::RouteWorker::RouteWorker(Nedrysoft::Core::IPVersion ipVersion) {
    m_isRunning = false;
    m_ipVersion = ipVersion;
}

Nedrysoft::RouteEngine::RouteWorker::~RouteWorker() = default;

bool Nedrysoft::RouteEngine::RouteWorker::ping_v4(const QHostAddress &hostAddress, int ttl, QHostAddress *returnAddress,
                                                  bool *isComplete) {
    if (!m_isRunning) {
        return false;
    }

    auto socket = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(ttl, Nedrysoft::ICMPSocket::V4);

    auto id = static_cast<uint16_t>(( QRandomGenerator::global()->generate() % ( UINT16_MAX - 1 )) + 1);

    for (uint16_t sequence = 1; sequence <= TransmitRetries; sequence++) {
        if (!m_isRunning) {
            break;
        }

        auto buffer = Nedrysoft::ICMPPacket::ICMPPacket::pingPacket(id, sequence, PingPayloadLength, hostAddress,
                                                                    Nedrysoft::ICMPPacket::V4);

        auto result = socket->sendto(buffer, hostAddress);

        if (result != buffer.length()) {
            qWarning() << "Error sending ICMP request.";
        }

        QByteArray receiveBuffer;

        std::chrono::milliseconds responseTimeout = DefaultReplyTimeout;

        while (responseTimeout.count() > 0) {
            auto startTime = std::chrono::system_clock::now();

            if (socket->recvfrom(receiveBuffer, *returnAddress, responseTimeout) == -1) {
                break;
            }

            auto endTime = std::chrono::system_clock::now();

            auto responsePacket = Nedrysoft::ICMPPacket::ICMPPacket::fromData(receiveBuffer, Nedrysoft::ICMPPacket::V4);

            if (responsePacket.resultCode() != Nedrysoft::ICMPPacket::Invalid) {
                if (( responsePacket.id() == id ) && ( responsePacket.sequence() == sequence )) {
                    if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::EchoReply) {
                        *isComplete = true;
                    }

                    delete socket;

                    return true;
                }
            }

            std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            responseTimeout -= diff;
        }
    }

    delete socket;

    return false;
}

bool
Nedrysoft::RouteEngine::RouteWorker::ping_v6(const QHostAddress &hostAddress, int hopLimit, QHostAddress *returnAddress,
                                             bool *isComplete) {
    if (!m_isRunning) {
        return false;
    }

    auto socket = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(hopLimit, Nedrysoft::ICMPSocket::V6);

    auto id = static_cast<uint16_t>(( QRandomGenerator::global()->generate() % ( UINT16_MAX - 1 )) + 1);

    for (uint16_t sequence = 1; sequence <= TransmitRetries; sequence++) {
        if (!m_isRunning) {
            break;
        }

        auto buffer = Nedrysoft::ICMPPacket::ICMPPacket::pingPacket(id, sequence, PingPayloadLength, hostAddress,
                                                                    Nedrysoft::ICMPPacket::V6);

        auto result = socket->sendto(buffer, hostAddress);

        if (result != buffer.length()) {
            qWarning() << "Error sending ICMP request.";
        }

        QByteArray receiveBuffer;

        std::chrono::milliseconds responseTimeout = DefaultReplyTimeout;

        while (responseTimeout.count() > 0) {
            auto startTime = std::chrono::system_clock::now();

            if (socket->recvfrom(receiveBuffer, *returnAddress, responseTimeout) == -1) {
                break;
            }

            auto endTime = std::chrono::system_clock::now();

            auto responsePacket = Nedrysoft::ICMPPacket::ICMPPacket::fromData(receiveBuffer, Nedrysoft::ICMPPacket::V6);

            if (responsePacket.resultCode() != Nedrysoft::ICMPPacket::Invalid) {
                if (( responsePacket.id() == id ) && ( responsePacket.sequence() == sequence )) {
                    if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::EchoReply) {
                        *isComplete = true;
                    }

                    delete socket;

                    return true;
                }
            }

            std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            responseTimeout -= diff;
        }
    }

    delete socket;

    return false;
}

void Nedrysoft::RouteEngine::RouteWorker::doWork() {
    auto addressList = QHostInfo::fromName(m_host).addresses();
    auto route = Nedrysoft::Core::RouteList();
    auto hopAddress = QHostAddress();
    auto isComplete = false;
    auto actualTargetAddress = QHostAddress();

    m_isRunning = true;

    for (auto targetAddress : addressList) {
        auto hop = 1;

        if (m_ipVersion == Nedrysoft::Core::IPVersion::V4) {
            if (targetAddress.protocol() != QAbstractSocket::IPv4Protocol) {
                continue;
            }
        } else if (m_ipVersion == Nedrysoft::Core::IPVersion::V6) {
            if (targetAddress.protocol() != QAbstractSocket::IPv6Protocol) {
                continue;
            }
        } else {
            continue;
        }

        route = Nedrysoft::Core::RouteList();

        while (( !isComplete ) && ( hop <= MaxRouteHops ) && ( m_isRunning )) {
            bool hopResponded;

            if (targetAddress.protocol() == QAbstractSocket::IPv4Protocol) {
                hopResponded = ping_v4(targetAddress, hop, &hopAddress, &isComplete);
            } else if (targetAddress.protocol() == QAbstractSocket::IPv6Protocol) {
                hopResponded = ping_v6(targetAddress, hop, &hopAddress, &isComplete);
            } else {
                return;
            }

            if (hopResponded) {
                route.append(hopAddress);
            } else {
                route.append(QHostAddress());
            }

            hop++;
        }

        actualTargetAddress = targetAddress;

        if (isComplete) {
            break;
        }
    }

    if (isComplete) {
        emit result(actualTargetAddress, route);
    } else {
        emit result(actualTargetAddress, Nedrysoft::Core::RouteList());
    }
}


void Nedrysoft::RouteEngine::RouteWorker::setHost(QString host) {
    m_host = std::move(host);
}
