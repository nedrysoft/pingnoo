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

#include "ICMPPingTarget.h"
#include "ICMPPingEngine.h"
#include "ICMPSocket/ICMPSocket.h"
#include <cerrno>
#include <fcntl.h>
#include <QHostAddress>
#include <QRandomGenerator>

constexpr int TotalTargetSockets = 10;

class FizzyAde::ICMPPingEngine::ICMPPingTargetData
{
public:
    ICMPPingTargetData(FizzyAde::ICMPPingEngine::ICMPPingTarget* parent)
    {
        m_pingTarget = parent;

        for (auto i=0;i<TotalTargetSockets;i++) {
            m_socketList.append(nullptr);
        }

        m_engine = nullptr;
        m_id = (QRandomGenerator::global()->generate() % (UINT16_MAX-1))+1;
        m_userData = nullptr;
        m_ttl = 0;
        m_currentSocket = 0;
    }

    friend class ICMPPingTarget;

private:
    FizzyAde::ICMPPingEngine::ICMPPingTarget* m_pingTarget;

    QHostAddress m_hostAddress;
    FizzyAde::ICMPPingEngine::ICMPPingEngine *m_engine;
    QList<FizzyAde::ICMPSocket::ICMPSocket *> m_socketList;
    uint16_t m_id;
    void *m_userData;
    int m_ttl;
    int m_currentSocket;
};

FizzyAde::ICMPPingEngine::ICMPPingTarget::ICMPPingTarget(FizzyAde::ICMPPingEngine::ICMPPingEngine *engine, QHostAddress hostAddress, int ttl) :
    d(std::make_shared<FizzyAde::ICMPPingEngine::ICMPPingTargetData>(this))
{
    d->m_hostAddress = std::move(hostAddress);
    d->m_engine = engine;
    d->m_ttl = ttl;
}

FizzyAde::ICMPPingEngine::ICMPPingTarget::~ICMPPingTarget()
{
    for(auto socket : d->m_socketList) {
        delete socket;
    }
}

void FizzyAde::ICMPPingEngine::ICMPPingTarget::setHostAddress(QHostAddress hostAddress)
{
    d->m_hostAddress = hostAddress;
}

QHostAddress FizzyAde::ICMPPingEngine::ICMPPingTarget::hostAddress()
{
    return(d->m_hostAddress);
}

FizzyAde::Core::IPingEngine *FizzyAde::ICMPPingEngine::ICMPPingTarget::engine()
{
    return(d->m_engine);
}

FizzyAde::ICMPSocket::ICMPSocket *FizzyAde::ICMPPingEngine::ICMPPingTarget::socket()
{
    if (d->m_socketList[d->m_currentSocket]==0) {
        if (d->m_hostAddress.protocol()==QAbstractSocket::IPv4Protocol) {
            d->m_socketList[d->m_currentSocket] = FizzyAde::ICMPSocket::ICMPSocket::createWriteSocket(d->m_ttl, FizzyAde::ICMPSocket::V4);
        } else if (d->m_hostAddress.protocol()==QAbstractSocket::IPv6Protocol) {
            d->m_socketList[d->m_currentSocket] = FizzyAde::ICMPSocket::ICMPSocket::createWriteSocket(d->m_ttl, FizzyAde::ICMPSocket::V6);
        }
    }

    auto socket = d->m_socketList[d->m_currentSocket];

    d->m_currentSocket = (d->m_currentSocket+1) % d->m_socketList.count();

    return(socket);
}

uint16_t FizzyAde::ICMPPingEngine::ICMPPingTarget::id()
{
    return(d->m_id);
}

void *FizzyAde::ICMPPingEngine::ICMPPingTarget::userData()
{
    return(d->m_userData);
}

void FizzyAde::ICMPPingEngine::ICMPPingTarget::setUserData(void *data)
{
    d->m_userData = data;
}

QJsonObject FizzyAde::ICMPPingEngine::ICMPPingTarget::saveConfiguration()
{
    return(QJsonObject());
}

bool FizzyAde::ICMPPingEngine::ICMPPingTarget::loadConfiguration(QJsonObject configuration)
{
    Q_UNUSED(configuration)

    return(false);
}
