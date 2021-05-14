/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 20/04/2021.
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

#ifndef PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGRESULT_H
#define PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGRESULT_H

#include <PingResult>

#include <QObject>

namespace Nedrysoft { namespace RouteAnalyser {
    class IPingTarget;
}}

namespace Nedrysoft { namespace ICMPAPIPingEngine {
    /**
     * @brief       The ICMPAPIPingResult is an internal class used by this ping engine that allows the sample number
     *              and target fields to be set.  It is required because the ping engine makes use of the singleShot
     *              member of the ping engine which nulls those fields, this allows us to set the correct data
     *              in the response from singleShot.
     */
    class ICMPAPIPingResult :
            public QObject,
            public Nedrysoft::RouteAnalyser::PingResult {

    private:
        Q_OBJECT

    public:
        /**
         * @brief       Constructs a new ICMPAPIPingResult from an existing PingResult.
         *
         * @param[in]   result the ping result to copy.
         */
        ICMPAPIPingResult(Nedrysoft::RouteAnalyser::PingResult result);

        /**
         * @brief       Sets the sample number.
         *
         * @param[in]   sampleNumber the sample number.
         */
        void setSampleNumber(int sampleNumber);

        /**
         * @brief       Sets the target.
         *
         * @param[in]   target the target.
         */
        void setTarget(Nedrysoft::RouteAnalyser::IPingTarget *target);
    };
}}

#endif //PINGNOO_COMPONENTS_ICMPAPIPINGENGINE_ICMPAPIPINGRESULT_H
