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

#include "ICMPPingTransmitter.h"

#include "ICMPPacket/ICMPPacket.h"
#include "ICMPPingEngine.h"
#include "ICMPPingItem.h"
#include "ICMPPingTarget.h"
#include "ICMPSocket/ICMPSocket.h"

#include <QThread>
#include <QtEndian>
#include <chrono>
#include <cstdint>
#include <random>
#include <spdlog/spdlog.h>
#include <thread>

using namespace std::chrono_literals;

constexpr auto DefaultTransmitInterval = 10s;

Nedrysoft::ICMPPingEngine::ICMPPingTransmitter::ICMPPingTransmitter(Nedrysoft::ICMPPingEngine::ICMPPingEngine *engine) :
        m_interval(DefaultTransmitInterval),
        m_engine(engine),
        m_isRunning(false) {

}

Nedrysoft::ICMPPingEngine::ICMPPingTransmitter::~ICMPPingTransmitter() {
    //qDeleteAll(m_targets);
}

void Nedrysoft::ICMPPingEngine::ICMPPingTransmitter::doWork() {
    unsigned long sampleNumber = 0;

    m_isRunning = true;

    m_engine->setEpoch(std::chrono::system_clock::now());

    QThread::currentThread()->setPriority(QThread::HighPriority);

    std::random_device randomDevice;
    std::mt19937 mt(randomDevice());
    std::uniform_int_distribution<uint16_t> dist(0, UINT16_MAX);

    auto currentSequenceId = dist(mt);

    while (m_isRunning) {
        if (!m_targets.isEmpty()) {
            SPDLOG_TRACE("Preparing ping set to " + m_targets.last()->hostAddress().toString().toStdString());
        }

        auto startTime = std::chrono::high_resolution_clock::now();

        m_targetsMutex.lock();

        for (auto target : m_targets) {
            auto socket = target->socket();

            auto pingItem = new Nedrysoft::ICMPPingEngine::ICMPPingItem();

            pingItem->setTarget(target);
            pingItem->setId(target->id());
            pingItem->setSequenceId(currentSequenceId);
            pingItem->setSampleNumber(sampleNumber);

            m_engine->addRequest(pingItem);

            pingItem->setTransmitTime(std::chrono::high_resolution_clock::now(), std::chrono::system_clock::now());

            auto buffer = Nedrysoft::ICMPPacket::ICMPPacket::pingPacket(
                    target->id(),
                    currentSequenceId,
                    52,
                    target->hostAddress(),
                    static_cast<Nedrysoft::ICMPPacket::IPVersion>(m_engine->version()) );

            auto result = socket->sendto(buffer, target->hostAddress());

            SPDLOG_TRACE(
                    QString("Sent ping to %1 (TTL=%2, Result=%3)")
                    .arg(target->hostAddress().toString())
                    .arg(socket->ttl()).arg(result)
                    .toStdString() );

            if (result != buffer.length()) {
                SPDLOG_ERROR("Unable to send packet to "+target->hostAddress().toString().toStdString());
            }
        }

        m_targetsMutex.unlock();

        auto diff = std::chrono::high_resolution_clock::now() - startTime;

        if (diff < m_interval) {
            std::this_thread::sleep_for(m_interval - diff);
        }

        currentSequenceId++;
        sampleNumber++;
    }
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTransmitter::setInterval(std::chrono::milliseconds interval) -> bool {
    m_interval = interval;

    return true;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTransmitter::addTarget(Nedrysoft::ICMPPingEngine::ICMPPingTarget *target) -> void {
    QMutexLocker locker(&m_targetsMutex);

    m_targets.append(target);
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTransmitter::interval() -> std::chrono::milliseconds {
    return m_interval;
}