/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 14/03/2021.
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

#ifndef NEDRYSOFT_JITTERPLOT_JITTERPLOTFACTORY_H
#define NEDRYSOFT_JITTERPLOT_JITTERPLOTFACTORY_H

#include "RouteAnalyser/SDK/IPlotFactory.h"
#include "RouteAnalyser/SDK/IPlot.h"

namespace Nedrysoft::JitterPlot {
    /**
     * @brief       Factory class for jitter plots
     */
    class JitterPlotFactory :
            public Nedrysoft::RouteAnalyser::IPlotFactory {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::RouteAnalyser::IPlotFactory)

        public:
            /**
             * @brief       Constructs an JitterPlotFactory.
             */
            JitterPlotFactory();

            /**
             * @brief       Constructs the JitterPlotFactory.
             */
            ~JitterPlotFactory();

        public:
            /**
             * @brief       Creates a new jitter plot.
             *
             * @param[in]   margins the margins for rendering the plot.
             *
             * @returns     the plot.
             */
            auto createPlot(const QMargins &margins) -> Nedrysoft::RouteAnalyser::IPlot * override;
    };
}

#endif // NEDRYSOFT_JITTERPLOT_JITTERPLOTFACTORY_H
