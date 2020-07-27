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

FizzyAde::ICMPPingEngine::ICMPPingReceiver::ICMPPingReceiver(FizzyAde::ICMPPingEngine::ICMPPingEngine *engine)
{
    m_engine = engine;
    m_timeout = DefaultReplyTimeout;
    m_isRunning = false;
}

void FizzyAde::ICMPPingEngine::ICMPPingReceiver::doWork()
{
    QByteArray receiveBuffer;
    FizzyAde::ICMPSocket::ICMPSocket *socket = FizzyAde::ICMPSocket::ICMPSocket::createReadSocket(static_cast<FizzyAde::ICMPSocket::IPVersion>(m_engine->version()));
    QHostAddress receiveAddress;

    QThread::currentThread()->setPriority(QThread::HighPriority);

    m_isRunning = true;

    while(m_isRunning) {
        auto result = socket->recvfrom(receiveBuffer, receiveAddress, m_timeout);
        auto receiveTime = std::chrono::high_resolution_clock::now();

        if (result!=-1) {
            FizzyAde::Core::PingResult::PingResultCode resultCode = FizzyAde::Core::PingResult::NoReply;

            auto responsePacket = FizzyAde::ICMPPacket::ICMPPacket::fromData(receiveBuffer, static_cast<FizzyAde::ICMPPacket::IPVersion>(m_engine->version()));

            //qDebug() << QThread::currentThreadId() << "ping result" << responsePacket.resultCode() << responsePacket.id() << responsePacket.sequence();

            if (responsePacket.resultCode()==FizzyAde::ICMPPacket::Invalid) {
                continue;
            }

            if (responsePacket.resultCode()==FizzyAde::ICMPPacket::EchoReply) {
                resultCode = FizzyAde::Core::PingResult::Ok;
            }

            if (responsePacket.resultCode()==FizzyAde::ICMPPacket::TimeExceeded) {
                resultCode = FizzyAde::Core::PingResult::Ok;
            }

            auto pingItem = m_engine->getRequest(FizzyAde::Utils::fzMake32(responsePacket.id(), responsePacket.sequence()));

            if (pingItem) {
                pingItem->lock();
                if (!pingItem->serviced()) {
                    pingItem->setServiced(true);
                    pingItem->unlock();

                    std::chrono::duration<double> diff = receiveTime-pingItem->transmitTime();

                    auto pingResult = FizzyAde::Core::PingResult(pingItem->sampleNumber(), resultCode, receiveAddress, pingItem->transmitEpoch(), diff, pingItem->target());

                    emit FizzyAde::ICMPPingEngine::ICMPPingReceiver::result(pingResult);
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
