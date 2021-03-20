/*
 * Copyright (C) 2021 Adrian Carpenter
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

#ifndef NEDRYSOFT_ROUTEANALYSER_IPLOT_H
#define NEDRYSOFT_ROUTEANALYSER_IPLOT_H

#include "ComponentSystem/IInterface.h"
#include "../RouteAnalyserSpec.h"

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The IPlot interface provides additional plots to the main route analyser.
     */
    class NEDRYSOFT_ROUTEANALYSER_DLLSPEC IPlot :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       Returns the widget for this plot.
             *
             * @returns     the widget.
             */
            virtual auto widget() -> QWidget * = 0;

            /**
             * @brief       Updates the plot with a new result.
             * @param[in]   time the unix timestamp for this result.
             * @param[in]   value the round trip time.
             */
            virtual auto update(double time, double value) -> void = 0;

            /**
             * @brief       Update the visible area (viewport) of the graph.
             * @param[in]   min the minimum displayed value.
             * @param[in]   max the maximum dispkayed value.
             */
            virtual auto updateRange(double min, double max) -> void = 0;

            /**
             * @brief       Sets the jitter range.
             *
             * @notes       The target value is drawn as a dash on the jitter graph, the maximum displayed
             *              value is set by max.
             *
             * @param[in]   targetJitter the target value in seconds.
             * @param[in]   maximumJitter the maximum value to show on the jitter graph.
             */
            virtual auto setRange(double targetJitter, double maximumJitter) -> void = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::RouteAnalyser::IPlot, "com.nedrysoft.routeanalyser.IPlot/1.0.0")

#endif //NEDRYSOFT_ROUTEANALYSER_IPLOT_H
