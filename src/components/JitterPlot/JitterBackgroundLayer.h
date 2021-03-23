/*
 * Copyright (C) 2020 Adrian Carpenter
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

#ifndef NEDRYSOFT_JITTERPLOT_JITTERBACKGROUNDLAYER_H
#define NEDRYSOFT_JITTERPLOT_JITTERBACKGROUNDLAYER_H

#include "QCustomPlot/qcustomplot.h"
#include "chrono"

namespace Nedrysoft::JitterPlot {
    /**
     * @brief       The GraphLatencyLayer renders the background with the latency colours.
     *
     * @details     Draws the background of a chart showing the latency colouring and markers for the latency levels
     *              in a QCustomPlot chart.
     */
    class JitterBackgroundLayer :
            public QCPItemRect {

        public:
            /**
             * @brief       Constructs a new GraphLatencyLayer.
             */
            JitterBackgroundLayer(QCustomPlot *customPlot);

            /**
             * @brief       Sets the range for the jitter graph.
             *
             * @param[in]   target the target jitter.
             * @param[in]   max the maximum displayed jitter value.
             */
            auto setRange(double target, double max) -> void;

        protected:
            /**
             * @brief       Draws the layer to the given painter.
             *
             * @param[in]   painter the QPainter to draw in.
             */
            auto draw(QCPPainter *painter) -> void;

        private:
            double m_targetJitter;
            double m_maximumJitter;
            QPixmap m_bufferedImage;
    };
}

#endif // NEDRYSOFT_JITTERPLOT_JITTERBACKGROUNDLAYER_H
