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

#ifndef FIZZYADE_PINGNOO_ICMPAPIPINGITEM_H
#define FIZZYADE_PINGNOO_ICMPAPIPINGITEM_H

#include "ICMPAPIPingEngineSpec.h"
#include <chrono>
#include <QObject>

namespace FizzyAde::Pingnoo
{
    class ICMPAPIPingTarget;

    class ICMPAPIPingItemData;

    /**
     * Object used to store information about a tracked request
     *
     * The FZICMPPingTransmitter registers each ping request with the
     * engine, this class holds the required information to allow
     * replies to be matched to requests (and timed) and also to allow
     * timeouts to be discovered.
     *
     */

    class ICMPAPIPingItem : public QObject
    {
        Q_OBJECT

    public:
        /**
         * Default constructor
         */
        ICMPAPIPingItem();

        /**
         * Sets the id used in the ping request
         *
         * @param[in] id the id to use
         */
        void setId(uint16_t id);

        /**
         * Returns the id used in the ping request
         *
         * @return the id
         */
        uint16_t id(void);

        /**
         * Sets the sequence id used in the ping request
         *
         * @param[in] sequence the sequence id to use
         */
        void setSequenceId(uint16_t sequence);

        /**
         * Returns the sequence id used in the ping request
         *
         * @return the sequence id
         */
        uint16_t sequenceId();

        /**
         * Marks the request as being serviced, prevents a packet
         * being flagged as both replied to and timeout in race
         * condition situations.
         *
         * @param[in] serviced true if serviced, else false
         */
        void setServiced(bool serviced);

        /**
         * Returns the serviced status of the request
         *
         * @return true if it has been serviced, else false
         */
        bool serviced();

        /**
         * Sets the sample number for this request
         *
         * @param[in] sampleNumber the sample number
         */
        void setSampleNumber(unsigned long sampleNumber);

        /**
         * Returns the sample number for this request
         *
         * @return the sample number
         */
        unsigned long sampleNumber();

        /**
         * Sets the target associated with this request
         *
         * @param[in] target the target
         */
        void setTarget(FizzyAde::Pingnoo::ICMPAPIPingTarget *target);

        /**
         * Returns the target associated with this request
         *
         * @return the request
         */
        FizzyAde::Pingnoo::ICMPAPIPingTarget *target();

        /**
         * Sets the time at which the request was transmitted
         *
         * @param[in] time the high resolution clock time
         */
        void setTransmitTime(std::chrono::high_resolution_clock::time_point time);

        /**
         * Returns the time at which the request was transmitted
         *
         * @return the high resolution clock time when the request was sent
         */
        std::chrono::high_resolution_clock::time_point transmitTime(void);

    protected:

        std::shared_ptr<ICMPAPIPingItemData> d;
    };
}

#endif // FIZZYADE_PINGNOO_ICMPAPIPINGITEM_H
