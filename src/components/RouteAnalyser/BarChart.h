/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 28/12/2020.
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

#ifndef NEDRYSOFT_ROUTEANALYSER_BARCHART_H
#define NEDRYSOFT_ROUTEANALYSER_BARCHART_H

#pragma warning(push)
#pragma warning(disable : 4996)

#include "QCustomPlot/qcustomplot.h"

#pragma warning(pop)

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The BarChart class is a subclass of QCPBars which clips the chart to a rounded rectangle.
     */
    class BarChart :
            public QCPBars {

        public:
            /**
             * @brief       Constructs a new BarChart and attaches it to the given axis.
             * @param[in]   keyAxis the axis for the keys.
             * @param[in]   valueAxis the axis for the values,
             */
            BarChart(QCPAxis *keyAxis, QCPAxis *valueAxis);

        protected:
            /**
             * @brief       Draws the bar chart to the given painter.
             *
             * @param[in]   painter the QPainter to draw in.
             */
            virtual void draw(QCPPainter *painter);
    };
}

#endif //NEDRYSOFT_ROUTEANALYSER_BARCHART_H
