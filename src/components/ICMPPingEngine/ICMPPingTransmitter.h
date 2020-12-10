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

#ifndef NEDRYSOFT_ICMPPINGENGINE_ICMPPINGTRANSMITTER_H
#define NEDRYSOFT_ICMPPINGENGINE_ICMPPINGTRANSMITTER_H

#include "Core/PingResult.h"

#include <QMutex>
#include <QObject>
#include <chrono>

namespace Nedrysoft::ICMPPingEngine {
    class ICMPPingEngine;
    class ICMPPingTarget;
    class ICMPPingItem;

    /**
     * @brief       ICMP packet transmitter thread implementation
     *
     * @details     Created and used by the ICMP engine, the transmitter thread
     *              creates requests for the associated targets and sends them
     *              at the given period
     */
    class ICMPPingTransmitter :
            public QObject {

        private:
            Q_OBJECT

        public:

            /**
             * @brief       Constructor
             *
             * @details     Creates the receiver object and passes in the engine
             *              so that the requests can be tagged to the correct engine
             *
             * @param[in]   engine  the owner engine
             */
            ICMPPingTransmitter(Nedrysoft::ICMPPingEngine::ICMPPingEngine *engine);

            /**
             * @brief       Sets the interval between a set of pings
             *
             * @param[in]   interval the interval
             *
             * @return       true if interval successfully set; otherwise false.
             */
            bool setInterval(std::chrono::milliseconds interval);

            /**
             * @brief       Adds a ping target to the transmitter
             *
             * @param[in]   target the target to ping
             *
             */
            void addTarget(Nedrysoft::ICMPPingEngine::ICMPPingTarget *target);

        private:

            /**
             * @brief       The receiver thread worker
             */
            Q_SLOT void doWork();

        public:
            /**
             * @brief       Signals when a transmission result is available
             *
             * @param[in]   result the result
             */
            Q_SIGNAL void result(Nedrysoft::Core::PingResult result);

            friend class ICMPPingEngine;

        private:
            std::chrono::milliseconds m_interval = {};                        //! The transmission period in milliseconds
            Nedrysoft::ICMPPingEngine::ICMPPingEngine *m_engine;              //! The engine that owns this transmitter worker

            QList<Nedrysoft::ICMPPingEngine::ICMPPingTarget *> m_targets;     //! List of ping targets
            QMutex m_targetsMutex;                                            //! Mutex to protect the ping target list

            std::chrono::high_resolution_clock::time_point m_epoch;           //! Transmission epoch

        protected:
            bool m_isRunning;                                                 //! Thread is running
    };
}

#endif // NEDRYSOFT_ICMPPINGENGINE_ICMPPINGTRANSMITTER_H
