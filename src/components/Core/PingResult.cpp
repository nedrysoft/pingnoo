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

#include "PingResult.h"

Nedrysoft::Core::PingResult::PingResult() :
    m_sampleNumber(0),
    m_code(PingResult::ResultCode::NoReply),
    m_hostAddress(QHostAddress()),
    m_roundTripTime(std::chrono::duration<double>(0)),
    m_requestTime(std::chrono::system_clock::now()),
    m_target(nullptr) {

}

Nedrysoft::Core::PingResult::~PingResult() = default;

Nedrysoft::Core::PingResult::PingResult(
        unsigned long sampleNumber,
        PingResult::ResultCode code,
        const QHostAddress &hostAddress,
        std::chrono::system_clock::time_point requestTime,
        std::chrono::duration<double> roundTripTime,
        Nedrysoft::Core::IPingTarget *target) :

            m_sampleNumber(sampleNumber),
            m_code(code),
            m_hostAddress(hostAddress),
            m_roundTripTime(roundTripTime),
            m_requestTime(requestTime),
            m_target(target) {

}

auto Nedrysoft::Core::PingResult::sampleNumber() -> unsigned long {
    return m_sampleNumber;
}

auto Nedrysoft::Core::PingResult::requestTime() -> std::chrono::system_clock::time_point {
    return m_requestTime;
}

auto Nedrysoft::Core::PingResult::code() -> Nedrysoft::Core::PingResult::ResultCode {
    return m_code;
}

auto Nedrysoft::Core::PingResult::hostAddress() -> QHostAddress {
    return m_hostAddress;
}

auto Nedrysoft::Core::PingResult::roundTripTime() -> std::chrono::duration<double> {
    return m_roundTripTime;
}

auto Nedrysoft::Core::PingResult::target() -> Nedrysoft::Core::IPingTarget * {
    return m_target;
}
