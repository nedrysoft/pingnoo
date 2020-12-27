/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
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

#include <QHostAddress>
#include <QThread>
#include <QtEndian>
#include <chrono>

using namespace std::chrono_literals;

constexpr auto DefaultReplyTimeout = 1s;

Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::ICMPPingReceiverWorker() :
        m_isRunning(false),
        m_engine(nullptr),
        m_receiveWorker(nullptr),
        m_receiverThread(new QThread),
        m_socket(nullptr) {

}

Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::~ICMPPingReceiverWorker() {
    if (m_engine) {
        delete m_engine;
    }

    m_receiveWorker->m_isRunning = false;

    if (m_receiverThread) {
        m_receiverThread->quit();
        m_receiverThread->wait();

        delete m_receiverThread;
    }

    if (m_socket) {
        delete m_socket;
    }
}

auto Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::getInstance() -> Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker * {
    static Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker *instance = nullptr;

    if (instance) {
        return instance;
    }

    instance = new Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker;

    qRegisterMetaType<std::chrono::time_point<std::chrono::high_resolution_clock>>();

    instance->moveToThread(instance->m_receiverThread);

    connect(instance->m_receiverThread, &QThread::started, instance, &Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::doWork);

    instance->m_receiverThread->start();

    instance->m_receiveWorker = instance;

    return instance;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::doWork() -> void {
    QByteArray receiveBuffer;
    m_socket =  Nedrysoft::ICMPSocket::ICMPSocket::createReadSocket(
            static_cast<Nedrysoft::ICMPSocket::IPVersion>(Nedrysoft::ICMPSocket::V4));

    QHostAddress receiveAddress;

    QThread::currentThread()->setPriority(QThread::HighPriority);

    m_isRunning = true;

    while (QThread::currentThread()->isRunning() && (m_isRunning)) {
        auto result = m_socket->recvfrom(receiveBuffer, receiveAddress, DefaultReplyTimeout);

        std::chrono::time_point<std::chrono::high_resolution_clock> receiveTime = std::chrono::high_resolution_clock::now();

        if (result!=-1) {
            Q_EMIT packetReceived(receiveTime, receiveBuffer, receiveAddress);
        }
    }
}