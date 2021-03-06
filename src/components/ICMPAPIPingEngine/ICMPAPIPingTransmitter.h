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

#ifndef PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGTRANSMITTER_H
#define PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGTRANSMITTER_H

#include <PingResult>

#include <QMutex>
#include <QObject>

namespace Nedrysoft { namespace ICMPAPIPingEngine {
    class ICMPAPIPingEngine;
    class ICMPAPIPingTarget;

    /**
     * @brief       The ICMPAPIPingTransmitter class provides a thread for transmitting ICMP packets.
     *
     * @details     and used by the ICMPAPI engine, the transmitter thread creates requests for the associated
     *              targets and sends them at the given period
     */
    class ICMPAPIPingTransmitter :
            public QObject {

        private:
            Q_OBJECT

        public:

            /**
             * @brief       Constructs a new ping transmitter with the given engine.
             *
             * @param[in]   engine the owner engine.
             */
            explicit ICMPAPIPingTransmitter(Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *engine);

            /**
             * "brief       Destroys the ICMPAPIPingTransmitter.
             */
            ~ICMPAPIPingTransmitter();

            /**
             * @brief       Sets the interval between a set of pings.
             *
             * @param[in]   interval the interval between pings in milliseconds.
             */
            auto setInterval(int interval) -> bool;

            /**
             * @brief       The transmitter thread worker.
             */
            Q_SLOT void doWork(void);

            /**
             * @brief       Signslled when a result is available.
             *
             * @param[in]   result the ping result.
             */
            Q_SIGNAL void result(Nedrysoft::RouteAnalyser::PingResult result);

            /**
             * @brief       Adds a target to be pinged.
             *
             * @param[in]   target the target to be pinged
             */
            void addTarget(Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *target);

            friend class ICMPAPIPingEngine;

        private:
            //! @cond

            int m_interval;
            Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *m_engine;

            QList<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *> m_targets;
            QMutex m_targetsMutex;
            bool m_isRunning;

            //! @endcond
    };
}}

#endif // PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGTRANSMITTER_H
