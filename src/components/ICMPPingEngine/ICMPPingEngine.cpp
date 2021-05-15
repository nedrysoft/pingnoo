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

#include "ICMPPingEngine.h"

#include "ICMPPingItem.h"
#include "ICMPPingReceiverWorker.h"
#include "ICMPPingTarget.h"
#include "ICMPPingTimeout.h"
#include "ICMPPingTransmitter.h"
#include "ICMPSocket/ICMPSocket.h"
#include "ICMPPacket/ICMPPacket.h"
#include "Utils.h"

#include <QElapsedTimer>
#include <QMap>
#include <QMutex>
#include <QThread>
#include <chrono>
#include <cstdint>

using namespace std::chrono_literals;

constexpr auto DefaultReceiveTimeout = 1s;
constexpr auto DefaultTerminateThreadTimeout = 5s;
constexpr auto DefaultTransmitInterval = 2.5s;

constexpr auto SecondsToMs(double seconds) {
    return seconds*1000;
}

/**
 * @brief       Private class to store the ping engines instance data.
 */
class Nedrysoft::ICMPPingEngine::ICMPPingEngineData {
    public:
        /**
         * @brief       Constructs a ICMPPingEngineData.
         *
         * @param[in]   parent the ICMPPingEngine instance that this data belongs to.
         */
        ICMPPingEngineData(Nedrysoft::ICMPPingEngine::ICMPPingEngine *parent) :
                m_pingEngine(parent),
                m_transmitterWorker(nullptr),
                m_timeoutWorker(nullptr),
                m_transmitterThread(nullptr),
                m_timeoutThread(nullptr),
                m_timeout(DefaultReceiveTimeout),
                m_epoch(std::chrono::system_clock::now()),
                m_receiverWorker(nullptr) {

        }

        friend class ICMPPingEngine;

    private:

        Nedrysoft::ICMPPingEngine::ICMPPingEngine *m_pingEngine;

        Nedrysoft::ICMPPingEngine::ICMPPingTransmitter *m_transmitterWorker;
        Nedrysoft::ICMPPingEngine::ICMPPingTimeout *m_timeoutWorker;

        QThread *m_transmitterThread;
        QThread *m_timeoutThread;

        QMap<uint32_t, Nedrysoft::ICMPPingEngine::ICMPPingItem *> m_pingRequests;
        QMutex m_requestsMutex;

        QList<Nedrysoft::ICMPPingEngine::ICMPPingTarget *> m_targetList;

        std::chrono::milliseconds m_timeout = {};

        std::chrono::milliseconds m_interval = 2500ms;

        std::chrono::system_clock::time_point m_epoch;

        Nedrysoft::Core::IPVersion m_version;

        Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker *m_receiverWorker;
};

Nedrysoft::ICMPPingEngine::ICMPPingEngine::ICMPPingEngine(Nedrysoft::Core::IPVersion version) :
        d(std::make_shared<Nedrysoft::ICMPPingEngine::ICMPPingEngineData>(this)) {

    d->m_version = version;
}

Nedrysoft::ICMPPingEngine::ICMPPingEngine::~ICMPPingEngine() {
    doStop();

    d.reset();
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::addTarget(
        QHostAddress hostAddress) -> Nedrysoft::RouteAnalyser::IPingTarget * {

    auto target = new Nedrysoft::ICMPPingEngine::ICMPPingTarget(this, hostAddress);

    d->m_transmitterWorker->addTarget(target);

    return target;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::addTarget(
        QHostAddress hostAddress,
        int ttl) -> Nedrysoft::RouteAnalyser::IPingTarget * {

    auto target = new Nedrysoft::ICMPPingEngine::ICMPPingTarget(this, hostAddress, ttl);

    d->m_targetList.append(target);

    return target;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::removeTarget(Nedrysoft::RouteAnalyser::IPingTarget *target) -> bool {
    Q_UNUSED(target)

    return true;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::start() -> bool {
    // timeout thread

    d->m_timeoutWorker = new Nedrysoft::ICMPPingEngine::ICMPPingTimeout(this);

    d->m_timeoutThread = new QThread();

    d->m_timeoutWorker->moveToThread(d->m_timeoutThread);

    connect(d->m_timeoutThread, &QThread::started, d->m_timeoutWorker,
            &Nedrysoft::ICMPPingEngine::ICMPPingTimeout::doWork);

    connect(d->m_timeoutWorker, &Nedrysoft::ICMPPingEngine::ICMPPingTimeout::result, this,
            &Nedrysoft::ICMPPingEngine::ICMPPingEngine::result);

    d->m_timeoutThread->start();

    // connect to the receiver thread

    d->m_receiverWorker = Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::getInstance();

    connect(d->m_receiverWorker,
            &Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker::packetReceived,
            this,
            &Nedrysoft::ICMPPingEngine::ICMPPingEngine::onPacketReceived);

    // transmitter thread

    d->m_transmitterWorker = new Nedrysoft::ICMPPingEngine::ICMPPingTransmitter(this);

    d->m_transmitterThread = new QThread();

    d->m_transmitterWorker->moveToThread(d->m_transmitterThread);

    d->m_transmitterWorker->setInterval(d->m_interval);

    for (auto target : d->m_targetList) {
        d->m_transmitterWorker->addTarget(target);
    }

    connect(d->m_transmitterThread, &QThread::started, d->m_transmitterWorker,
            &Nedrysoft::ICMPPingEngine::ICMPPingTransmitter::doWork);

    connect(d->m_transmitterWorker, &Nedrysoft::ICMPPingEngine::ICMPPingTransmitter::result, this,
            &Nedrysoft::ICMPPingEngine::ICMPPingEngine::result);

    d->m_transmitterThread->start();

    return true;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::doStop() -> bool {
    auto waitTime = std::chrono::duration_cast<std::chrono::milliseconds>(DefaultTerminateThreadTimeout);

    if (d->m_transmitterWorker) {
        d->m_transmitterWorker->m_isRunning = false;
    }

    if (d->m_transmitterThread) {
        d->m_transmitterThread->quit();
    }

    if (d->m_timeoutWorker) {
        d->m_timeoutWorker->m_isRunning = false;
    }

    if (d->m_timeoutThread) {
        d->m_timeoutThread->quit();
    }

    if (d->m_transmitterThread) {
        d->m_transmitterThread->wait(waitTime.count());

        if (d->m_transmitterThread->isRunning()) {
            d->m_transmitterThread->terminate();
        }

        delete d->m_transmitterThread;

        d->m_transmitterThread = nullptr;
    }

    if (d->m_timeoutThread) {
        d->m_timeoutThread->wait(waitTime.count());

        if (d->m_timeoutThread->isRunning()) {
            d->m_timeoutThread->terminate();
        }

        delete d->m_timeoutThread;

        d->m_timeoutThread = nullptr;
    }

    delete d->m_transmitterWorker;
    delete d->m_timeoutWorker;

    d->m_transmitterWorker = nullptr;
    d->m_timeoutWorker = nullptr;

    for (auto request : d->m_pingRequests) {
        delete request;
    }

    d->m_pingRequests.clear();

    return true;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::stop() -> bool {
    return doStop();
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::addRequest(Nedrysoft::ICMPPingEngine::ICMPPingItem *pingItem) -> void {
    QMutexLocker locker(&d->m_requestsMutex);

    auto id = Nedrysoft::Utils::fzMake32(pingItem->id(), pingItem->sequenceId());

    d->m_pingRequests[id] = pingItem;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::removeRequest(
        Nedrysoft::ICMPPingEngine::ICMPPingItem *pingItem ) -> void {

    QMutexLocker locker(&d->m_requestsMutex);

    auto id = Nedrysoft::Utils::fzMake32(pingItem->id(), pingItem->sequenceId());

    if (d->m_pingRequests.contains(id)) {
        d->m_pingRequests.remove(id);

        delete pingItem;
    }
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::getRequest(uint32_t id) -> Nedrysoft::ICMPPingEngine::ICMPPingItem * {
    QMutexLocker locker(&d->m_requestsMutex);

    if (d->m_pingRequests.contains(id)) {
        return d->m_pingRequests[id];
    }

    return nullptr;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::setInterval(std::chrono::milliseconds interval) -> bool {
    d->m_interval = std::chrono::duration_cast<std::chrono::milliseconds>(interval);

    return true;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::setTimeout(std::chrono::milliseconds timeout) -> bool {
    d->m_timeout = timeout;

    return true;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::timeoutRequests() -> void {
    QMutexLocker locker(&d->m_requestsMutex);
    QMutableMapIterator<uint32_t, Nedrysoft::ICMPPingEngine::ICMPPingItem *> i(d->m_pingRequests);

    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    while (i.hasNext()) {
        i.next();

        auto pingItem = i.value();

        std::chrono::duration<double> diff = startTime - pingItem->transmitTime();

        if (diff > d->m_timeout) {
            if (pingItem->lock()) {
                if (!pingItem->serviced()) {
                    QHostAddress hostAddress;

                    pingItem->setServiced(true);
                    pingItem->unlock();

                    Nedrysoft::RouteAnalyser::PingResult pingResult(
                            pingItem->sampleNumber(),
                            Nedrysoft::RouteAnalyser::PingResult::ResultCode::NoReply,
                            hostAddress,
                            pingItem->transmitEpoch(),
                            diff,
                            pingItem->target());

                    Q_EMIT result(pingResult);

                    i.remove();
                } else {
                    pingItem->unlock();
                }
            }
        }
    }
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::setEpoch(std::chrono::system_clock::time_point epoch) -> void {
    d->m_epoch = epoch;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::epoch() -> std::chrono::system_clock::time_point {
    return d->m_epoch;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::version() -> Nedrysoft::Core::IPVersion {
    return d->m_version;
}

void Nedrysoft::ICMPPingEngine::ICMPPingEngine::onPacketReceived(
        std::chrono::time_point < std::chrono::high_resolution_clock > receiveTime,
        QByteArray receiveBuffer,
        QHostAddress receiveAddress ) {

    Nedrysoft::RouteAnalyser::PingResult::ResultCode resultCode =
            Nedrysoft::RouteAnalyser::PingResult::ResultCode::NoReply;

    auto responsePacket = Nedrysoft::ICMPPacket::ICMPPacket::fromData(
            receiveBuffer,
            static_cast<Nedrysoft::ICMPPacket::IPVersion>(this->version()) );

    if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::Invalid) {
        return;
    }

    if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::EchoReply) {
        resultCode = Nedrysoft::RouteAnalyser::PingResult::ResultCode::Ok;
    }

    if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::TimeExceeded) {
        resultCode = Nedrysoft::RouteAnalyser::PingResult::ResultCode::TimeExceeded;
    }

    auto pingItem = this->getRequest(Nedrysoft::Utils::fzMake32(responsePacket.id(), responsePacket.sequence()));

    if (pingItem) {
        pingItem->lock();

        if (!pingItem->serviced()) {
            std::chrono::duration<double> diff = receiveTime - pingItem->transmitTime();

            auto pingResult = Nedrysoft::RouteAnalyser::PingResult(
                    pingItem->sampleNumber(),
                    resultCode,
                    receiveAddress,
                    pingItem->transmitEpoch(), diff, pingItem->target() );

            Q_EMIT Nedrysoft::ICMPPingEngine::ICMPPingEngine::result(pingResult);

            pingItem->setServiced(true);
            pingItem->unlock();
        } else {
            pingItem->unlock();
        }
    }
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::interval() -> std::chrono::milliseconds {
    return d->m_transmitterWorker->interval();
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::targets() -> QList<Nedrysoft::RouteAnalyser::IPingTarget *> {
    QList<Nedrysoft::RouteAnalyser::IPingTarget *> list;

    for (auto target : d->m_targetList) {
        list.append(target);
    }

    return list;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingEngine::singleShot(
        QHostAddress hostAddress,
        int ttl,
        double timeout ) -> Nedrysoft::RouteAnalyser::PingResult {

    Nedrysoft::ICMPSocket::ICMPSocket *writeSocket;
    Nedrysoft::ICMPSocket::ICMPSocket *readSocket;

    Nedrysoft::RouteAnalyser::PingResult pingResult;

    if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
        writeSocket = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(ttl, Nedrysoft::ICMPSocket::V4);
    } else if (hostAddress.protocol() == QAbstractSocket::IPv6Protocol) {
        writeSocket = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(ttl, Nedrysoft::ICMPSocket::V6);
    }

    if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
        readSocket = Nedrysoft::ICMPSocket::ICMPSocket::createReadSocket(Nedrysoft::ICMPSocket::V4);
    } else if (hostAddress.protocol() == QAbstractSocket::IPv6Protocol) {
        readSocket = Nedrysoft::ICMPSocket::ICMPSocket::createReadSocket(Nedrysoft::ICMPSocket::V6);
    }

    QByteArray receiveBuffer;

    // TODO: fix

    int id = 6666;
    int sequenceId = 5555 + ttl;

    auto buffer = Nedrysoft::ICMPPacket::ICMPPacket::pingPacket(
            id,
            sequenceId,
            52,
            hostAddress,
            static_cast<Nedrysoft::ICMPPacket::IPVersion>(version()));

    auto requestTime = std::chrono::high_resolution_clock::now();
    auto transmitEpoch = std::chrono::system_clock::now();

    writeSocket->sendto(buffer, hostAddress);

    QHostAddress receiveAddress;

    QElapsedTimer timeoutTimer;

    timeoutTimer.start();

    while(timeoutTimer.elapsed()<(SecondsToMs(timeout))) {
        auto remaining = DefaultReceiveTimeout-std::chrono::milliseconds(timeoutTimer.elapsed());

        if (remaining.count()<=0) {
            return pingResult;
        }

        if (readSocket->recvfrom(receiveBuffer, receiveAddress, remaining)) {
            auto responseTime = std::chrono::high_resolution_clock::now();

            Nedrysoft::RouteAnalyser::PingResult::ResultCode resultCode;

            if (!receiveBuffer.length()) {
                continue;
            }

            auto responsePacket = Nedrysoft::ICMPPacket::ICMPPacket::fromData(
                    receiveBuffer,
                    static_cast<Nedrysoft::ICMPPacket::IPVersion>(this->version()) );

            if ((responsePacket.id()!=id) || (responsePacket.sequence()!=sequenceId)) {
                continue;
            }

            if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::Invalid) {
                continue;
            }

            if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::EchoReply) {
                resultCode = Nedrysoft::RouteAnalyser::PingResult::ResultCode::Ok;
            }

            if (responsePacket.resultCode() == Nedrysoft::ICMPPacket::TimeExceeded) {
                resultCode = Nedrysoft::RouteAnalyser::PingResult::ResultCode::TimeExceeded;
            }

            auto roundTripTime = std::chrono::duration<double, std::milli>(responseTime-requestTime);

            pingResult = Nedrysoft::RouteAnalyser::PingResult(
                0,
                resultCode,
                receiveAddress,
                transmitEpoch,
                roundTripTime,
                nullptr );

            break;
        }
    }

    delete writeSocket;
    delete readSocket;

    return pingResult;
}