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

#ifndef NEDRYSOFT_ICMPPINGENGINE_ICMPPINGITEM_H
#define NEDRYSOFT_ICMPPINGENGINE_ICMPPINGITEM_H

#include <QMutex>
#include <QObject>
#include <chrono>

namespace Nedrysoft::ICMPPingEngine {
    class ICMPPingTarget;

    /**
     * @brief       The ICMPPingItem class stores information to track requests & responses.
     *
     * @details     The ICMPPingTransmitter instance registers each ping request with the engine, this class holds the
     *              required information to allow replies to be matched to requests (and timed) and also to allow
     *              timeouts to be discovered.
     */
    class ICMPPingItem :
            public QObject {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs an ICMPPingItem.
             */
            ICMPPingItem();

            /**
             * @brief       Destroys the ICMPPingItem.
             */
            ~ICMPPingItem();

            /**
             * @brief       Sets the id used in the ping request.
             *
             * @param[in]   id the id field for this request.
             */
            auto setId(uint16_t id) -> void;

            /**
             * @brief       Returns the id used in the ping request.
             *
             * @returns     id of the request.
             */
            auto id(void) -> uint16_t;

            /**
             * @brief       Sets the sequence id used in the ping request.
             *
             * @param[in]   sequence the sequence id to use.
             */
            auto setSequenceId(uint16_t sequence) -> void;

            /**
             * @brief       Returns the sequence id used in the ping request.
             *
             * @returns     the sequence id.
             */
            auto sequenceId() -> uint16_t;

            /**
             * @brief       Set whether this request has been serviced.
             *
             * @details     Marks the request as being serviced, prevents a packet being flagged as both replied to
             *              and timeout in race condition situations.
             *
             * @param[in]   serviced true if serviced; otherwise false.
             */
            auto setServiced(bool serviced) -> void;

            /**
             * @brief       Returns the serviced status of the request.
             *
             * @returns     true if request has been serviced; otherwise false.
             */
            auto serviced() -> bool;

            /**
             * @brief       Sets the sample number for this request.
             *
             * @param[in]   sampleNumber the sample number.
             */
            auto setSampleNumber(unsigned long sampleNumber) -> void;

            /**
             * @brief       Returns the sample number for this request.
             *
             * @returns     the sample number.
             */
            auto sampleNumber() -> unsigned long;

            /**
              * @brief       Sets the target associated with this request.
              *
              * @param[in]   target the target.
              */
            auto setTarget(Nedrysoft::ICMPPingEngine::ICMPPingTarget *target) -> void;

            /**
             * @brief       Returns the target associated with this request.
             *
             * @returns     the request.
             */
            auto target() -> Nedrysoft::ICMPPingEngine::ICMPPingTarget *;

            /**
             * @brief       Sets the time at which the request was transmitted.
             *
             * @param[in]   time the high resolution clock time.
             * @param[in]   epoch the epoch for transmission.
             */
            auto setTransmitTime(
                    std::chrono::high_resolution_clock::time_point time,
                    std::chrono::system_clock::time_point epoch ) -> void;

            /**
             * @brief       Returns the time at which the request was transmitted.
             *
             * @returns     the high resolution clock time when the request was sent.
             */
            auto transmitTime(void) -> std::chrono::high_resolution_clock::time_point;

            /**
             * @brief       Returns the epoch at which the request was transmitted.
             *
             * @returns     the epoch when the request was sent.
             */
            auto transmitEpoch() -> std::chrono::system_clock::time_point;

            /**
             * @brief       Locks the item for exclusive access.
             */
            auto lock(void) -> void;

            /**
             * @brief       Unlocks the item that was previous locked.
             */
            auto unlock(void) -> void;

        private:
            std::chrono::high_resolution_clock::time_point m_transmitTime;
            std::chrono::system_clock::time_point m_transmitEpoch;

            uint16_t m_id;
            uint16_t m_sequenceId;

            bool m_serviced;

            Nedrysoft::ICMPPingEngine::ICMPPingTarget *m_target;

            unsigned long m_sampleNumber;

            QMutex m_mutex;
    };
}

#endif // NEDRYSOFT_ICMPPINGENGINE_ICMPPINGITEM_H
