/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 07/01/2021.
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

#ifndef NEDRYSOFT_PLOTSCROLLAREA_H
#define NEDRYSOFT_PLOTSCROLLAREA_H

#include <QScrollArea>

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The PlotScrollArea class is a thin wrapper around QScrollArea.  It's sole purpose is o emit the
     *              didScroll() signal when the view is scrolled, this allows the editor widget to re-plot graphs
     *              that were out of view but are now in view.  By doing this, the load when plotting the current
     *              graphics is reduced as only those that are partially or fully in view will be plotted.
     */
    class PlotScrollArea :
            public QScrollArea {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       This signal is emitted when the scroll area is scrolled.
             */
            Q_SIGNAL void didScroll();

        protected:
            /**
             * @brief       Reimplements QScrollArea::scrollContentsBy(int dx, int dy)
             *
             * @param[in]   dx the distance that the view was scrolled by in x.
             * @param[in]   dy the distance that the view was scrolled by in y.
             */
            auto scrollContentsBy(int dx, int dy) -> void override;
    };
}

#endif //NEDRYSOFT_PLOTSCROLLAREA_H
