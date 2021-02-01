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

#ifndef NEDRYSOFT_ICMPPINGENGINE_ICMPPINGTIMEOUT_H
#define NEDRYSOFT_ICMPPINGENGINE_ICMPPINGTIMEOUT_H

#include "Core/PingResult.h"

namespace Nedrysoft::ICMPPingEngine {
    class ICMPPingEngine;

    /**
     * @brief       The ICMPPingTimeout class monitors packets and signals if a timeout occurred.
     */
    class ICMPPingTimeout :
            public QObject {

        private:
            Q_OBJECT

        public:

            /**
             * @brief       Constructs a new ICMPPingTimeout with the given ping engine.
             *
             * @details     Creates the receiver object and passes in the engine so that the requests can be tagged
             *              to the correct engine.
             *
             * @param[in]   engine the owner engine.
             */
            ICMPPingTimeout(Nedrysoft::ICMPPingEngine::ICMPPingEngine *engine);

            /**
             * @brief       The timeout thread worker.
             */
            Q_SLOT void doWork();

            /**
             * @brief       This signal is emitted when a timeout has been detected.
             *
             * @param[in]   result the timeout result.
             */
            Q_SIGNAL void result(Nedrysoft::Core::PingResult result);

            friend class ICMPPingEngine;

        private:
            Nedrysoft::ICMPPingEngine::ICMPPingEngine *m_engine;    //! The engine that owns this timeout worker

        protected:
            bool m_isRunning;                               //! Thread is running
    };
}

#endif // NEDRYSOFT_ICMPPINGENGINE_ICMPPINGTIMEOUT_H
