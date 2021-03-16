/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 16/03/2021.
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

#ifndef NEDRYSOFT_JITTERPLOT_JITTERPLOT_H
#define NEDRYSOFT_JITTERPLOT_JITTERPLOT_H

#include "RouteAnalyser/SDK/IPlot.h"

#include "QCustomPlot/qcustomplot.h"

namespace Nedrysoft::JitterPlot {
    /**
     * @brief       Implementation of a jitter plot.
     */
    class JitterPlot :
            public Nedrysoft::RouteAnalyser::IPlot {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::RouteAnalyser::IPlot)

        public:
            /**
             * @brief       Constructs an JitterPlot.
             */
            JitterPlot();

            /**
             * @brief       Constructs the JitterPlot.
             */
            ~JitterPlot();

        public:
            /**
             * @brief       Returns the widget for this plot.
             *
             * @returns     the widget.
             */
            auto widget() -> QWidget * override;

            auto update(double time, double value) -> void override;

            auto updateRange(double min, double max) -> void override;

        private:
            QCustomPlot *m_customPlot;
            double m_previousValue;
    };
}

#endif // NEDRYSOFT_JITTERPLOT_JITTERPLOT_H
