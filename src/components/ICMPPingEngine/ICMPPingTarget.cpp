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
#include "ICMPPingTarget.h"
#include "ICMPSocket/ICMPSocket.h"

#include <QHostAddress>
#include <cassert>
#include <random>

class Nedrysoft::ICMPPingEngine::ICMPPingTargetData {
    public:
        ICMPPingTargetData(Nedrysoft::ICMPPingEngine::ICMPPingTarget *parent) :
                m_pingTarget(parent),
                m_engine(nullptr),
                m_socket(nullptr),
                m_userData(nullptr),
                m_ttl(0) {

            std::random_device randomDevice;
            std::mt19937 mt(randomDevice());
            std::uniform_int_distribution<uint16_t> dist(1.0, UINT16_MAX-1);

            m_id = dist(mt);
        }

        friend class ICMPPingTarget;

    private:
        Nedrysoft::ICMPPingEngine::ICMPPingTarget *m_pingTarget;

        QHostAddress m_hostAddress;
        Nedrysoft::ICMPPingEngine::ICMPPingEngine *m_engine;
        Nedrysoft::ICMPSocket::ICMPSocket *m_socket;
        uint16_t m_id;
        void *m_userData;
        int m_ttl;
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
    if (d->m_socket) {
        delete d->m_socket;
    }

    d.reset();
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::setHostAddress(QHostAddress hostAddress) -> void {
    d->m_hostAddress = hostAddress;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::hostAddress() -> QHostAddress {
    return d->m_hostAddress;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::engine() -> Nedrysoft::Core::IPingEngine * {
    return d->m_engine;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::socket() -> Nedrysoft::ICMPSocket::ICMPSocket * {
    if (d->m_socket==nullptr) {
        if (d->m_hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
            d->m_socket = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(
                    d->m_ttl,
                    Nedrysoft::ICMPSocket::V4 );
        } else if (d->m_hostAddress.protocol() == QAbstractSocket::IPv6Protocol) {
            d->m_socket = Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(
                    d->m_ttl,
                    Nedrysoft::ICMPSocket::V6);
        }
    }

    return d->m_socket;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::id() -> uint16_t {
    return d->m_id;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::ttl() -> uint16_t {
    return d->m_ttl;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::userData() -> void * {
    return d->m_userData;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::setUserData(void *data) -> void {
    d->m_userData = data;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::ICMPPingEngine::ICMPPingTarget::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}
