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

#include "ICMPAPIPingTransmitter.h"
#include "ICMPAPIPingTarget.h"

//#include <IPExport.h>
//#include <IcmpAPI.h>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
//#include <WS2tcpip.h>
#include <chrono>

using namespace std::chrono_literals;

constexpr auto DefaultReplyTimeout = 3s;

class Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineData {

    public:
        ICMPAPIPingEngineData(Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *parent) :
                m_pingEngine(parent),
                m_transmitter(nullptr),
                m_transmitterThread(nullptr),
                m_timeout(std::chrono::milliseconds(0)),
                //m_timeout(DefaultReplyTimeout),
                m_ipVersion(Nedrysoft::Core::IPVersion::V4) {

        }

        friend class ICMPAPIPingEngine;

    private:
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *m_pingEngine;

        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTransmitter *m_transmitter;
        /*FZICMPPingReceiver *m_receiver;

        FZICMPPingTimeout *m_timeout;

        QThread *m_receiverThread;
        QThread *m_timeoutThread;*/
        QThread *m_transmitterThread;

        QMap<uint32_t, Nedrysoft::ICMPAPIPingEngine::ICMPPingItem *> m_pingRequests;
        QMutex m_requestsMutex;
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

    ICMPAPIPingTarget *target = new ICMPAPIPingTarget(this, hostAddress);

    d->m_transmitter->addTarget(target);

    return(target);
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::addTarget(
        QHostAddress hostAddress,
        int ttl ) -> Nedrysoft::Core::IPingTarget * {

    ICMPAPIPingTarget *target = new ICMPAPIPingTarget(this, hostAddress, ttl);

    d->m_transmitter->addTarget(target);

    return(target);
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::removeTarget(
        Nedrysoft::Core::IPingTarget *target) -> bool {

    Q_UNUSED(target)

    return true;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::start() -> bool {
    return true;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::stop() -> bool {
    return true;
}

/*
void FZICMPAPIPingEngine::addRequest(FZICMPAPIPingItem *pingItem)
{
    Q_D(FZICMPAPIPingEngine);

    QMutexLocker locker(&d->m_requestsMutex);

    uint32_t id = (pingItem->id()<<16) | pingItem->sequenceId();

    d->m_pingRequests[id] = pingItem;
}


void FZICMPPingEngine::removeRequest(FZICMPAPIPingItem *pingItem)
{
    Q_D(FZICMPAPIPingEngine);

    QMutexLocker locker(&d->m_requestsMutex);

    uint32_t id = (pingItem->id()<<16) | pingItem->sequenceId();

    if (d->m_pingRequests.contains(id))
    {
        d->m_pingRequests.remove(id);

        pingItem->deleteLater();
    }
}

FZICMPPingItem *FZICMPPingEngine::getRequest(uint32_t id)
{
    Q_D(FZICMPAPIPingEngine);

    QMutexLocker locker(&d->m_requestsMutex);

    if (d->m_pingRequests.contains(id))
        return(d->m_pingRequests[id]);

    return(NULL);
}
*/

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

    return Nedrysoft::Core::PingResult();
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine::targets() -> QList<Nedrysoft::Core::IPingTarget *> {
    return QList<Nedrysoft::Core::IPingTarget *>();
}

/*
void FZICMPAPIPingEngine::timeoutRequests(void)
{
    Q_D(FZICMPAPIPingEngine);

    QMutexLocker locker(&d->m_requestsMutex);
    FZICMPPingItem *pingItem;
    QMutableMapIterator<uint32_t, FZICMPPingItem *> i(d->m_pingRequests);

    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    while (i.hasNext()) {
        i.next();

        pingItem = i.value();

        std::chrono::duration<double> diff = startTime-pingItem->transmitTime();

        if (diff.count()>((double) d->m_timeoutTime/1000.0f))
        {
            if (!pingItem->serviced())
            {
                FZPingResult pingResult(pingItem->sampleNumber(), FZPingResult::NoReply, QHostAddress(), (double) d->m_timeoutTime/1000.0f, pingItem->target());

                emit result(pingResult);

                i.remove();
            }
        }
    }
}
*/
