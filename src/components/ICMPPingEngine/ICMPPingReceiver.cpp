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

#include "ICMPPingReceiver.h"
#include "ICMPPingEngine.h"
#include "ICMPPingItem.h"
#include "ICMPPingTarget.h"
#include "ICMPSocket/ICMPSocket.h"
#include "ICMPPacket/ICMPPacket.h"
#include "Utils.h"
#include <cerrno>
#include <array>
#include <fcntl.h>
#include <cstdint>
#include <chrono>
#include <QThread>
#include <QHostAddress>
#include <QtEndian>
#include <gsl/gsl>
#include <QDateTime>

using namespace std::chrono_literals;

constexpr auto DefaultReplyTimeout = 1s;

Nedrysoft::ICMPPingEngine::ICMPPingReceiver::ICMPPingReceiver(Nedrysoft::ICMPPingEngine::ICMPPingEngine *engine) {
    m_engine = engine;
    m_timeout = DefaultReplyTimeout;
    m_isRunning = false;
}

void Nedrysoft::ICMPPingEngine::ICMPPingReceiver::doWork() {
    QByteArray receiveBuffer;
    Nedrysoft::ICMPSocket::ICMPSocket *socket = Nedrysoft::ICMPSocket::ICMPSocket::createReadSocket(
            static_cast<Nedrysoft::ICMPSocket::IPVersion>(m_engine->version()));
    QHostAddress receiveAddress;

    QThread::currentThread()->setPriority(QThread::HighPriority);

    m_isRunning = true;

    while (m_isRunning) {
        auto result = socket->recvfrom(receiveBuffer, receiveAddress, m_timeout);
        auto receiveTime = std::chrono::high_resolution_clock::now();

        if (result != -1) {
            Nedrysoft::Core::PingResult::PingResultCode resultCode = Nedrysoft::Core::PingResult::NoReply;

            auto responsePacket = Nedrysoft::ICMPPacket::ICMPPacket::fromData(receiveBuffer,
                                                                              static_cast<Nedrysoft::ICMPPacket::IPVersion>(m_engine->version()));

            //qDebug() << QThread::currentThreadId() << "ping result" << responsePacket.resultCode() << responsePacket.id() << responsePacket.sequence();

            if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::Invalid) {
                continue;
            }

            if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::EchoReply) {
                resultCode = Nedrysoft::Core::PingResult::Ok;
            }

            if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::TimeExceeded) {
                resultCode = Nedrysoft::Core::PingResult::Ok;
            }

            auto pingItem = m_engine->getRequest(
                    Nedrysoft::Utils::fzMake32(responsePacket.id(), responsePacket.sequence()));

            if (pingItem) {
                pingItem->lock();
                if (!pingItem->serviced()) {
                    pingItem->setServiced(true);
                    pingItem->unlock();

                    std::chrono::duration<double> diff = receiveTime - pingItem->transmitTime();

                    auto pingResult = Nedrysoft::Core::PingResult(pingItem->sampleNumber(), resultCode, receiveAddress,
                                                                  pingItem->transmitEpoch(), diff, pingItem->target());

                    emit Nedrysoft::ICMPPingEngine::ICMPPingReceiver::result(pingResult);
                } else {
                    pingItem->unlock();
                }

                m_engine->removeRequest(pingItem);

                delete pingItem;
            }
        }
    }

    delete socket;
}
