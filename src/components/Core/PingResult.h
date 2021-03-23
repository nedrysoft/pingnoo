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

#ifndef NEDRYSOFT_CORE_PINGRESULT_H
#define NEDRYSOFT_CORE_PINGRESULT_H

#include "CoreSpec.h"

#include <QHostAddress>
#include <QObject>
#include <cmath>
#include <cstdint>

namespace Nedrysoft::Core {
    class IPingTarget;

    /**
     * @brief       The PingResult class provides information about a ping response.
     */
    class NEDRYSOFT_CORE_DLLSPEC PingResult {

        public:

            /**
             * @brief       The result codes for a ping.
             */
            enum class ResultCode {
                Ok,
                NoReply,
                TimeExceeded
            };

            /**
             * @brief       Constructs a PingResult instance.
             */
            PingResult();

            /**
             * @brief       Destroys the PingResult.
             */
            ~PingResult();

            /**
             * @brief       Constructs a PingResult with parameters.
             *
             * @param[in]   sampleNumber the count which this result is associated with.
             * @param[in]   code the result code.
             * @param[in]   hostAddress the IP address that responded to the request.
             * @param[in]   requestTime the time the request was sent.
             * @param[in]   roundTripTime the time taken for the hop to respond.
             * @param[in]   target the target that was pinged.
             */
            PingResult(unsigned long sampleNumber,
                       ResultCode code,
                       const QHostAddress &hostAddress,
                       std::chrono::system_clock::time_point requestTime,
                       std::chrono::duration<double> roundTripTime,
                       Nedrysoft::Core::IPingTarget *target);

        public:

            /**
             * @brief       Returns the sample number of the request.
             *
             * @returns     the sample number.
             */
            auto sampleNumber() -> unsigned long;

            /**
             * @brief       Returns the time that the request was transmitted at.
             *
             * @returns     the request time.
             */
            auto requestTime() -> std::chrono::system_clock::time_point;

            /**
             * @brief       The result code for the request (Echo Reply, Timeout).
             *
             * @returns     the result code.
             */
            auto code() -> ResultCode;

            /**
             * @brief       The host address of the reply.
             *
             * @details     This may differ from the target ip address, if the TTL of the request was less than the
             *              number of hops to the target then a time exceeded response will be returned and the IP of
             *              last hop that responded will be returned.
             *
             * @returns     the IP address of the host that sent the reply..
             */
            auto hostAddress() -> QHostAddress;

            /**
             * @brief       The round trip time.
             *
             * @details     The round trip time is the elapsed time from the packet being sent to the response.
             *
             * @returns     the round trip time.
             */
            auto roundTripTime() -> std::chrono::duration<double>;

            /**
             * @brief       The target associated with this result.
             *
             * @returns     the target.
             */
            auto target() -> Nedrysoft::Core::IPingTarget *;

        private:
            unsigned long m_sampleNumber;
            PingResult::ResultCode m_code;
            QHostAddress m_hostAddress;
            std::chrono::duration<double> m_roundTripTime = {};
            std::chrono::system_clock::time_point m_requestTime = {};
            Nedrysoft::Core::IPingTarget *m_target;
    };
}

#endif // NEDRYSOFT_CORE_PINGRESULT_H
