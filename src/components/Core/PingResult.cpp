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

#include "PingResult.h"

FizzyAde::Core::PingResult::PingResult()
{
    m_sampleNumber = 0;
    m_code = PingResult::NoReply;
    m_hostAddress = QHostAddress();
    m_roundTripTime = std::chrono::duration<double>(0);
    m_requestTime = std::chrono::system_clock::now();
    m_target = nullptr;
}

FizzyAde::Core::PingResult::~PingResult() = default;

FizzyAde::Core::PingResult::PingResult(unsigned long sampleNumber, PingResultCode code, QHostAddress hostAddress, std::chrono::system_clock::time_point requestTime, std::chrono::duration<double> roundTripTime, FizzyAde::Core::IPingTarget *target)
{
    m_sampleNumber = sampleNumber;
    m_code = code;
    m_hostAddress = std::move(hostAddress);
    m_requestTime = requestTime;
    m_roundTripTime = roundTripTime;
    m_target = target;
}

unsigned long FizzyAde::Core::PingResult::sampleNumber()
{
    return(m_sampleNumber);
}

std::chrono::system_clock::time_point FizzyAde::Core::PingResult::requestTime()
{
    return(m_requestTime);
}

FizzyAde::Core::PingResult::PingResultCode FizzyAde::Core::PingResult::code()
{
    return(m_code);
}

QHostAddress FizzyAde::Core::PingResult::hostAddress()
{
    return(m_hostAddress);
}

std::chrono::duration<double> FizzyAde::Core::PingResult::roundTripTime()
{
    return(m_roundTripTime);
}

FizzyAde::Core::IPingTarget *FizzyAde::Core::PingResult::target()
{
    return(m_target);
}
