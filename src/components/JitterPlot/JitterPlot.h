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
    class JitterBackgroundLayer;

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
            JitterPlot(const QMargins &margins);

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

            /**
             * @brief       Updates the plot with a new result.
             * @param[in]   time the unix timestamp for this result.
             * @param[in]   value the round trip time.
             */
            auto update(double time, double value) -> void override;

            /**
             * @brief       Update the visible area (viewport) of the graph.
             * @param[in]   min the minimum displayed value.
             * @param[in]   max the maximum dispkayed value.
             */
            auto updateRange(double min, double max) -> void override;

            /**
             * @brief       Sets the jitter range.
             *
             * @notes       The target value is drawn as a dash on the jitter graph, the maximum displayed
             *              value is set by max.
             *
             * @param[in]   target the target value in seconds.
             * @param[in]   max the maximum value to show on the jitter graph.
             */
            auto setRange(double target, double max) -> void override;

        private:
            QCustomPlot *m_customPlot;
            double m_previousValue;
            Nedrysoft::JitterPlot::JitterBackgroundLayer *m_backgroundLayer;
            QMargins m_margins;
    };
}

#endif // NEDRYSOFT_JITTERPLOT_JITTERPLOT_H
