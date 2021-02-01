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
#include <cerrno>
#include <fcntl.h>

#if defined(Q_OS_UNIX)

#include <arpa/inet.h>
#include <arpa/inet.h>
#include <cstdint>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

#elif defined(Q_OS_WIN)
#include "windows_ip_icmp.h"
#endif

constexpr int TotalTargetSockets = 10;

class Nedrysoft::Pingnoo::ICMPAPIPingTargetData {
    public:
        ICMPAPIPingTargetData(Nedrysoft::Pingnoo::ICMPAPIPingTarget *parent) :
                m_pingTarget(parent),
                m_engine(nullptr)
                m_id(( QRandomGenerator::global()->generate() % ( UINT16_MAX - 1 )) + 1),
                m_userData(nullptr),
                m_ttl(0),
                m_currentSocket(0) {

            for (int i = 0; i < TotalTargetSockets; i++)
                m_socketDescriptors.append(0);
        }

        friend class ICMPAPIPingTarget;

    private:
        Nedrysoft::Pingnoo::ICMPAPIPingTarget *m_pingTarget;

        QHostAddress m_hostAddress;
        Nedrysoft::Pingnoo::ICMPAPIPingEngine *m_engine;
#if defined(Q_OS_UNIX)
        QList<int> m_socketDescriptors;
#elif defined(Q_OS_WIN)
        QList<SOCKET> m_socketDescriptors;
#endif
        uint16_t m_id;
        void *m_userData;
        unsigned int m_ttl;
        int m_currentSocket;
};

Nedrysoft::Pingnoo::ICMPAPIPingTarget::ICMPAPIPingTarget(
        Nedrysoft::Pingnoo::ICMPAPIPingEngine *engine,
        const QHostAddress &hostAddress,
        int ttl) :

            d(std::make_shared<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTargetData>(this)) {

    d->m_hostAddress = hostAddress;
    d->m_engine = engine;
    d->m_ttl = ttl;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::asQObject() -> QObject * {
    return this;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::setHostAddress(const QHostAddress &hostAddress) -> void {
    d->m_hostAddress = hostAddress;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::hostAddress() -> QHostAddress {
    return d->m_hostAddress;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::engine() -> Nedrysoft::Pingnoo::IPingEngine * {
    return d->m_engine;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::socketDescriptor() -> SOCKET {
    if (d->m_socketDescriptors[d->m_currentSocket] == 0) {

        d->m_socketDescriptors[d->m_currentSocket] = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

        int socketFlags = 1;

        int result = ioctlsocket(d->m_socketDescriptors[d->m_currentSocket], static_cast<long>(FIONBIO), reinterpret_cast<u_long *>(&socketFlags));

        if (result<0) {
            qDebug() << "Error setting non blocking on socket";
        }

        if (d->m_ttl)
            setsockopt(d->m_socketDescriptors[d->m_currentSocket], IPPROTO_IP, IP_TTL,
                       reinterpret_cast<char *>(&d->m_ttl), sizeof(d->m_ttl));
    }

    SOCKET socketDescriptor;

    socketDescriptor = d->m_socketDescriptors[d->m_currentSocket];

    d->m_currentSocket = d->m_currentSocket % d->m_socketDescriptors.count();

    return socketDescriptor;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::id() -> uint16_t {
    return d->m_id;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::userData() -> void * {
    return d->m_userData;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::setUserData(void *data) -> void {
    d->m_userData = data;
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::Pingnoo::ICMPAPIPingTarget::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}
