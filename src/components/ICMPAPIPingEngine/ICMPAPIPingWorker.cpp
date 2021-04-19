/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 18/04/2021.
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

#include "ICMPAPIPingWorker.h"

#include "ICMPAPIPingEngine.h"
#include "ICMPAPIPingTarget.h"

#include <QHostAddress>
#include <QMutexLocker>
#include <QThread>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <chrono>

#include <iphlpapi.h>
#include <IcmpAPI.h>

using namespace std::chrono_literals;

constexpr unsigned long PingPayloadLength = 1024;
constexpr auto DefaultTransmitTimeout = 3s;
constexpr auto nanosecondsInMillisecond = 1.0e6;

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingWorker::ICMPAPIPingWorker(
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *engine,
        int sampleNumber,
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *target) :

            m_engine(engine),
            m_target(target),
            m_sampleNumber(sampleNumber) {

}

void Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingWorker::doWork() {
    auto pingResult = m_engine->singleShot(
            m_target->hostAddress(),
            m_target->ttl(),
            std::chrono::duration<DWORD, std::milli>(DefaultTransmitTimeout).count());

    Q_EMIT result(Nedrysoft::Core::PingResult(
            m_sampleNumber,
            pingResult.code(),
            pingResult.hostAddress(),
            pingResult.requestTime(),
            pingResult.roundTripTime(),
            m_target));
}



