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

#ifndef NEDRYSOFT_ICMPPINGENGINE_ICMPPINGITEM_H
#define NEDRYSOFT_ICMPPINGENGINE_ICMPPINGITEM_H

#include <QMutex>
#include <QObject>
#include <chrono>

namespace Nedrysoft::ICMPPingEngine {
    class ICMPPingTarget;

    /**
     * @brief       Object used to store information about a tracked request
     *
     * @details     The FZICMPPingTransmitter registers each ping request with the
     *              engine, this class holds the required information to allow
     *              replies to be matched to requests (and timed) and also to allow
     *              timeouts to be discovered.
     */
    class ICMPPingItem :
            public QObject {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructor
             */
            ICMPPingItem();

            /**
             * @brief       Destructor
             */
            ~ICMPPingItem();

            /**
             * @brief       Sets the id used in the ping request
             *
             * @param[in]   id the id field for this request
             */
            void setId(uint16_t id);

            /**
             * @brief       Returns the id used in the ping request
             *
             * @return      id of the request
             */
            uint16_t id(void);

            /**
             * @brief       Sets the sequence id used in the ping request
             *
             * @param[in]   sequence the sequence id to use
             */
            void setSequenceId(uint16_t sequence);

            /**
             * @brief       Returns the sequence id used in the ping request
             *
             * @return      the sequence id
             */
            uint16_t sequenceId();

            /**
             * @brief       Set whether this request has been serviced
             *
             * @details     Marks the request as being serviced, prevents a packet
             *              being flagged as both replied to and timeout in race
             *              condition situations.
             *
             * @param[in]   serviced true if serviced; otherwise false.
             */
            void setServiced(bool serviced);

            /**
             * @brief       Returns the serviced status of the request
             *
             * @return      true if request has been serviced; otherwise false.
             */
            bool serviced();

            /**
             * @brief       Sets the sample number for this request
             *
             * @param[in]   sampleNumber the sample number
             */
            void setSampleNumber(unsigned long sampleNumber);

            /**
             * @brief       Returns the sample number for this request
             *
             * @return      the sample number
             */
            unsigned long sampleNumber();

            /**
             * @brief       Sets the target associated with this request
             *
             * @param[in]   target the target
             */
            void setTarget(Nedrysoft::ICMPPingEngine::ICMPPingTarget *target);

            /**
             * @brief       Returns the target associated with this request
             *
             * @return      the request
             */
            Nedrysoft::ICMPPingEngine::ICMPPingTarget *target();

            /**
             * @brief       Sets the time at which the request was transmitted
             *
             * @param[in]   time the high resolution clock time
             * @param[in]   epoch the system time
             */
            void setTransmitTime(std::chrono::high_resolution_clock::time_point time,
                                 std::chrono::system_clock::time_point epoch);

            /**
             * @brief       Returns the time at which the request was transmitted
             *
             * @return      the high resolution clock time when the request was sent
             */
            std::chrono::high_resolution_clock::time_point transmitTime(void);

            /**
             * @brief       Returns the time at which the request was transmitted
             *
             * @return      the system clock time when the request was sent
             */
            std::chrono::system_clock::time_point transmitEpoch(void);

            /**
             * @brief       Locks the item for exclusive access
             */
            void lock(void);

            /**
             * @brief       Locks the item for exclusive access
             */
            void unlock(void);

        private:
            std::chrono::high_resolution_clock::time_point m_transmitTime;
            std::chrono::system_clock::time_point m_transmitEpoch;

            uint16_t m_id;
            uint16_t m_sequenceId;

            unsigned long m_sampleNumber;

            bool m_serviced;

            Nedrysoft::ICMPPingEngine::ICMPPingTarget *m_target;

            QMutex m_mutex;
    };
}

#endif // NEDRYSOFT_ICMPPINGENGINE_ICMPPINGITEM_H
