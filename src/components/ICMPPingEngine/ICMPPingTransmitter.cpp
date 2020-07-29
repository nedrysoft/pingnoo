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

#include "ICMPPingTransmitter.h"
#include "ICMPPingEngine.h"
#include "ICMPPingTarget.h"
#include "ICMPPingItem.h"
#include "ICMPPacket/ICMPPacket.h"
#include "ICMPSocket/ICMPSocket.h"
#include <cerrno>
#include <fcntl.h>
#include <cstdint>
#include <chrono>
#include <QThread>
#include <QRandomGenerator>
#include <QMutexLocker>
#include <QtEndian>
#include <thread>

using namespace std::chrono_literals;

constexpr auto DefaultTransmitInterval = 10s;

FizzyAde::ICMPPingEngine::ICMPPingTransmitter::ICMPPingTransmitter(FizzyAde::ICMPPingEngine::ICMPPingEngine *engine)
{
    m_engine = engine;
    m_interval = DefaultTransmitInterval;
    m_isRunning = false;
}

void FizzyAde::ICMPPingEngine::ICMPPingTransmitter::doWork()
{
    struct icmp icmp_request = {};
    unsigned long sampleNumber = 0;

    m_isRunning = true;

    m_engine->setEpoch(std::chrono::system_clock::now());

    QThread::currentThread()->setPriority(QThread::HighPriority);

    auto currentSequenceId = static_cast<uint16_t>(QRandomGenerator::global()->generate());

    while(m_isRunning) {
        auto startTime = std::chrono::high_resolution_clock::now();

        m_targetsMutex.lock();

        for(auto target : m_targets) {
            auto socket = target->socket();

            auto pingItem = new FizzyAde::ICMPPingEngine::ICMPPingItem();

            pingItem->setTarget(target);
            pingItem->setId(target->id());
            pingItem->setSequenceId(currentSequenceId);
            pingItem->setSampleNumber(sampleNumber);

            m_engine->addRequest(pingItem);

            pingItem->setTransmitTime(std::chrono::high_resolution_clock::now(), std::chrono::system_clock::now());

            auto buffer = FizzyAde::ICMPPacket::ICMPPacket::pingPacket(target->id(), currentSequenceId, 52, target->hostAddress(), static_cast<FizzyAde::ICMPPacket::IPVersion>(m_engine->version()));

            auto result = socket->sendto(buffer, target->hostAddress());

            if (result!=sizeof(icmp_request)) {
                // transmit error
            }
        }

        m_targetsMutex.unlock();

        auto diff = std::chrono::high_resolution_clock::now() - startTime;

        if (diff<m_interval) {
            std::this_thread::sleep_for(m_interval-diff);
        }

        currentSequenceId++;
        sampleNumber++;
    }
}

bool FizzyAde::ICMPPingEngine::ICMPPingTransmitter::setInterval(std::chrono::milliseconds interval)
{
    m_interval = interval;

    return true;
}

void FizzyAde::ICMPPingEngine::ICMPPingTransmitter::addTarget(FizzyAde::ICMPPingEngine::ICMPPingTarget *target)
{
    QMutexLocker locker(&m_targetsMutex);

    m_targets.append(target);
}
