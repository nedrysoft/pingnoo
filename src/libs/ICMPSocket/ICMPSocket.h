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

#ifndef NEDRYSOFT_ICMPSOCKET_ICMPSOCKET_H
#define NEDRYSOFT_ICMPSOCKET_ICMPSOCKET_H

#include <QtGlobal>
#include <chrono>

#if defined(Q_OS_UNIX)
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#elif defined(Q_OS_WIN)
#define NOMINMAX
#include <WS2tcpip.h>
#include <WinSock2.h>
#endif

#include <QByteArray>
#include <QHostAddress>

#if ( defined(NEDRYSOFT_LIBRARY_ICMPSOCKET_EXPORT))
#define NEDRYSOFT_ICMPSOCKET_DLLSPEC Q_DECL_EXPORT
#else
#define NEDRYSOFT_ICMPSOCKET_DLLSPEC Q_DECL_IMPORT
#endif

namespace Nedrysoft::ICMPSocket {
    enum IPVersion {
        V4 = 4,
        V6 = 6
    };

    /**
     * @brief           The ICMPSocket class abstracts the platform specific code for ICMP sockets.
     */
    class NEDRYSOFT_ICMPSOCKET_DLLSPEC ICMPSocket {
        private:
#if defined(Q_OS_WIN)
            typedef SOCKET socket_t;
#else
            typedef int socket_t;
#endif
        private:
            /**
             * @brief       Constructs a new ICMPSocket.
             *
             * @details     The ICMPSocket constructor is private, instances are created by calling either
             *              createReadSocket or createWriteSocket
             *
             * @param[in]   socket platform socket handle.
             * @param[in]   version version of IP of the socket to open.
             */
            ICMPSocket(ICMPSocket::socket_t socket, IPVersion version = Nedrysoft::ICMPSocket::V4);

            /**
             * @brief       Checks whether the given platform socket is valid.
             *
             * @param[in]   socket platform socket handle.
             *
             * @returns     true if socket is valid; otherwise false..
             */
            static auto isValid(ICMPSocket::socket_t socket) -> bool;

            /**
             * @brief       Called by socket creation to ensure underlying networking subsystem is initialised.
             */
            static auto initialiseSockets() -> void;

        public:
            /**
             * @brief       Destroys the ICMPSocket.
             */
            ~ICMPSocket();

            /**
             * @brief       Creates a socket for reading ALL incoming ICMP packets.
             *
             * @param[in]   version the ip version of the socket to create.
             *
             * @returns      an instance of this class.
             */
            static auto createReadSocket(Nedrysoft::ICMPSocket::IPVersion version = Nedrysoft::ICMPSocket::V4) -> ICMPSocket *;

            /**
             * @brief       Creates a socket for writing ICMP packets with the given ttl.
             *
             * @param[in]   ttl the ttl for the socket.
             *
             * @returns     the write socket instance.
             */
            static auto createWriteSocket(
                    int ttl = 0,
                    Nedrysoft::ICMPSocket::IPVersion version = Nedrysoft::ICMPSocket::V4 ) -> ICMPSocket *;

            /**
             * @brief       Receives data from a read or write socket.
             *
             * @param[in]   buffer the buffer to receive data.
             * @param[out]  receiveAddress the address that the packet was received from.
             * @param[in]   timeout read timeout.
             *
             * @returns     -1 on timeout or error; otherwise the number of bytes read.
             */
            auto recvfrom(QByteArray &buffer, QHostAddress &receiveAddress, std::chrono::milliseconds timeout) -> int;

            /**
             * @brief       Sends data to a write socket.
             *
             * @param[in]   buffer the data to send.
             * @param[in]   hostAddress the address that the packet was received from.
             *
             * @returns     -1 on error; otherwise the number of bytes written.
             */
            auto sendto(QByteArray &buffer, const QHostAddress &hostAddress) -> int;

            /**
             * @brief       Sets the TTL on a write socket.
             *
             * @param[in]   ttl the ttl for the socket.
             */
            auto setTTL(int ttl) -> void;

            /**
             * @brief       Returns the TTL for this socket.
             *
             * @returns     The TTL.
             */
            auto ttl() -> int;

            /**
             * @brief       Sets the Hop Limit on a write socket.
             *
             * @param[in]   hopLimit the hop limit to set.
             */
            auto setHopLimit(int hopLimit) -> void;

            /**
             * @brief       Returns the IP version of the socket.
             *
             * @returns     V4 or V6.
             */
            auto version() -> Nedrysoft::ICMPSocket::IPVersion;

        private:

            ICMPSocket::socket_t m_socketDescriptor;
            Nedrysoft::ICMPSocket::IPVersion m_version;
            int m_ttl;
    };
}

#endif // NEDRYSOFT_ICMPSOCKET_ICMPSOCKET_H
