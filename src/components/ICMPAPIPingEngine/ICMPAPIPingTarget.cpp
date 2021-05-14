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

#include "ICMPAPIPingTarget.h"

#include "ICMPAPIPingEngine.h"

#include <QHostAddress>
#include <QRandomGenerator>

constexpr int TotalTargetSockets = 10;

class Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTargetData {
    public:
        ICMPAPIPingTargetData(Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *parent) :
                m_pingTarget(parent),
                m_engine(nullptr),
                m_id(( QRandomGenerator::global()->generate() % ( UINT16_MAX - 1 )) + 1),
                m_userData(nullptr),
                m_ttl(0) {

        }

        friend class ICMPAPIPingTarget;

    private:
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *m_pingTarget;

        QHostAddress m_hostAddress;
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *m_engine;

        uint16_t m_id;
        void *m_userData;
        unsigned int m_ttl;
};

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::ICMPAPIPingTarget(
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *engine,
        const QHostAddress &hostAddress,
        int ttl) :

            d(std::make_shared<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTargetData>(this)) {

    d->m_hostAddress = hostAddress;
    d->m_engine = engine;
    d->m_ttl = ttl;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::setHostAddress(QHostAddress hostAddress) -> void {
    d->m_hostAddress = hostAddress;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::hostAddress() -> QHostAddress {
    return d->m_hostAddress;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::engine() -> Nedrysoft::RouteAnalyser::IPingEngine * {
    return d->m_engine;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::id() -> uint16_t {
    return d->m_id;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::userData() -> void * {
    return d->m_userData;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::setUserData(void *data) -> void {
    d->m_userData = data;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::ttl() -> unsigned short {
    return d->m_ttl;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}
