/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 13/12/2020.
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

#ifndef NEDRYSOFT_ROUTEANALYSER_VIEWPORTRIBBONGROUP_H
#define NEDRYSOFT_ROUTEANALYSER_VIEWPORTRIBBONGROUP_H

#include <QWidget>

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class ViewportRibbonGroup;
    }

    /**
     * @brief       The ViewportRibbonGroup is a Ribbon group allows the viewport to be set.
     */
    class ViewportRibbonGroup :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new ViewportRibbonGroup instance which is a child of the parent.
             *
             * @param[in]   parent the parent.
             */
            explicit ViewportRibbonGroup(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the ViewportRibbonGroup.
             */
            ~ViewportRibbonGroup() override;

            /**
             * @brief       Sets the viewport start and end
             * @param[in]   start the start.
             * @paramp[in]  end the end.
             */
            auto setViewport(double start, double end) -> void;

            /**
             * @brief       Sets whether the viewport trimmer widget is enabled (i.e editable)
             * @param[in]   enabled true if enabled; otherwise false.
             */
            auto setViewportEnabled(bool enabled) -> void;

            /**
             * @brief       Returns whether the trimmer widget is enabled.
             * @param[in]   enabled true if enabled; otherwise false.
             */
            auto isViewportEnabled() -> bool;

            /**
             * @brief       Sets the start and end text boxes on the ribbon control.
             *
             * @param[in]   start the start unix time.
             * @param[in]   end the end unix time.
             */
            auto setStartAndEnd(double start, double end) -> void;

            /**
             * @brief       Returns the size of the viewport in seconds.
             *
             * @returns     the viewport size.
             */
            auto viewportSize() -> double;

        public:
            /**
             * @brief       This signal is emitted when the viewport start and/or end has been modified.
             * @param[in]   start the new start point. (unit value)
             * @param[in]   end the new end point. (unit value)
             */
            Q_SIGNAL void viewportChanged(double start, double end);

            /**
             * @brief       This signal is emitted when the viewport size has changed.
             * @param[in]   size the new size of the viewport.
             */
            Q_SIGNAL void viewportWindowChanged(double size);

        private:
            Ui::ViewportRibbonGroup *ui;
    };
}

#endif //NEDRYSOFT_ROUTEANALYSER_VIEWPORTRIBBONGROUP_H
