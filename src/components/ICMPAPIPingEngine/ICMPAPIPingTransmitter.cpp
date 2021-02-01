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

#include <IcmpAPI.h>
#include <QMutexLocker>
#include <QRandomGenerator>
#include <QThread>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <array>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <fcntl.h>
#include <iphlpapi.h>
#include <string>

using namespace std::chrono_literals;

constexpr auto DefaultTransmitInterval = 1s;
constexpr auto DefaultReplyTimeout = 3s;
constexpr auto DefaultTransmitTimeout = 3s;
constexpr unsigned long PingPayloadLength = 1024;

Nedrysoft::Pingnoo::ICMPAPIPingTransmitter::ICMPAPIPingTransmitter(Nedrysoft::Pingnoo::ICMPAPIPingEngine *engine) :
        m_engine(engine),
        m_interval(DefaultTransmitInterval),
        m_isRunning(false) {

}

auto Nedrysoft::Pingnoo::ICMPAPIPingTransmitter::doWork() -> void {
    HANDLE icmpHandle;
    uint16_t currentSequenceId;
    std::chrono::high_resolution_clock::time_point startTime;
    //FZICMPPingItem *pingItem;
    unsigned long sampleNumber = 0;

    m_isRunning = true;

    QThread::currentThread()->setPriority(QThread::HighPriority);

    currentSequenceId = static_cast<uint16_t>(QRandomGenerator::global()->generate());

    icmpHandle = IcmpCreateFile();

    while (m_isRunning) {
        startTime = std::chrono::high_resolution_clock::now();

        m_targetsMutex.lock();

        QByteArray dataBuffer = QString("Hello World").toLatin1();
        QByteArray tempBuffer(PingPayloadLength, 0);
        struct sockaddr_in toAddress = {};

        inet_pton(AF_INET, "172.29.13.1", &toAddress.sin_addr.S_un.S_addr);

        HANDLE waitEvent;

        waitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

        DWORD returnValue;

        returnValue = IcmpSendEcho2(icmpHandle, waitEvent, nullptr, nullptr, toAddress.sin_addr.S_un.S_addr,
                                    dataBuffer.data(), static_cast<WORD>(dataBuffer.length()), nullptr,
                                    tempBuffer.data(), static_cast<DWORD>(tempBuffer.length()),
                                    std::chrono::duration<DWORD, std::milli>(
                                            DefaultTransmitTimeout).count()); // NOLINT(cppcoreguidelines-pro-type-union-access)

        if (returnValue != ERROR_IO_PENDING) {
            qWarning() << tr("Error sending icmp.");

            m_targetsMutex.unlock();

            continue;
        }

        returnValue = WaitForMultipleObjectsEx(1, &waitEvent, FALSE,
                                               std::chrono::duration<DWORD, std::milli>(DefaultReplyTimeout).count(),
                                               TRUE);

        CloseHandle(waitEvent);

        m_targetsMutex.unlock();

        QThread::sleep(1);

        std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - startTime;

        if (diff < m_interval)
            std::this_thread::sleep_for(m_interval - diff);

        currentSequenceId++;
        sampleNumber++;
    }
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTransmitter::setInterval(std::chrono::milliseconds interval) -> bool {
    m_interval = interval;

    return ( true );
}

/*
void Nedrysoft::Pingnoo::ICMPAPIPingTransmitter::addTarget(FZICMPPingTarget *target)
{
    QMutexLocker locker(&m_targetsMutex);

    m_targets.append(target);
}
*/
