/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 10/12/2020.
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

#ifndef NEDRYSOFT_ICMPPINGRECEIVERWORKER_H
#define NEDRYSOFT_ICMPPINGRECEIVERWORKER_H

#include <QObject>
#include <chrono>
#include <QByteArray>
#include <QHostAddress>
#include <QThread>

namespace Nedrysoft::ICMPSocket {
    class ICMPSocket;
}

namespace Nedrysoft::ICMPPingEngine {
    class ICMPPingEngine;
    class ICMPPingReceiverWorker;

    /**
     * @brief       The ICMP packet receiver class.
     *
     * @details     This is a singleton class, there is a single receive thread which reads packets as they arrive
     *              and then signals that a packet is available, other objects can then process the packet.
     */
    class ICMPPingReceiverWorker :
            public QObject {

        private:
            Q_OBJECT

        private:
            /**
             * @brief       Constructs a ICMPPingReceiverWorker.
             *
             * @note        Hidden as this is a singleton class and should be accessed through getInstance().
             */
            ICMPPingReceiverWorker();

            /**
             * @brief       Destroys the ICMPPingReceiverWorker.
             */
            ~ICMPPingReceiverWorker();

        public:
            /**
             * @brief       Returns the ICMPPingReceiverWorker singleton instance.
             *
             * @param[in]   returnNull if the singleton has not been allocated, then return null if true.
             *
             * @returns     the singleton instance.
             */
            static auto getInstance(bool returnNull=false) -> Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker *;

            /**
             * @brief       This signal is emitted when an ICMP packet has been received.
             *
             * @param[in]   receiveTime the time at which the reception occurred.
             * @param[in]   receiveBuffer the packet data.
             * @param[in]   receiveAddress the address the packet was received from (this may differ from the target).
             */
            Q_SIGNAL void packetReceived(
                    std::chrono::time_point < std::chrono::high_resolution_clock > receiveTime,
                    QByteArray receiveBuffer,
                    QHostAddress receiveAddress);

            friend class ICMPPingEngine;
            friend class ICMPPingEngineFactory;

        private:
            /**
             * @brief       The worker thread.
             */
            auto doWork() -> void;

        private:
            Nedrysoft::ICMPPingEngine::ICMPPingEngine *m_engine;
            Nedrysoft::ICMPPingEngine::ICMPPingReceiverWorker *m_receiveWorker;
            QThread *m_receiverThread;
            Nedrysoft::ICMPSocket::ICMPSocket *m_socket;

            bool m_isRunning;
    };
}

Q_DECLARE_METATYPE(std::chrono::time_point<std::chrono::high_resolution_clock>)

#endif //NEDRYSOFT_ICMPPINGRECEIVERWORKER_H
