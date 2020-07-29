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

#include "ICMPPingEngine.h"
#include "ICMPPingReceiver.h"
#include "ICMPPingTransmitter.h"
#include "ICMPPingItem.h"
#include "ICMPPingTarget.h"
#include "ICMPPingTimeout.h"
#include "Utils.h"
#include <chrono>
#include <cstdint>
#include <QThread>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>

using namespace std::chrono_literals;

constexpr auto DefaultReceiveTimeout = 3s;
constexpr auto DefaultTerminateThreadTimeout = 5s;

using seconds_double = std::chrono::duration<double>;

class FizzyAde::ICMPPingEngine::ICMPPingEngineData
{

public:
    ICMPPingEngineData(FizzyAde::ICMPPingEngine::ICMPPingEngine *parent)
    {
        m_pingEngine = parent;
        m_receiverWorker = nullptr;
        m_transmitterWorker = nullptr;
        m_timeoutWorker = nullptr;
        m_receiverThread = nullptr;
        m_transmitterThread = nullptr;
        m_timeoutThread = nullptr;
        m_timeout = DefaultReceiveTimeout;
        m_epoch = std::chrono::system_clock::now();
    }

    friend class ICMPPingEngine;

private:

    FizzyAde::ICMPPingEngine::ICMPPingEngine *m_pingEngine;

    FizzyAde::ICMPPingEngine::ICMPPingReceiver *m_receiverWorker;
    FizzyAde::ICMPPingEngine::ICMPPingTransmitter *m_transmitterWorker;
    FizzyAde::ICMPPingEngine::ICMPPingTimeout *m_timeoutWorker;

    QThread *m_receiverThread;
    QThread *m_transmitterThread;
    QThread *m_timeoutThread;

    QMap<uint32_t, FizzyAde::ICMPPingEngine::ICMPPingItem *> m_pingRequests;
    QMutex m_requestsMutex;

    QList<FizzyAde::ICMPPingEngine::ICMPPingTarget *> m_targetList;

    std::chrono::milliseconds m_timeout = {};

    std::chrono::system_clock::time_point m_epoch;

    FizzyAde::Core::IPVersion m_version;
};

FizzyAde::ICMPPingEngine::ICMPPingEngine::ICMPPingEngine(FizzyAde::Core::IPVersion version) :
    d(std::make_shared<FizzyAde::ICMPPingEngine::ICMPPingEngineData>(this))
{
    d->m_version = version;

    // timeout thread

    d->m_timeoutWorker = new FizzyAde::ICMPPingEngine::ICMPPingTimeout(this);

    d->m_timeoutThread = new QThread();

    d->m_timeoutWorker->moveToThread(d->m_timeoutThread);

    connect(d->m_timeoutThread, &QThread::started, d->m_timeoutWorker, &FizzyAde::ICMPPingEngine::ICMPPingTimeout::doWork);

    connect(d->m_timeoutWorker, &FizzyAde::ICMPPingEngine::ICMPPingTimeout::result, this, &FizzyAde::ICMPPingEngine::ICMPPingEngine::result);

    d->m_timeoutThread->start();

    // receiver thread

    d->m_receiverWorker = new FizzyAde::ICMPPingEngine::ICMPPingReceiver(this);

    d->m_receiverThread = new QThread();

    d->m_receiverWorker->moveToThread(d->m_receiverThread);

    connect(d->m_receiverThread, &QThread::started, d->m_receiverWorker, &FizzyAde::ICMPPingEngine::ICMPPingReceiver::doWork);

    connect(d->m_receiverWorker, &FizzyAde::ICMPPingEngine::ICMPPingReceiver::result, this, &FizzyAde::ICMPPingEngine::ICMPPingEngine::result);

    d->m_receiverThread->start();

    // transmitter thread

    d->m_transmitterWorker = new FizzyAde::ICMPPingEngine::ICMPPingTransmitter(this);

    d->m_transmitterThread = new QThread();

    d->m_transmitterWorker->moveToThread(d->m_transmitterThread);

    connect(d->m_transmitterThread, &QThread::started, d->m_transmitterWorker, &FizzyAde::ICMPPingEngine::ICMPPingTransmitter::doWork);

    connect(d->m_transmitterWorker, &FizzyAde::ICMPPingEngine::ICMPPingTransmitter::result, this, &FizzyAde::ICMPPingEngine::ICMPPingEngine::result);

    d->m_transmitterThread->start();
}

FizzyAde::ICMPPingEngine::ICMPPingEngine::~ICMPPingEngine()
{
    auto waitTime = std::chrono::duration_cast<std::chrono::milliseconds>(DefaultTerminateThreadTimeout);

    if (d->m_transmitterWorker) {
        d->m_transmitterWorker->m_isRunning = false;
    }

    if (d->m_transmitterThread) {
        d->m_transmitterThread->quit();
    }

    if (d->m_receiverWorker) {
        d->m_receiverWorker->m_isRunning = false;
    }

    if (d->m_receiverThread) {
        d->m_receiverThread->quit();
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
    }

    if (d->m_receiverThread) {
        d->m_receiverThread->wait(waitTime.count());

        if (d->m_receiverThread->isRunning()) {
            d->m_receiverThread->terminate();
        }

        delete d->m_receiverThread;
    }

    if (d->m_timeoutThread) {
        d->m_timeoutThread->wait(waitTime.count());

        if (d->m_timeoutThread->isRunning()) {
            d->m_timeoutThread->terminate();
        }

        delete d->m_timeoutThread;
    }

    delete d->m_transmitterWorker;
    delete d->m_receiverWorker;
    delete d->m_timeoutWorker;

    for(auto target : d->m_targetList) {
        delete target;
    }

    for (auto request : d->m_pingRequests) {
        delete request;
    }
}

FizzyAde::Core::IPingTarget *FizzyAde::ICMPPingEngine::ICMPPingEngine::addTarget(QHostAddress hostAddress)
{
    auto target = new FizzyAde::ICMPPingEngine::ICMPPingTarget(this, hostAddress);

    d->m_transmitterWorker->addTarget(target);

    return target;
}

FizzyAde::Core::IPingTarget *FizzyAde::ICMPPingEngine::ICMPPingEngine::addTarget(QHostAddress hostAddress, int ttl)
{
    auto target = new FizzyAde::ICMPPingEngine::ICMPPingTarget(this, hostAddress, ttl);

    d->m_transmitterWorker->addTarget(target);

    d->m_targetList.append(target);

    return target;
}

bool FizzyAde::ICMPPingEngine::ICMPPingEngine::removeTarget(FizzyAde::Core::IPingTarget *target)
{
    Q_UNUSED(target)

    return true;
}

bool FizzyAde::ICMPPingEngine::ICMPPingEngine::start()
{
    return true;
}

bool FizzyAde::ICMPPingEngine::ICMPPingEngine::stop()
{
    return true;
}

void FizzyAde::ICMPPingEngine::ICMPPingEngine::addRequest(FizzyAde::ICMPPingEngine::ICMPPingItem *pingItem)
{
    QMutexLocker locker(&d->m_requestsMutex);

    auto id = FizzyAde::Utils::fzMake32(pingItem->id(), pingItem->sequenceId());

    d->m_pingRequests[id] = pingItem;
}


void FizzyAde::ICMPPingEngine::ICMPPingEngine::removeRequest(FizzyAde::ICMPPingEngine::ICMPPingItem *pingItem)
{
    QMutexLocker locker(&d->m_requestsMutex);

    auto id = FizzyAde::Utils::fzMake32(pingItem->id(), pingItem->sequenceId());

    if (d->m_pingRequests.contains(id)) {
        d->m_pingRequests.remove(id);

        pingItem->deleteLater();
    }
}

FizzyAde::ICMPPingEngine::ICMPPingItem *FizzyAde::ICMPPingEngine::ICMPPingEngine::getRequest(uint32_t id)
{
    QMutexLocker locker(&d->m_requestsMutex);

    if (d->m_pingRequests.contains(id)) {
        return d->m_pingRequests[id];
    }

    return nullptr;
}

bool FizzyAde::ICMPPingEngine::ICMPPingEngine::setInterval(std::chrono::milliseconds interval)
{
    return d->m_transmitterWorker->setInterval(interval);
}

bool FizzyAde::ICMPPingEngine::ICMPPingEngine::setTimeout(std::chrono::milliseconds timeout)
{
    d->m_timeout = timeout;

    return true;
}

void FizzyAde::ICMPPingEngine::ICMPPingEngine::timeoutRequests()
{
    QMutexLocker locker(&d->m_requestsMutex);
    QMutableMapIterator<uint32_t, FizzyAde::ICMPPingEngine::ICMPPingItem *> i(d->m_pingRequests);

    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    while (i.hasNext()) {
        i.next();

        auto pingItem = i.value();

        std::chrono::duration<double> diff = startTime-pingItem->transmitTime();

        if (diff>d->m_timeout) {
            pingItem->lock();
            if (!pingItem->serviced()) {
                pingItem->setServiced(true);
                pingItem->unlock();

                FizzyAde::Core::PingResult pingResult(pingItem->sampleNumber(), FizzyAde::Core::PingResult::NoReply, QHostAddress(), pingItem->transmitEpoch(), diff, pingItem->target());

                emit result(pingResult);

                i.remove();

                delete pingItem;
            } else {
                pingItem->unlock();
            }
        }
    }
}

QJsonObject FizzyAde::ICMPPingEngine::ICMPPingEngine::saveConfiguration()
{
    return QJsonObject();
}

bool FizzyAde::ICMPPingEngine::ICMPPingEngine::loadConfiguration(QJsonObject configuration)
{
    Q_UNUSED(configuration)

    return false;
}

void FizzyAde::ICMPPingEngine::ICMPPingEngine::setEpoch(std::chrono::system_clock::time_point epoch)
{
    d->m_epoch = epoch;
}

std::chrono::system_clock::time_point FizzyAde::ICMPPingEngine::ICMPPingEngine::epoch()
{
    return d->m_epoch;
}

FizzyAde::Core::IPVersion FizzyAde::ICMPPingEngine::ICMPPingEngine::version()
{
    return d->m_version;
}
