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

#include "ICMPAPIPingItem.h"
#include <QTimer>
#include <QDebug>

class Nedrysoft::Pingnoo::ICMPAPIPingItemData {
    public:
        ICMPAPIPingItemData(Nedrysoft::Pingnoo::ICMPAPIPingItem *parent) {
            m_pingItem = parent;
            m_id = 0;
            m_sequenceId = 0;
            m_serviced = false;
            m_target = nullptr;
            m_sampleNumber = 0;
        }

        friend class ICMPAPIPingItem;

    private:
        Nedrysoft::Pingnoo::ICMPAPIPingItem *m_pingItem;

        std::chrono::high_resolution_clock::time_point m_transmitTime;

        uint16_t m_id;
        uint16_t m_sequenceId;

        unsigned long m_sampleNumber;

        bool m_serviced;

        Nedrysoft::Pingnoo::ICMPAPIPingTarget *m_target;
};

Nedrysoft::Pingnoo::ICMPAPIPingItem::ICMPAPIPingItem() :
        d(std::make_shared<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingItemData>(this)) {
}

void Nedrysoft::Pingnoo::ICMPAPIPingItem::setId(uint16_t id) {
    d->m_id = id;
}

void Nedrysoft::Pingnoo::ICMPAPIPingItem::setSequenceId(uint16_t sequence) {
    d->m_sequenceId = sequence;
}

uint16_t Nedrysoft::Pingnoo::ICMPAPIPingItem::sequenceId() {
    return ( d->m_sequenceId );
}

void Nedrysoft::Pingnoo::ICMPAPIPingItem::setTransmitTime(std::chrono::high_resolution_clock::time_point time) {
    d->m_transmitTime = time;
}

void Nedrysoft::Pingnoo::ICMPAPIPingItem::setServiced(bool serviced) {
    d->m_serviced = serviced;
}

bool Nedrysoft::Pingnoo::ICMPAPIPingItem::serviced() {
    return ( d->m_serviced );
}

uint16_t Nedrysoft::Pingnoo::ICMPAPIPingItem::id() {
    return ( d->m_id );
}

void Nedrysoft::Pingnoo::ICMPAPIPingItem::setTarget(Nedrysoft::Pingnoo::ICMPAPIPingTarget *target) {
    d->m_target = target;
}

Nedrysoft::Pingnoo::ICMPAPIPingTarget *Nedrysoft::Pingnoo::ICMPAPIPingItem::target() {
    return ( d->m_target );
}

std::chrono::high_resolution_clock::time_point Nedrysoft::Pingnoo::ICMPAPIPingItem::transmitTime() {
    return ( d->m_transmitTime );
}

void Nedrysoft::Pingnoo::ICMPAPIPingItem::setSampleNumber(unsigned long sampleNumber) {
    d->m_sampleNumber = sampleNumber;
}

unsigned long Nedrysoft::Pingnoo::ICMPAPIPingItem::sampleNumber() {
    return ( d->m_sampleNumber );
}
