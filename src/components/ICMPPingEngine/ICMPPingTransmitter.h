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

#ifndef PINGNOO_COMPONENTS_ICMPPINGENGINE_ICMPPINGTRANSMITTER_H
#define PINGNOO_COMPONENTS_ICMPPINGENGINE_ICMPPINGTRANSMITTER_H

#include <PingResult>

#include <QMutex>
#include <QObject>
#include <chrono>

namespace Nedrysoft { namespace ICMPPingEngine {
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
            explicit ICMPPingTransmitter(Nedrysoft::ICMPPingEngine::ICMPPingEngine *engine);

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
            Q_SIGNAL void result(Nedrysoft::RouteAnalyser::PingResult result);

            friend class ICMPPingEngine;

        private:
            //! @cond

            std::chrono::milliseconds m_interval = {};
            Nedrysoft::ICMPPingEngine::ICMPPingEngine *m_engine;

            QList<Nedrysoft::ICMPPingEngine::ICMPPingTarget *> m_targets;
            QMutex m_targetsMutex;

            std::chrono::high_resolution_clock::time_point m_epoch;

            static QMutex m_sequenceMutex;
            static uint16_t m_sequenceId;

        protected:
            bool m_isRunning;

            //! @endcond
    };
}}

#endif // PINGNOO_COMPONENTS_ICMPPINGENGINE_ICMPPINGTRANSMITTER_H
