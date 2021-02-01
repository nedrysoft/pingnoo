/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/12/2020.
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

#ifndef NEDRYSOFT_COLOURMANAGER_H
#define NEDRYSOFT_COLOURMANAGER_H

#include <QRgb>

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The ColourManager class is a convenience class for getting the min, mid and max colours, these
     *              change according to light or dark mode.
     */
    class ColourManager {
        public:
            /**
             * @brief       Returns the ideal latency colour (low latency).
             *
             * @returns     the minimum colour
             */
            static auto getIdealColour() -> QRgb;

            /**
             * @brief       Returns the warning latency colour (mid latency).
             *
             * @returns     the median colour
             */
            static auto getWarningColour() -> QRgb;

            /**
             * @brief       Returns the critical latency colour (high latency).
             *
             * @returns     the maximum colour
             */
            static auto getCriticalColour() -> QRgb;
    };
}

#endif //NEDRYSOFT_COLOURMANAGER_H
