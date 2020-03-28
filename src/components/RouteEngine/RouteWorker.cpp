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

constexpr int TransmitRetries = 1;//5;
constexpr int MaxRouteHops = 64;
constexpr auto DefaultReplyTimeout = 1s;//3s;
constexpr int PingPayloadLength = 52;

FizzyAde::RouteEngine::RouteWorker::RouteWorker()
{
    m_isRunning = false;
}

FizzyAde::RouteEngine::RouteWorker::~RouteWorker() = default;

bool FizzyAde::RouteEngine::RouteWorker::ping_v4(const QHostAddress &hostAddress, int ttl, QHostAddress *returnAddress, bool *isComplete)
{
    if (!m_isRunning) {
        return(false);
    }

    auto socket = FizzyAde::ICMPSocket::ICMPSocket::createWriteSocket(ttl, FizzyAde::ICMPSocket::V4);

    auto id = static_cast<uint16_t>((QRandomGenerator::global()->generate() % (UINT16_MAX-1))+1);

    for (uint16_t sequence=1;sequence<=TransmitRetries;sequence++) {
        if (!m_isRunning) {
            break;
        }

        auto buffer = FizzyAde::ICMPPacket::ICMPPacket::pingPacket(id, sequence, PingPayloadLength, hostAddress, FizzyAde::ICMPPacket::V4);

        auto result = socket->sendto(buffer, hostAddress);

        if (result!=buffer.length()) {
            qWarning() << "Error sending ICMP request.";
        }

        QByteArray receiveBuffer;

        result = socket->recvfrom(receiveBuffer, *returnAddress, DefaultReplyTimeout);

        if (result!=-1) {
            auto responsePacket = FizzyAde::ICMPPacket::ICMPPacket::fromData(receiveBuffer, FizzyAde::ICMPPacket::V4);

            if (responsePacket.resultCode()!=FizzyAde::ICMPPacket::Invalid) {
                if ((responsePacket.id()==id) && (responsePacket.sequence()==sequence)) {
                    if (responsePacket.resultCode()==FizzyAde::ICMPPacket::EchoReply) {
                        *isComplete = true;
                    }

                    delete socket;

                    return(true);
                }
            }
        }
    }

    delete socket;

    return(false);
}

bool FizzyAde::RouteEngine::RouteWorker::ping_v6(const QHostAddress &hostAddress, int hopLimit, QHostAddress *returnAddress, bool *isComplete)
{
    if (!m_isRunning) {
        return(false);
    }

    auto socket = FizzyAde::ICMPSocket::ICMPSocket::createWriteSocket(hopLimit, FizzyAde::ICMPSocket::V6);

    auto id = static_cast<uint16_t>((QRandomGenerator::global()->generate() % (UINT16_MAX-1))+1);

    for (uint16_t sequence=1;sequence<=TransmitRetries;sequence++) {
        if (!m_isRunning) {
            break;
        }

        auto buffer = FizzyAde::ICMPPacket::ICMPPacket::pingPacket(id, sequence, PingPayloadLength, hostAddress, FizzyAde::ICMPPacket::V6);

        auto result = socket->sendto(buffer, hostAddress);

        if (result!=buffer.length()) {
            qWarning() << "Error sending ICMP request.";
        }

        QByteArray receiveBuffer;

        result = socket->recvfrom(receiveBuffer, *returnAddress, DefaultReplyTimeout);

        if (result!=-1) {
            auto responsePacket = FizzyAde::ICMPPacket::ICMPPacket::fromData(receiveBuffer, FizzyAde::ICMPPacket::V6);

            if (responsePacket.resultCode()!=FizzyAde::ICMPPacket::Invalid) {
                if ((responsePacket.id()==id) && (responsePacket.sequence()==sequence)) {
                    if (responsePacket.resultCode()==FizzyAde::ICMPPacket::EchoReply) {
                        *isComplete = true;
                    }

                    delete socket;

                    return(true);
                }
            }
        }
    }

    delete socket;

    return(false);
}

void FizzyAde::RouteEngine::RouteWorker::doWork()
{
    auto addressList = QHostInfo::fromName(m_host).addresses();
    auto route = FizzyAde::Core::RouteList();
    auto hopAddress = QHostAddress();

    m_isRunning = true;

    if (addressList.count()) {
        auto targetAddress = addressList.at(0);
        auto hop = 1;
        auto isComplete = false;

        while ((!isComplete) && (hop<=MaxRouteHops) && (m_isRunning)) {
            bool hopResponded;

            if (targetAddress.protocol()==QAbstractSocket::IPv4Protocol) {
                hopResponded = ping_v4(targetAddress, hop, &hopAddress, &isComplete);
            } else if (targetAddress.protocol()==QAbstractSocket::IPv6Protocol) {
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

        if (isComplete) {
            emit result(addressList.first(), route);
        } else {
            emit result(addressList.first(), FizzyAde::Core::RouteList());
        }
    }
}

void FizzyAde::RouteEngine::RouteWorker::setHost(QString host)
{
    m_host = std::move(host);
}
