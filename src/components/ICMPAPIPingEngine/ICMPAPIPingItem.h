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

#ifndef NEDRYSOFT_PINGNOO_ICMPAPIPINGITEM_H
#define NEDRYSOFT_PINGNOO_ICMPAPIPINGITEM_H

#include "ICMPAPIPingEngineSpec.h"

#include <QObject>
#include <chrono>

namespace Nedrysoft::Pingnoo {
    class ICMPAPIPingTarget;

    class ICMPAPIPingItemData;

    /**
     * @brief       The ICMPAPIPingItem stores information about a ping request.
     *
     * @details     The ICMPPingTransmitter registers each ping request with the engine, this class holds the required
     *              information to allow replies to be matched to requests (and timed) and also to allow timeouts to
     *              be generated.
     */
    class ICMPAPIPingItem :
            public QObject {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs an ICMPAPIPingItem.
             */
            ICMPAPIPingItem();

            /**
             * @brief       Sets the id field of the ping request.
             *
             * @notes       Some platforms do not allow setting of the id field and will ignore it, instead they
             *              may use the process id or something similar instead.
             *
             * @param[in]   id the idendfier of the request.
             */
            void setId(uint16_t id);

            /**
             * @brief       Sets the id field of the ping request.
             *
             * @notes       Some platforms do not allow setting of the id field and will ignore it, instead they
             *              may use the process id or something similar instead.
             *
             * @param[in]   id the idendfier of the request.
             */
            void setId(uint16_t id);

            /**
             * @brief       Returns the id used in the ping request.
             *
             * @returns     the id.
             */
            uint16_t id(void);

            /**
             * @brief       Sets the sequence id used in the ping request.
             *
             * @param[in]   sequence the sequence identifier.
             */
            void setSequenceId(uint16_t sequence);

            /**
             * @brief       Set whether this request has been serviced.
             *
             * @details     Marks the request as being serviced, prevents a packet being flagged as both replied to
             *              and timeout in race condition situations.
             *
             * @param[in]   serviced true if serviced; otherwise false.
             */
            void setServiced(bool serviced);

            /**
             * @brief       Returns the serviced status of the request.
             *
             * @return      true if request has been serviced; otherwise false.
             */
            bool serviced();

            /**
             * @brief       Sets the sample number for this request.
             *
             * @param[in]   sampleNumber the sample number.
             */
            void setSampleNumber(unsigned long sampleNumber);

            /**
             * @brief       Returns the sample number for this request.
             *
             * @return      the sample number.
             */
            unsigned long sampleNumber();

            /**
             * @brief       Sets the target associated with this request.
             *
             * @param[in]   target the target.
             */
            void setTarget(Nedrysoft::Pingnoo::ICMPAPIPingTarget *target);

            /**
             * @brief       Returns the target associated with this request.
             *
             * @returns     the request.
             */
            Nedrysoft::Pingnoo::ICMPAPIPingTarget *target();

            /**
             * @brief       Sets the time at which the request was transmitted.
             *
             * @param[in]   time the high resolution clock time.
             */
            void setTransmitTime(std::chrono::high_resolution_clock::time_point time);

            /**
             * @brief       Returns the time at which the request was transmitted.
             *
             * @returns     the high resolution clock time when the request was sent.
             */
            std::chrono::high_resolution_clock::time_point transmitTime(void);

        protected:

            std::shared_ptr<ICMPAPIPingItemData> d;
    };
}

#endif // NEDRYSOFT_PINGNOO_ICMPAPIPINGITEM_H
