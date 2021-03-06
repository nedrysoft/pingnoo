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

#ifndef NEDRYSOFT_ROUTEANALYSER_GRAPHLATENCYLAYER_H
#define NEDRYSOFT_ROUTEANALYSER_GRAPHLATENCYLAYER_H

#include "QCustomPlot/qcustomplot.h"
#include "chrono"

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The GraphLatencyLayer renders the background with the latency colours.
     *
     * @details     Draws the background of a chart showing the latency colouring and markers for the latency levels
     *              in a QCustomPlot chart.
     */
    class GraphLatencyLayer :
            public QCPItemRect {

        public:
            /**
             * @brief       Constructs a new GraphLatencyLayer.
             */
            GraphLatencyLayer(QCustomPlot *customPlot);

            /**
             * @brief       Set whether to draw the background as a smooth gradient or steps.
             *
             * @param[in]   useGradient true if smooth gradient; otherwise false.
             */
            auto setGradientEnabled(bool useGradient) -> void;

            /**
             * @brief       Removes buffered offscreen background Pixmaps.
             */
            static auto removeUnused() -> void;

            /**
             * @brief       Clears all offscreen buffers
             */
            auto invalidate() -> void;

        protected:
            /**
             * @brief       Draws the layer to the given painter.
             *
             * @param[in]   painter the QPainter to draw in.
             */
            auto draw(QCPPainter *painter) -> void;

        private:
            std::chrono::duration<double> m_warningLatency = {};
            std::chrono::duration<double> m_criticalLatency = {};

            bool m_useGradient;

            static QMap<QString, QPixmap> m_buffers;
            static QMap<QString, uint64_t> m_age;
    };
}

#endif // NEDRYSOFT_ROUTEANALYSER_GRAPHLATENCYLAYER_H
