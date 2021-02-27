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
     * @brief       The ICMPPingTransmitter class sends pings to the target (and intermediate nodes) at a prescribed
     *              interval.
     */
    class ICMPPingTransmitter :
            public QObject {

        private:
            Q_OBJECT

        public:

            /**
             * @brief       Constructs a new ICMPPingTransmitter for the given engine.
             *
             * @param[in]   engine the owner engine.
             */
            ICMPPingTransmitter(Nedrysoft::ICMPPingEngine::ICMPPingEngine *engine);

            /**
             * @brief       Destroys the ICMPPingTransmitter.
             */
            ~ICMPPingTransmitter();

            /**
             * @brief       Sets the interval between a set of pings.
             *
             * @param[in]   interval the interval.
             */
            auto setInterval(std::chrono::milliseconds interval) -> bool;

            /**
             * @brief       Returns the ping interval.
             *
             * @returns     the interval.
             */
            auto interval() -> std::chrono::milliseconds;

            /**
             * @brief       Adds a ping target to the transmitter.
             *
             * @param[in]   target the target to ping.
             *
             */
            auto addTarget(Nedrysoft::ICMPPingEngine::ICMPPingTarget *target) -> void;

        private:

            /**
             * @brief       The transmitter thread worker.
             */
            Q_SLOT void doWork();

        public:
            /**
             * @brief       This signal is emitted when a transmission result is available.
             *
             * @param[in]   result the result.
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
