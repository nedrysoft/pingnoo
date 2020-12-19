/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
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
#include "ICMPPingTarget.h"
#include "ICMPSocket/ICMPSocket.h"

#include <QHostAddress>
#include <QRandomGenerator>
#include <cassert>

constexpr int TotalTargetSockets = 255;

class Nedrysoft::ICMPPingEngine::ICMPPingTargetData {
    public:
        ICMPPingTargetData(Nedrysoft::ICMPPingEngine::ICMPPingTarget *parent) :
                m_pingTarget(parent),
                m_engine(nullptr),
                m_id(( QRandomGenerator::global()->generate() % ( UINT16_MAX - 1 )) + 1),
                m_userData(nullptr),
                m_ttl(0),
                m_currentSocket(0) {

            for (auto currentSocket = 0; currentSocket < TotalTargetSockets; currentSocket++) {
                m_socketList.append(nullptr);
            }
        }

        friend class ICMPPingTarget;

    private:
        Nedrysoft::ICMPPingEngine::ICMPPingTarget *m_pingTarget;

        QHostAddress m_hostAddress;
        Nedrysoft::ICMPPingEngine::ICMPPingEngine *m_engine;
        QList<Nedrysoft::ICMPSocket::ICMPSocket *> m_socketList;
        uint16_t m_id;
        void *m_userData;
        int m_ttl;
        int m_currentSocket;
};

Nedrysoft::ICMPPingEngine::ICMPPingTarget::ICMPPingTarget(
        Nedrysoft::ICMPPingEngine::ICMPPingEngine *engine,
        QHostAddress hostAddress,
        int ttl) :

            d(std::make_shared<Nedrysoft::ICMPPingEngine::ICMPPingTargetData>(this)) {

    d->m_hostAddress = std::move(hostAddress);
    d->m_engine = engine;
    d->m_ttl = ttl;
}

Nedrysoft::ICMPPingEngine::ICMPPingTarget::~ICMPPingTarget() {
    for (auto socket : d->m_socketList) {
        delete socket;
    }
}

void Nedrysoft::ICMPPingEngine::ICMPPingTarget::setHostAddress(QHostAddress hostAddress) {
    d->m_hostAddress = hostAddress;
}

QHostAddress Nedrysoft::ICMPPingEngine::ICMPPingTarget::hostAddress() {
    return d->m_hostAddress;
}

Nedrysoft::Core::IPingEngine *Nedrysoft::ICMPPingEngine::ICMPPingTarget::engine() {
    return d->m_engine;
}

Nedrysoft::ICMPSocket::ICMPSocket *Nedrysoft::ICMPPingEngine::ICMPPingTarget::socket() {
    if (d->m_socketList[d->m_currentSocket] == 0) {
        if (d->m_hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
            d->m_socketList[d->m_currentSocket] = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(
                    d->m_ttl,
                    Nedrysoft::ICMPSocket::V4 );
        } else if (d->m_hostAddress.protocol() == QAbstractSocket::IPv6Protocol) {
            d->m_socketList[d->m_currentSocket] = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(
                    d->m_ttl,
                    Nedrysoft::ICMPSocket::V6 );
        }
    }

    assert(d->m_socketList[d->m_currentSocket]!=nullptr);

    auto socket = d->m_socketList[d->m_currentSocket];

    d->m_currentSocket = ( d->m_currentSocket + 1 ) % d->m_socketList.count();

    return socket;
}

uint16_t Nedrysoft::ICMPPingEngine::ICMPPingTarget::id() {
    return d->m_id;
}

uint16_t Nedrysoft::ICMPPingEngine::ICMPPingTarget::ttl() {
    return d->m_ttl;
}

void *Nedrysoft::ICMPPingEngine::ICMPPingTarget::userData() {
    return d->m_userData;
}

void Nedrysoft::ICMPPingEngine::ICMPPingTarget::setUserData(void *data) {
    d->m_userData = data;
}

QJsonObject Nedrysoft::ICMPPingEngine::ICMPPingTarget::saveConfiguration() {
    return QJsonObject();
}

bool Nedrysoft::ICMPPingEngine::ICMPPingTarget::loadConfiguration(QJsonObject configuration) {
    Q_UNUSED(configuration)

    return false;
}
