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

#include "ICMPAPIPingItem.h"

#include <QDebug>
#include <QTimer>

class Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItemData {
    public:
        explicit ICMPAPIPingItemData(Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem *parent) :
                m_pingItem(parent),
                m_id(0),
                m_sequenceId(0),
                m_serviced(false),
                m_target(nullptr),
                m_sampleNumber(0) {

        }

        friend class ICMPAPIPingItem;

    private:
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem *m_pingItem;

        std::chrono::high_resolution_clock::time_point m_transmitTime;

        uint16_t m_id;
        uint16_t m_sequenceId;

        unsigned long m_sampleNumber;

        bool m_serviced;

        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *m_target;
};

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::ICMPAPIPingItem() :
        d(std::make_shared<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItemData>(this)) {

}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::setId(uint16_t id) -> void {
    d->m_id = id;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::setSequenceId(uint16_t sequence) -> void {
    d->m_sequenceId = sequence;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::sequenceId() -> uint16_t {
    return d->m_sequenceId;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::setTransmitTime(
        std::chrono::high_resolution_clock::time_point time ) -> void {

    d->m_transmitTime = time;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::setServiced(bool serviced) -> void {
    d->m_serviced = serviced;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::serviced() -> bool {
    return d->m_serviced;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::id() -> uint16_t {
    return d->m_id;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::setTarget(
        Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget *target) -> void {

    d->m_target = target;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::target() -> Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingTarget * {
    return d->m_target;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::transmitTime() -> std::chrono::high_resolution_clock::time_point {
    return d->m_transmitTime;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::setSampleNumber(unsigned long sampleNumber) -> void {
    d->m_sampleNumber = sampleNumber;
}

auto Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItem::sampleNumber() -> unsigned long {
    return d->m_sampleNumber;
}
