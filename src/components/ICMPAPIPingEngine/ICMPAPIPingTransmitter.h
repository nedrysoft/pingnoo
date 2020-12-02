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

#ifndef NEDRYSOFT_PINGNOO_ICMPAPIPINGTRANSMITTER_H
#define NEDRYSOFT_PINGNOO_ICMPAPIPINGTRANSMITTER_H

#include "ICMPAPIPingEngineSpec.h"
#include "Core/PingResult.h"
#include <QObject>
#include <QMutex>

namespace Nedrysoft::Pingnoo {
    class ICMPAPIPingEngine;

    /**
     * ICMPAPI packet transmitter thread implementation
     *
     * Created and used by the ICMPAPI engine, the transmitter thread
     * creates requests for the associated targets and sends them
     * at the given period
     *
     */

    class ICMPAPIPingTransmitter :
            public QObject {
        Q_OBJECT

        public:

            /**
             * Constructor with engine
             *
             * Creates the receiver object and passes in the engine
             * so that the requests can be tagged to the correct engine
             *
             * @param[in] engine the owner engine
             */
            ICMPAPIPingTransmitter(Nedrysoft::Pingnoo::ICMPAPIPingEngine *engine);

            /**
             * Sets the interval between a set of pings
             *
             * @param[in] interval interval
             */
            bool setInterval(std::chrono::milliseconds interval);

            /**
             * Adds a ping target to the transmitter
             *
             * @param[in] target the target to ping
             */
            //void addTarget(FZICMPPingTarget *target);

            friend class ICMPAPIPingEngine;

        private slots:

            /**
             * The receiver thread worker
             */
            void doWork(void);

        signals:

            /**
             * Signals when a transmission result is available
             *
             * @param[in] result the result
             */
            void result(Nedrysoft::Core::PingResult result);

        private:
            std::chrono::milliseconds m_interval = {};      //! The transmission period in milliseconds
            Nedrysoft::Pingnoo::ICMPAPIPingEngine *m_engine; //! The engine that owns this transmitter worker

            //QList<FZICMPPingTarget *> m_targets;            //! List of ping targets
            QMutex m_targetsMutex;                          //! Mutex to protect the ping target list
            bool m_isRunning;                               //! Whether thread is running
    };
}

#endif // NEDRYSOFT_PINGNOO_ICMPAPIPINGTRANSMITTER_H
