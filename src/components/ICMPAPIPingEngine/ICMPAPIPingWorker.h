/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 18/04/2021.
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

#ifndef PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGWORKER_H
#define PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGWORKER_H

#include <QObject>

#include <IPingEngine>

namespace Nedrysoft { namespace ICMPAPIPingEngine {
    class ICMPAPIPingTarget;
    class ICMPAPIPingEngine;

    /**
     * @brief       The ICMPAPIPingWorker class uses the synchronous ICMPAPI ping function to send
     *              pings, each ping runs in its own thread.
     */
    class ICMPAPIPingWorker : public QObject {
        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs an ICMPAPIPingWorker.
             *
             * @param[in]   engine the ping engine that owns the worker.
             * @param[in]   sampleNumber the sample number of this ping.
             * @param[in]   target the target to be pinged.
             */
            ICMPAPIPingWorker(
                    Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *engine,
                    int sampleNumber,
                    Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *target);

            /**
             * @brief       The thread worker function.
             */
            Q_SLOT void doWork();

            /**
             * @brief       Emits the result of the ping.
             *
             * @param[in]   pingResult a PingResult object containing the result.
             */
            Q_SIGNAL void result(Nedrysoft::RouteAnalyser::PingResult pingResult);

        private:
            //! @cond

            Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *m_target;
            int m_sampleNumber;
            Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngine *m_engine;

            //! @endcond

    };
}}

#endif //PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGWORKER_H
