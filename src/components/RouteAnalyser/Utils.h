/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 17/12/2020.
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

#ifndef NEDRYSOFT_UTILS_H
#define NEDRYSOFT_UTILS_H

#include <limits.h>
#include <stdint.h>

#include <QString>

namespace Nedrysoft::Utils {
    /**
     * @brief       Convert 16 bit words to 32 bit.
     *
     * @note        this uses the constant CHAR_BIT rather than the immediate value 8 to prevent warnings from
     *              clang-tidy.
     *
     * @param[in]   high the high 16 bits.
     * @param[in]   low the low 16 bits.
     *
     * @returns     the 32 bit result.
     */
    constexpr auto fzMake32(uint16_t high, uint16_t low) -> uint32_t {
        return ( static_cast<uint32_t>(( high << ( sizeof(high) * CHAR_BIT ) | low )));
    }

    /**
     * @brief       Validates and returns the interval from a string.
     *
     * @details     Handles strings with or without suffixes, 2.0 100ms etc.
     *
     * @param[in]   string the string to validate.
     * @param[out]  intervalValue the interval value in seconds (if valid).
     *
     * @returns     true if interval was valid; otherwise false;
     */
    auto parseIntervalString(QString intervalString, double &intervalValue) -> bool;

    /**
     * @brief       Validates an interval string.
     *
     * @details     Handles strings with or without suffixes, 2.0 100ms etc.
     *
     * @param[in]   string the string to validate.
     *
     * @returns     true if interval was valid; otherwise false;
     */
    auto parseIntervalString(QString intervalString) -> bool;

    /**
     * @brief       Check if the host is a valid host name or ip address.
     *
     * @param[in]   host the host to check.
     *
     * @returns     true if valid; otherwise false.
     */
    auto checkHostValid(const QString &host) -> bool;

    /**
     * @brief       Converts seconds to a interval string with units.
     *
     * @param[in]   value the value in seconds.
     *
     * @returns     the interval as a string
     */
    auto intervalToString(double value) -> QString;
}

#endif // NEDRYSOFT_UTILS_H
