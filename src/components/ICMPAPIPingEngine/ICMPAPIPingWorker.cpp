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
        int sampleNumber,
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *target) {

    m_target = target;
    m_sampleNumber = sampleNumber;
}

void Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingWorker::doWork() {
    QByteArray dataBuffer = QString("pingnoo ping ").arg(m_sampleNumber).toLatin1();
    QByteArray replyBuffer(sizeof(ICMP_ECHO_REPLY) + dataBuffer.length(), 0);
    HANDLE icmpHandle;
    QHostAddress replyHost;
    Nedrysoft::Core::PingResult result;
    QElapsedTimer timer;
    qint64 started, finished;

#if defined(_WIN64)
    IP_OPTION_INFORMATION32 options;
#else
    IP_OPTION_INFORMATION options;
#endif
    //TODO: ipV6

    options.Ttl = m_target->ttl();
    options.Flags = 0;
    options.OptionsData = nullptr;
    options.OptionsSize = 0;
    options.Tos = 0;

    PIP_OPTION_INFORMATION pipOptions = reinterpret_cast<PIP_OPTION_INFORMATION>(&options);

    icmpHandle = IcmpCreateFile();

    started = timer.nsecsElapsed();

    auto returnValue = IcmpSendEcho(icmpHandle, m_target->hostAddress().toIPv4Address(),
                                    dataBuffer.data(), static_cast<WORD>(dataBuffer.length()), pipOptions,
                                    replyBuffer.data(), static_cast<DWORD>(replyBuffer.length()),
                                    std::chrono::duration<DWORD, std::milli>(
                                            DefaultTransmitTimeout).count()); // NOLINT(cppcoreguidelines-pro-type-union-access)

    finished = timer.nsecsElapsed();

    auto roundTripTime = static_cast<double>(finished - started) / nanosecondsInMillisecond;
    auto epoch = std::chrono::system_clock::now();

    Nedrysoft::Core::PingResult::ResultCode resultCode = Nedrysoft::Core::PingResult::ResultCode::NoReply;

    if (returnValue) {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY) replyBuffer.data();

        //TODO: ipV6

        replyHost = QHostAddress(ntohl(pEchoReply->Address));

        if (pEchoReply->Status==IP_SUCCESS) {
            resultCode = Nedrysoft::Core::PingResult::ResultCode::Ok;
        } else if (pEchoReply->Status==IP_TTL_EXPIRED_TRANSIT) {
            resultCode = Nedrysoft::Core::PingResult::ResultCode::TimeExceeded;
        }
    }

    result = Nedrysoft::Core::PingResult(
            m_sampleNumber,
            resultCode,
            replyHost,
            epoch,
            std::chrono::duration<double, std::milli>(roundTripTime),
            m_target);

    IcmpCloseHandle(icmpHandle);

    Q_EMIT pingResult(result);
}



