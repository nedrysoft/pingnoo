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

#include "ICMPAPIPingEngine.h"

#include "ICMPAPIPingTarget.h"
#include "ICMPAPIPingTransmitter.h"

#include <QMutex>
#include <QThread>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <chrono>

#include <iphlpapi.h>
#include <IcmpAPI.h>
#include <IPExport.h>

using namespace std::chrono_literals;

constexpr auto DefaultTransmitTimeout = 1s;
constexpr auto DefaultReplyTimeout = 3s;
constexpr auto PingPayloadLength = 64;
constexpr auto nanosecondsInMillisecond = 1.0e6;

class Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineData {

    public:
        ICMPAPIPingEngineData(Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *parent) :
                m_pingEngine(parent),
                m_transmitter(nullptr),
                m_transmitterThread(nullptr),
                m_timeout(std::chrono::milliseconds(0)),
                m_ipVersion(Nedrysoft::Core::IPVersion::V4) {

        }

        friend class ICMPAPIPingEngine;

    private:
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *m_pingEngine;

        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTransmitter *m_transmitter;

        QThread *m_transmitterThread;

        QList<Nedrysoft::Core::IPingTarget *> m_pingTargets;
        Nedrysoft::Core::IPVersion m_ipVersion;

        std::chrono::milliseconds m_timeout = {};
        std::chrono::milliseconds m_interval = {};
};

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::ICMPAPIPingEngine(Nedrysoft::Core::IPVersion version) :
        d(std::make_shared<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineData>(this)) {

    d->m_ipVersion = version;

    // transmitter thread

    d->m_transmitter = new Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTransmitter(this);

    d->m_transmitterThread = new QThread();

    d->m_transmitter->moveToThread(d->m_transmitterThread);

    connect(d->m_transmitterThread, &QThread::started, d->m_transmitter, &ICMPAPIPingTransmitter::doWork);

    connect(d->m_transmitter, &ICMPAPIPingTransmitter::result, this, &ICMPAPIPingEngine::result);

    d->m_transmitterThread->start();
}

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::~ICMPAPIPingEngine() {

}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::addTarget(
        QHostAddress hostAddress) -> Nedrysoft::Core::IPingTarget * {

    Q_UNUSED(hostAddress)

    ICMPAPIPingTarget *pingTarget = new ICMPAPIPingTarget(this, hostAddress);

    d->m_transmitter->addTarget(pingTarget);

    d->m_pingTargets.append(pingTarget);

    return(pingTarget);
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::addTarget(
        QHostAddress hostAddress,
        int ttl ) -> Nedrysoft::Core::IPingTarget * {

    ICMPAPIPingTarget *pingTarget = new ICMPAPIPingTarget(this, hostAddress, ttl);

    d->m_transmitter->addTarget(pingTarget);

    d->m_pingTargets.append(pingTarget);

    return(pingTarget);
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::removeTarget(
        Nedrysoft::Core::IPingTarget *pingTarget) -> bool {

    Q_UNUSED(pingTarget)

    return true;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::start() -> bool {
    return true;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::stop() -> bool {
    return true;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::setInterval(std::chrono::milliseconds interval) -> bool {
    d->m_interval = interval;

    return true;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::interval() -> std::chrono::milliseconds {
    return d->m_interval;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::setTimeout(std::chrono::milliseconds timeout) -> bool {
    d->m_timeout = timeout;

    return true;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::epoch() -> std::chrono::system_clock::time_point {
    return std::chrono::system_clock::now();
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::singleShot(
        QHostAddress hostAddress,
        int ttl,
        double timeout ) -> Nedrysoft::Core::PingResult {

    QByteArray dataBuffer = QString("pingnoo ping").toLatin1();
    QByteArray replyBuffer(sizeof(ICMP_ECHO_REPLY) + dataBuffer.length(), 0);
    HANDLE icmpHandle;
    Nedrysoft::Core::PingResult::ResultCode resultCode = Nedrysoft::Core::PingResult::ResultCode::NoReply;
    Nedrysoft::Core::PingResult pingResult;
    QHostAddress replyHost;
    QElapsedTimer timer;
    qint64 started, finished;
    auto epoch = std::chrono::system_clock::now();

#if defined(_WIN64)
    IP_OPTION_INFORMATION32 options;
#else
    IP_OPTION_INFORMATION options;
#endif

    options.Ttl = ttl;
    options.Flags = 0;
    options.OptionsData = nullptr;
    options.OptionsSize = 0;
    options.Tos = 0;

    PIP_OPTION_INFORMATION pipOptions = reinterpret_cast<PIP_OPTION_INFORMATION>(&options);

    //TODO: ipV6

    icmpHandle = IcmpCreateFile();

    hostAddress.toIPv4Address();

    started = timer.nsecsElapsed();

    auto returnValue = IcmpSendEcho(icmpHandle, hostAddress.toIPv4Address(),
                                dataBuffer.data(), static_cast<WORD>(dataBuffer.length()), pipOptions,
                                replyBuffer.data(), static_cast<DWORD>(replyBuffer.length()),
                                std::chrono::duration<DWORD, std::milli>(
                                        DefaultTransmitTimeout).count()); // NOLINT(cppcoreguidelines-pro-type-union-access)

    finished = timer.nsecsElapsed();

    auto roundTripTime = static_cast<double>(finished - started) / nanosecondsInMillisecond;

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

    IcmpCloseHandle(icmpHandle);

    return Nedrysoft::Core::PingResult(
           0,
           resultCode,
           replyHost,
           epoch,
           std::chrono::duration<double, std::milli>(roundTripTime),
           nullptr);;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::targets() -> QList<Nedrysoft::Core::IPingTarget *> {
    QList<Nedrysoft::Core::IPingTarget *> list;

    for (auto target : d->m_pingTargets) {
        list.append(target);
    }

    return list;
}
