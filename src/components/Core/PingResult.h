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

#ifndef FIZZYADE_CORE_PINGRESULT_H
#define FIZZYADE_CORE_PINGRESULT_H

#include "CoreSpec.h"
#include <cmath>
#include <cstdint>
#include <QObject>
#include <QHostAddress>

namespace FizzyAde::Core
{
    class IPingTarget;

    /**
     * @brief       Ping result
     *
     * @details     Holds the result of a ping request/response/timeout, this is
     *              passed as a parameter by the ping engine.
     */
    class FIZZYADE_CORE_DLLSPEC PingResult
    {

    public:

        /**
         * Result codes for a ping
         */
        enum PingResultCode {
            Ok,
            NoReply,
            TimeExceeded
        };

        /**
          * @brief       Constructor
          *
          */
        PingResult();

        /**
          * @brief       Destructor
          *
          */
        ~PingResult();

        /**
         * @brief       Constructs a FZPingResult with parameters
         *
         * @param[in]   sampleNumber        the count which this result is associated with
         * @param[in]   code                the result code
         * @param[in]   hostAddress         the IP address that responded to the request
         * @param[in]   requestTime         the time the request was sent
         * @param[in]   roundTripTime       the time taken for the hop to respond
         * @param[in]   target              the target that was pinged
         *
         */
        PingResult(unsigned long sampleNumber, PingResultCode code, QHostAddress hostAddress, std::chrono::system_clock::time_point requestTime, std::chrono::duration<double> roundTripTime, FizzyAde::Core::IPingTarget *target);

    public:

        /**
         * @brief       The sample number of the request
         *
         * @return      the sample number
         *
         */
        unsigned long sampleNumber();

        /**
         * @brief       The time of the response
         *
         * @return      the reply time
         *
         */
        std::chrono::system_clock::time_point requestTime();

        /**
         * @brief       The result code
         *
         * @return      the result number
         *
         */
        PingResultCode code();

        /**
         * @brief       The host address of the reply
         *
         * @details     This may differ from the target ip address, if the TTL of the
         *              request was less than the number of hops to the target then
         *              a time exceeded response will be returned and the IP of last hop
         *              that responded will be returned
         *
         * @return      the IP address of the reply
         *
         */
        QHostAddress hostAddress();

        /**
         * @brief       The round trip time
         *
         * @details     The round trip time from the packet being sent to the response
         *
         * @return      the round trip time
         *
         */
        std::chrono::duration<double> roundTripTime();

        /**
         * @brief       The target assosciated with this result
         *
         * @return      the target
         *
         */
        FizzyAde::Core::IPingTarget *target();

    private:
        unsigned long m_sampleNumber;
        PingResult::PingResultCode m_code;
        QHostAddress m_hostAddress;
        std::chrono::duration<double> m_roundTripTime = {};
        FizzyAde::Core::IPingTarget *m_target;
        std::chrono::system_clock::time_point m_requestTime = {};
    };
}

#endif // FIZZYADE_CORE_PINGRESULT_H
