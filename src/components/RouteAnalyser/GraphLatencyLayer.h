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

#ifndef FIZZYADE_ROUTEANALYSER_GRAPHLATENCYLAYER_H
#define FIZZYADE_ROUTEANALYSER_GRAPHLATENCYLAYER_H

#include "QCustomPlot/qcustomplot.h"
#include "chrono"

namespace FizzyAde::RouteAnalyser
{
    class GraphLatencyLayer :
        public QCPItemRect
    {
    public:
        GraphLatencyLayer(QCustomPlot *customPlot);

    protected:
        void draw(QCPPainter *painter);

    private:
        std::chrono::duration<double> m_lowRangeLatency = {};            //! The lowest latency seen by this item
        std::chrono::duration<double> m_midRangeLatency = {};            //! The highest latencty seen by this item
    };
}

#endif // FIZZYADE_ROUTEANALYSER_GRAPHLATENCYLAYER_H
