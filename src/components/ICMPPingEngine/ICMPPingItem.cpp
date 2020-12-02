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

#include <QDebug>
#include <QMutex>
#include <QTimer>

Nedrysoft::ICMPPingEngine::ICMPPingItem::ICMPPingItem() :
        m_id(0),
        m_sequenceId(0),
        m_serviced(false),
        m_target(nullptr),
        m_sampleNumber(0) {

}

Nedrysoft::ICMPPingEngine::ICMPPingItem::~ICMPPingItem() = default;

void Nedrysoft::ICMPPingEngine::ICMPPingItem::setId(uint16_t id) {
    m_id = id;
}

void Nedrysoft::ICMPPingEngine::ICMPPingItem::setSequenceId(uint16_t sequence) {
    m_sequenceId = sequence;
}

uint16_t Nedrysoft::ICMPPingEngine::ICMPPingItem::sequenceId() {
    return m_sequenceId;
}

void Nedrysoft::ICMPPingEngine::ICMPPingItem::setTransmitTime(std::chrono::high_resolution_clock::time_point time,
                                                              std::chrono::system_clock::time_point epoch) {
    m_transmitTime = time;
    m_transmitEpoch = epoch;
}

void Nedrysoft::ICMPPingEngine::ICMPPingItem::setServiced(bool serviced) {
    m_serviced = serviced;
}

bool Nedrysoft::ICMPPingEngine::ICMPPingItem::serviced() {
    return m_serviced;
}

uint16_t Nedrysoft::ICMPPingEngine::ICMPPingItem::id() {
    return m_id;
}

void Nedrysoft::ICMPPingEngine::ICMPPingItem::setTarget(Nedrysoft::ICMPPingEngine::ICMPPingTarget *target) {
    m_target = target;
}

Nedrysoft::ICMPPingEngine::ICMPPingTarget *Nedrysoft::ICMPPingEngine::ICMPPingItem::target() {
    return m_target;
}

std::chrono::high_resolution_clock::time_point Nedrysoft::ICMPPingEngine::ICMPPingItem::transmitTime() {
    return m_transmitTime;
}

std::chrono::system_clock::time_point Nedrysoft::ICMPPingEngine::ICMPPingItem::transmitEpoch() {
    return m_transmitEpoch;
}

void Nedrysoft::ICMPPingEngine::ICMPPingItem::setSampleNumber(unsigned long sampleNumber) {
    m_sampleNumber = sampleNumber;
}

unsigned long Nedrysoft::ICMPPingEngine::ICMPPingItem::sampleNumber() {
    return m_sampleNumber;
}

void Nedrysoft::ICMPPingEngine::ICMPPingItem::lock() {
    m_mutex.lock();
}

void Nedrysoft::ICMPPingEngine::ICMPPingItem::unlock() {
    m_mutex.unlock();
}
