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

#include "ICMPSocket.h"

#if defined(Q_OS_UNIX)
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#elif defined(Q_OS_WIN)
#include <WS2tcpip.h>
#include <WinSock2.h>
#endif

#include <QtEndian>

#if defined(Q_OS_WIN)
constexpr int SocketError = SOCKET_ERROR;
#else
constexpr int SocketError = -1;
#endif

constexpr auto ReceiveBufferSize = 4096;

Nedrysoft::ICMPSocket::ICMPSocket::ICMPSocket(Nedrysoft::ICMPSocket::ICMPSocket::socket_t socket, IPVersion version) :
        m_socketDescriptor(socket),
        m_version(version),
        m_ttl(64) {

}

Nedrysoft::ICMPSocket::ICMPSocket::~ICMPSocket() {
#if defined(Q_OS_WIN)
    closesocket(m_socketDescriptor);
#else
    close(m_socketDescriptor);
#endif
}

auto Nedrysoft::ICMPSocket::ICMPSocket::createReadSocket(
        Nedrysoft::ICMPSocket::IPVersion version ) -> Nedrysoft::ICMPSocket::ICMPSocket * {

    Nedrysoft::ICMPSocket::ICMPSocket::socket_t socketDescriptor;

    initialiseSockets();

#if defined(Q_OS_MACOS)
    if (version == Nedrysoft::ICMPSocket::V4) {
        socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    } else if (version == Nedrysoft::ICMPSocket::V6) {
        socketDescriptor = socket(AF_INET6, SOCK_DGRAM, IPPROTO_ICMPV6);
    } else {
        qWarning() << QObject::tr("Unknown IP version");

        return nullptr;
    }

    if (isValid(socketDescriptor)) {
        auto result = fcntl(socketDescriptor, F_SETFL, fcntl(socketDescriptor, F_GETFL, 0) |
                                                       O_NONBLOCK); // NOLINT(cppcoreguidelines-pro-type-vararg)

        if (result < 0) {
            qWarning() << QObject::tr("Error setting non blocking on socket");
        }
    }
#elif defined(Q_OS_UNIX)
    if (version==Nedrysoft::ICMPSocket::V4) {
        socketDescriptor = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMP);
    } else if (version==Nedrysoft::ICMPSocket::V6) {
        socketDescriptor = socket(AF_INET6, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMPV6);
    } else {
        qWarning() << QObject::tr("Unknown IP version");

        return nullptr;
    }
#elif defined(Q_OS_WIN)
    if (version==Nedrysoft::ICMPSocket::V4) {
        socketDescriptor = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    } else if (version==Nedrysoft::ICMPSocket::V6) {
        socketDescriptor = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
    } else {
        qWarning() << QObject::tr("Unknown IP version");

        return nullptr;
    }

    if (socketDescriptor!=INVALID_SOCKET) {
        int socketFlags = 1;
        struct sockaddr_storage source = {};

        auto result = ioctlsocket(socketDescriptor, static_cast<long>(FIONBIO), reinterpret_cast<ulong *>(&socketFlags));

        if (result==SOCKET_ERROR) {
            qWarning() << QObject::tr("Error setting non blocking.");
        }

        memset(&source, 0, sizeof(source));

        if (version==Nedrysoft::ICMPSocket::V4) {
            sockaddr_in *src = reinterpret_cast<sockaddr_in *>(&source);

            src->sin_family = AF_INET;
            src->sin_port = 0;
            src->sin_addr.S_un.S_addr = INADDR_ANY;
        } else {
            sockaddr_in6 *src = reinterpret_cast<sockaddr_in6 *>(&source);

            src->sin6_family = AF_INET6;
            src->sin6_port = 0;
            src->sin6_flowinfo = 0;
            src->sin6_addr = in6addr_any;
        }

        result = bind(socketDescriptor, reinterpret_cast<sockaddr*>(&source), sizeof(source));

        if (result==SOCKET_ERROR) {
            qWarning() << QObject::tr("Error binding socket.");
        }
    }
#endif

#if defined(Q_OS_UNIX)
    if (socketDescriptor == -1) {
        qWarning() << QObject::tr("Error creating socket descriptor.");

        return nullptr;
    }
#endif

    return new Nedrysoft::ICMPSocket::ICMPSocket(socketDescriptor, version);
}

auto Nedrysoft::ICMPSocket::ICMPSocket::createWriteSocket(
        int ttl,
        Nedrysoft::ICMPSocket::IPVersion version ) -> Nedrysoft::ICMPSocket::ICMPSocket * {

    Nedrysoft::ICMPSocket::ICMPSocket::socket_t socketDescriptor;

    initialiseSockets();

#if defined(Q_OS_MACOS)
    if (version == Nedrysoft::ICMPSocket::V4) {
        socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    } else if (version == Nedrysoft::ICMPSocket::V6) {
        socketDescriptor = socket(AF_INET6, SOCK_DGRAM, IPPROTO_ICMPV6);
    } else {
        qWarning() << QObject::tr("Unknown IP version");

        return nullptr;
    }

    if (isValid(socketDescriptor)) {
        int result = fcntl(socketDescriptor, F_SETFL, fcntl(socketDescriptor, F_GETFL, 0) |
                                                      O_NONBLOCK); // NOLINT(cppcoreguidelines-pro-type-vararg)

        if (result < 0) {
            qWarning() << QObject::tr("Error setting non blocking on socket");
        }
    }
#elif defined(Q_OS_UNIX)
    if (version==Nedrysoft::ICMPSocket::V4) {
        socketDescriptor = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMP);
    } else if (version==Nedrysoft::ICMPSocket::V6) {
        socketDescriptor = socket(AF_INET6, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMPV6);
    } else {
        qWarning() << QObject::tr("Unknown IP version");

        return nullptr;
    }
#elif defined(Q_OS_WIN)
    if (version==Nedrysoft::ICMPSocket::V4) {
        socketDescriptor = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    } else if (version==Nedrysoft::ICMPSocket::V6) {
        socketDescriptor = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
    } else {
        qWarning() << QObject::tr("Unknown IP version");

        return nullptr;
    }

    if (isValid(socketDescriptor)) {
        int socketFlags = 1;

        auto result = ioctlsocket(socketDescriptor, static_cast<long>(FIONBIO), reinterpret_cast<u_long *>(&socketFlags));

        if (result==SocketError) {
            qWarning() << QObject::tr("Error setting non blocking on socket");
        }
    }
#endif
    Nedrysoft::ICMPSocket::ICMPSocket *socketInstance = nullptr;

    if (isValid(socketDescriptor)) {
        socketInstance = new Nedrysoft::ICMPSocket::ICMPSocket(socketDescriptor, version);

        if (ttl) {
            if (version == V4) {
                socketInstance->setTTL(ttl);
            } else  {
                socketInstance->setHopLimit(ttl);
            }
        }
    } else {
        qWarning() << QObject::tr("Error creating socket descriptor.");
    }

    return socketInstance;
}

auto Nedrysoft::ICMPSocket::ICMPSocket::recvfrom(
        QByteArray &buffer,
        QHostAddress &receiveAddress,
        std::chrono::milliseconds timeout) -> int {

#if defined(Q_OS_UNIX)
    socklen_t addressLength;
    unsigned int socketErrorLength;
#elif defined(Q_OS_WIN)
    int addressLength;
    int socketErrorLength;

    int (WSAAPI *poll)(struct pollfd *, ulong , int ) = WSAPoll;
#endif
    int socketError;
    struct sockaddr_storage fromAddress = {};
    struct pollfd descriptorSet = {};

    descriptorSet.fd = m_socketDescriptor;
    descriptorSet.events = POLLIN;

    auto numberOfReadyDescriptors = poll(&descriptorSet, 1, static_cast<int>(timeout.count()));

    if (numberOfReadyDescriptors > 0) {
        if (descriptorSet.events & POLLIN) {
            socketErrorLength = sizeof(socketError);

            getsockopt(
                    m_socketDescriptor,
                    SOL_SOCKET, SO_ERROR,
                    reinterpret_cast<char *>(&socketError),
                    &socketErrorLength );

            memset(&fromAddress, 0, sizeof(fromAddress));

            addressLength = sizeof(fromAddress);

            buffer.resize(ReceiveBufferSize);

            auto result = ::recvfrom(
                                    m_socketDescriptor,
                                    buffer.data(),
                                    buffer.length(),
                                    0,
                                    reinterpret_cast<sockaddr *>(&fromAddress),
                                    &addressLength );

            if (result >= 0) {
                receiveAddress = QHostAddress(reinterpret_cast<sockaddr *>(&fromAddress));

                buffer.resize(result);
            }

            return result;
        }
    }

    return -1;
}

auto Nedrysoft::ICMPSocket::ICMPSocket::sendto(QByteArray &buffer, const QHostAddress &hostAddress) -> int {
    if (m_version == V4) {
        struct sockaddr_in toAddress = {};

        memset(&toAddress, 0, sizeof(toAddress));

        toAddress.sin_family = AF_INET;
        toAddress.sin_addr.s_addr = qToBigEndian<uint32_t>(hostAddress.toIPv4Address());

        return ::sendto(m_socketDescriptor, buffer.data(), buffer.length(), 0,
                        reinterpret_cast<struct sockaddr *>(&toAddress), sizeof(toAddress));
    } else if (m_version == V6) {
        struct sockaddr_in6 toAddress = {};

        auto destinationAddress = hostAddress.toIPv6Address();

        toAddress.sin6_family = AF_INET6;
        memcpy(toAddress.sin6_addr.s6_addr, &destinationAddress, 16);

        return ::sendto(m_socketDescriptor, buffer.data(), buffer.length(), 0,
                        reinterpret_cast<struct sockaddr *>(&toAddress), sizeof(toAddress));
    }

    return -1;
}

auto Nedrysoft::ICMPSocket::ICMPSocket::isValid(Nedrysoft::ICMPSocket::ICMPSocket::socket_t socket) -> bool {
#if defined(Q_OS_WIN)
    return socket!=INVALID_SOCKET;
#else
    return socket != -1;
#endif
}

auto Nedrysoft::ICMPSocket::ICMPSocket::initialiseSockets() -> void {
    static auto initialised = false;

    if (!initialised) {
#if defined(Q_OS_WIN)
        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2,2), &wsaData)!=0) {
            qDebug() << "error initialising winsock";
        }
#endif
        initialised = true;
    }
}

auto Nedrysoft::ICMPSocket::ICMPSocket::setTTL(int ttl) -> void {
    auto result = setsockopt(m_socketDescriptor, IPPROTO_IP, IP_TTL, reinterpret_cast<char *>(&ttl), sizeof(ttl));

    m_ttl = ttl;

    if (result == SocketError) {
        qWarning() << QObject::tr("Error setting TTL.");
    }
}

auto Nedrysoft::ICMPSocket::ICMPSocket::setHopLimit(int hopLimit) -> void {
    auto result = setsockopt(m_socketDescriptor, IPPROTO_IPV6, IPV6_UNICAST_HOPS, reinterpret_cast<char *>(&hopLimit),
                             sizeof(hopLimit));

    if (result == SocketError) {
        qWarning() << QObject::tr("Error setting Hop Limit.");
    }
}

auto  Nedrysoft::ICMPSocket::ICMPSocket::version() -> Nedrysoft::ICMPSocket::IPVersion {
    return m_version;
}

auto Nedrysoft::ICMPSocket::ICMPSocket::ttl() -> int {
    return m_ttl;
}