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

#ifndef FIZZYADE_ICMPPINGENGINE_ICMPPINGRECEIVER_H
#define FIZZYADE_ICMPPINGENGINE_ICMPPINGRECEIVER_H

#include "Core/PingResult.h"
#include <chrono>
#include <QObject>

namespace FizzyAde::ICMPPingEngine
{
    class ICMPPingEngine;

    /**
     * ICMP packet receiver thread implementation
     *
     * Created and used by the ICMP engine, the receiver thread
     * receives incoming ICMP packet and associates them with
     * requests that were sent and then signals that a ICMP result
     * is available
     *
     */

    class ICMPPingReceiver : public QObject
    {
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
        ICMPPingReceiver(FizzyAde::ICMPPingEngine::ICMPPingEngine *engine);

    private slots:

        /**
         * The receiver thread worker
         */
        void doWork(void);

    signals:

        /**
         * Signals when a response result is available
         *
         * @param[in] result the result
         */
        void result(FizzyAde::Core::PingResult result);

        friend class ICMPPingEngine;

    private:
        std::chrono::milliseconds m_timeout={};         //! The default reception timeout value
        FizzyAde::ICMPPingEngine::ICMPPingEngine *m_engine;    //! The engine that owns this receiver worker

    protected:
        bool m_isRunning;                               //! Thread is running
    };
}

#endif // FIZZYADE_ICMPPINGENGINE_ICMPPINGRECEIVER_H
