/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 14/06/2021.
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

#ifndef PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTEDISCOVERYWIDGET_H
#define PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTEDISCOVERYWIDGET_H

#include <QWidget>

namespace Nedrysoft { namespace RouteAnalyser {
    namespace Ui {
        class RouteDiscoveryWidget;
    }

    /**
     * @brief       The RouteDiscoveryWidget shows feedback to the user as a route is being discovered.
     */
    class RouteDiscoveryWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new RouteDiscoveryWidget
             *
             * @param[in]   parent the parent of this child widget.
             */
            explicit RouteDiscoveryWidget(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the RouteDiscoveryWidget.
             */
            ~RouteDiscoveryWidget() override;

            /**
             * @brief       Sets the target name to be displayed.
             *
             * @param[in]   target the target name.
             */
            auto setTarget(QString target) -> void;

            /**
             * @brief       Sets the current progress of the discovery.
             *
             * @note        If miimum and maximum are set the to same value, then the progress bar is displayed
             *              using an non-deterministic style.
             *
             * @param[in]   currentHop the current hop number.
             * @param[ib]   totalHops the total number of hops (if known).
             * @param[in]   maximumTTL the maximum TTL begin used to limit the discovery process.
             */
            auto setProgress(int currentHop, int totalHops, int maximumTTL) -> void;

        private:
            //! @cond

            Ui::RouteDiscoveryWidget *ui;

            //! @endcond
    };
}}

#endif // PINGNOO_COMPONENTS_ROUTEANALYSER_ROUTEDISCOVERYWIDGET_H
