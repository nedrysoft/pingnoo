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

#include "ICMPPingItem.h"
#include <QTimer>
#include <QDebug>
#include <QMutex>

FizzyAde::ICMPPingEngine::ICMPPingItem::ICMPPingItem()
{
    m_id = 0;
    m_sequenceId = 0;
    m_serviced = false;
    m_target = nullptr;
    m_sampleNumber = 0;
}

FizzyAde::ICMPPingEngine::ICMPPingItem::~ICMPPingItem() = default;

void FizzyAde::ICMPPingEngine::ICMPPingItem::setId(uint16_t id)
{
    m_id = id;
}

void FizzyAde::ICMPPingEngine::ICMPPingItem::setSequenceId(uint16_t sequence)
{
    m_sequenceId = sequence;
}

uint16_t FizzyAde::ICMPPingEngine::ICMPPingItem::sequenceId()
{
    return(m_sequenceId);
}

void FizzyAde::ICMPPingEngine::ICMPPingItem::setTransmitTime(std::chrono::high_resolution_clock::time_point time, std::chrono::system_clock::time_point epoch)
{
    m_transmitTime = time;
    m_transmitEpoch = epoch;
}

void FizzyAde::ICMPPingEngine::ICMPPingItem::setServiced(bool serviced)
{
    m_serviced = serviced;
}

bool FizzyAde::ICMPPingEngine::ICMPPingItem::serviced()
{
    return(m_serviced);
}

uint16_t FizzyAde::ICMPPingEngine::ICMPPingItem::id()
{
    return(m_id);
}

void FizzyAde::ICMPPingEngine::ICMPPingItem::setTarget(FizzyAde::ICMPPingEngine::ICMPPingTarget *target)
{
    m_target = target;
}

FizzyAde::ICMPPingEngine::ICMPPingTarget *FizzyAde::ICMPPingEngine::ICMPPingItem::target()
{
    return(m_target);
}

std::chrono::high_resolution_clock::time_point FizzyAde::ICMPPingEngine::ICMPPingItem::transmitTime()
{
    return(m_transmitTime);
}

std::chrono::system_clock::time_point FizzyAde::ICMPPingEngine::ICMPPingItem::transmitEpoch()
{
    return(m_transmitEpoch);
}

void FizzyAde::ICMPPingEngine::ICMPPingItem::setSampleNumber(unsigned long sampleNumber)
{
    m_sampleNumber = sampleNumber;
}

unsigned long FizzyAde::ICMPPingEngine::ICMPPingItem::sampleNumber()
{
    return(m_sampleNumber);
}

void FizzyAde::ICMPPingEngine::ICMPPingItem::lock()
{
    m_mutex.lock();
}

void FizzyAde::ICMPPingEngine::ICMPPingItem::unlock()
{
    m_mutex.unlock();
}
