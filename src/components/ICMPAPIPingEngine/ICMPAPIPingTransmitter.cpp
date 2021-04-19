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

#include "ICMPAPIPingTransmitter.h"

#include "ICMPAPIPingEngine.h"
#include "ICMPAPIPingTarget.h"
#include "ICMPAPIPingWorker.h"

#include <QRandomGenerator>
#include <QThread>
#include <chrono>
#include <cstdint>

using namespace std::chrono_literals;

constexpr auto DefaultTransmitInterval = 1s;
constexpr auto DefaultReplyTimeout = 3s;
constexpr auto DefaultTransmitTimeout = 3s;

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTransmitter::ICMPAPIPingTransmitter(
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *engine) :

            m_engine(engine),
            m_interval(DefaultTransmitInterval),
            m_isRunning(false) {

}

void Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTransmitter::addTarget(
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *target) {

    m_targets.append(target);
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTransmitter::doWork() -> void {
    uint16_t currentSequenceId;
    std::chrono::high_resolution_clock::time_point startTime;
    unsigned long sampleNumber = 0;

    m_isRunning = true;

    currentSequenceId = static_cast<uint16_t>(QRandomGenerator::global()->generate());

    while (m_isRunning) {
        startTime = std::chrono::high_resolution_clock::now();

        m_targetsMutex.lock();

        for (auto target : m_targets) {
            auto pingWorker = new Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingWorker(m_engine, sampleNumber, target);

            auto pingThread = new QThread();

            pingWorker->moveToThread(pingThread);

            connect(pingThread, &QThread::started, pingWorker, &ICMPAPIPingWorker::doWork);
            connect(pingThread, &QThread::finished, pingThread, &QThread::deleteLater);
            connect(pingThread, &QThread::finished, pingWorker, &ICMPAPIPingWorker::deleteLater);

            connect(pingWorker,
                    &ICMPAPIPingWorker::result,
                    this,
                    &ICMPAPIPingTransmitter::result,
                    Qt::DirectConnection);

            pingThread->start();
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

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTransmitter::setInterval(std::chrono::milliseconds interval) -> bool {
    m_interval = interval;

    return true;
}
