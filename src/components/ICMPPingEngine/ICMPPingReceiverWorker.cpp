/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo
 *
 * Created by Adrian Carpenter on 10/12/2020.
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

#include "ICMPPingReceiverWorker.h"

#include "ICMPPacket/ICMPPacket.h"
#include "ICMPPingEngine.h"
#include "ICMPPingItem.h"
#include "ICMPPingTarget.h"
#include "ICMPSocket/ICMPSocket.h"

#include <QDateTime>
#include <QHostAddress>
#include <QThread>
#include <QtEndian>
#include <array>
#include <chrono>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

constexpr auto DefaultReplyTimeout = 1s;

Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::ICMPPingReceiverWorker() :
        m_isRunning(false),
        m_engine(nullptr) {

}

Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker *Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::getInstance() {
    static Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker *instance = nullptr;

    if (instance) {
        return instance;
    }

    qRegisterMetaType<std::chrono::time_point<std::chrono::high_resolution_clock>>();

    instance = new Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker();

    auto receiverThread = new QThread();

    instance->moveToThread(receiverThread);

    connect(receiverThread, &QThread::started, instance, &Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::doWork);

    receiverThread->start();

    return instance;
}

void Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::doWork() {
    QByteArray receiveBuffer;
    Nedrysoft::ICMPSocket::ICMPSocket *socket = Nedrysoft::ICMPSocket::ICMPSocket::createReadSocket(
            static_cast<Nedrysoft::ICMPSocket::IPVersion>(Nedrysoft::ICMPSocket::V4));

    QHostAddress receiveAddress;

    QThread::currentThread()->setPriority(QThread::HighPriority);

    m_isRunning = true;

    while (m_isRunning) {
        auto result = socket->recvfrom(receiveBuffer, receiveAddress, DefaultReplyTimeout);
        std::chrono::time_point<std::chrono::high_resolution_clock> receiveTime = std::chrono::high_resolution_clock::now();

        if (result!=-1) {
            Q_EMIT packetReceived(receiveTime, receiveBuffer, receiveAddress);
        }
    }

    delete socket;

    QThread::currentThread()->deleteLater();
}