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

#ifndef FIZZYADE_ICMPPINGENGINE_ICMPPINGITEM_H
#define FIZZYADE_ICMPPINGENGINE_ICMPPINGITEM_H

#include <chrono>
#include <QObject>
#include <QMutex>

namespace FizzyAde::ICMPPingEngine
{
    class ICMPPingTarget;

    /**
     * Object used to store information about a tracked request
     *
     * The FZICMPPingTransmitter registers each ping request with the
     * engine, this class holds the required information to allow
     * replies to be matched to requests (and timed) and also to allow
     * timeouts to be discovered.
     *
     */

    class ICMPPingItem : public QObject
    {
        Q_OBJECT

    public:
        ICMPPingItem();
        ~ICMPPingItem();

        /**
         * Sets the id used in the ping request
         *
         * @param[in]   id              id for this request
         *
         */
        void setId(uint16_t id);

        /**
         * Returns the id used in the ping request
         *
         * @return      id of the request
         *
         */
        uint16_t id(void);

        /**
         * Sets the sequence id used in the ping request
         *
         * @param[in]   sequence        the sequence id to use
         *
         */
        void setSequenceId(uint16_t sequence);

        /**
         * Returns the sequence id used in the ping request
         *
         * @return      the sequence id
         *
         */
        uint16_t sequenceId();

        /**
         * Set whether this request has been serviced
         *
         * Marks the request as being serviced, prevents a packet
         * being flagged as both replied to and timeout in race
         * condition situations.
         *
         * @param[in]   serviced        true if serviced, else false
         *
         */
        void setServiced(bool serviced);

        /**
         * Returns the serviced status of the request
         *
         * @return      true if serviced, else false
         *
         */
        bool serviced();

        /**
         * Sets the sample number for this request
         *
         * @param[in]   sampleNumber    the sample number
         *
         */
        void setSampleNumber(unsigned long sampleNumber);

        /**
         * Returns the sample number for this request
         *
         * @return      the sample number
         */
        unsigned long sampleNumber();

        /**
         * Sets the target associated with this request
         *
         * @param[in] target            the target
         *
         */
        void setTarget(FizzyAde::ICMPPingEngine::ICMPPingTarget *target);

        /**
         * Returns the target associated with this request
         *
         * @return      the request
         *
         */
        FizzyAde::ICMPPingEngine::ICMPPingTarget *target();

        /**
         * Sets the time at which the request was transmitted
         *
         * @param[in]   time            the high resolution clock time
         * @param[in]   epoch           the system time
         *
         */
        void setTransmitTime(std::chrono::high_resolution_clock::time_point time, std::chrono::system_clock::time_point epoch);

        /**
         * Returns the time at which the request was transmitted
         *
         * @return      the high resolution clock time when the request was sent
         *
         */
        std::chrono::high_resolution_clock::time_point transmitTime(void);

        /**
         * Returns the time at which the request was transmitted
         *
         * @return      the system clock time when the request was sent
         *
         */
        std::chrono::system_clock::time_point transmitEpoch(void);

        /**
         * Locks the item for exclusive access
         *
         */
        void lock(void);

        /**
         * Locks the item for exclusive access
         *
         */
        void unlock(void);

    private:
        std::chrono::high_resolution_clock::time_point m_transmitTime;
        std::chrono::system_clock::time_point m_transmitEpoch;

        uint16_t m_id;
        uint16_t m_sequenceId;

        unsigned long m_sampleNumber;

        bool m_serviced;

        FizzyAde::ICMPPingEngine::ICMPPingTarget *m_target;

        QMutex m_mutex;
    };
}

#endif // FIZZYADE_ICMPPINGENGINE_ICMPPINGITEM_H
