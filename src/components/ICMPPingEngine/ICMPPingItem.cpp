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

#include "ICMPPingItem.h"

#include <QDebug>
#include <QTimer>

Nedrysoft::ICMPPingEngine::ICMPPingItem::ICMPPingItem() :
        m_id(0),
        m_sequenceId(0),
        m_serviced(false),
        m_target(nullptr),
        m_sampleNumber(0) {

}

Nedrysoft::ICMPPingEngine::ICMPPingItem::~ICMPPingItem() = default;

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::setId(uint16_t id) -> void {
    m_id = id;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::setSequenceId(uint16_t sequence) -> void {
    m_sequenceId = sequence;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::sequenceId() -> uint16_t {
    return m_sequenceId;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::setTransmitTime(
        std::chrono::high_resolution_clock::time_point time,
        std::chrono::system_clock::time_point epoch) -> void {

    m_transmitTime = time;
    m_transmitEpoch = epoch;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::setServiced(bool serviced) -> void {
    m_serviced = serviced;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::serviced() -> bool {
    return m_serviced;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::id() -> uint16_t {
    return m_id;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::setTarget(Nedrysoft::ICMPPingEngine::ICMPPingTarget *target) -> void {
    m_target = target;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::target() -> Nedrysoft::ICMPPingEngine::ICMPPingTarget * {
    return m_target;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::transmitTime() -> std::chrono::high_resolution_clock::time_point {
    return m_transmitTime;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::transmitEpoch() -> std::chrono::system_clock::time_point {
    return m_transmitEpoch;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::setSampleNumber(unsigned long sampleNumber) -> void {
    m_sampleNumber = sampleNumber;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::sampleNumber() -> unsigned long {
    return m_sampleNumber;
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::lock() -> void {
    m_mutex.lock();
}

auto Nedrysoft::ICMPPingEngine::ICMPPingItem::unlock() -> void {
    m_mutex.unlock();
}
