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

#ifndef NEDRYSOFT_ROUTEANALYSER_CPAXISTICKERMS_H
#define NEDRYSOFT_ROUTEANALYSER_CPAXISTICKERMS_H

#include "QCustomPlot/qcustomplot.h"

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The QCPAxisTicker is a QCustomPlot Axis ticker class that displays milliseconds.
     */
    class CPAxisTickerMS :
            public QCPAxisTicker {

        public:
            /**
             * @brief       Returns a string which expressess the given tick as milliseconds.
             *
             * @param[in]   tick the tick value.
             * @param[in]   locale the locale to be used.
             * @param[in]   formatChar the format used for floating point conversions.  (f or g)
             * @param[in]   precision the precision used for floating point conversions.
             *
             * @returns     the tick formatted as a string.
             */
            virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision);
    };
}

#endif // NEDRYSOFT_ROUTEANALYSER_CPAXISTICKERMS_H
