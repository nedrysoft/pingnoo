/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 29/12/2020.
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

#ifndef NEDRYSOFT_ROUTEANALYSER_TRIMMERWIDGET_H
#define NEDRYSOFT_ROUTEANALYSER_TRIMMERWIDGET_H

#include <QWidget>

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The TrimmerWidget provides an overview of the dataset along with a viewport for the currently viewed
     *              subset.
     */
    class TrimmerWidget :
            public QWidget {

        private:
            enum class State {
                NotEditing,
                MovingViewport,
                MovingViewportStart,
                MovingViewportEnd
            };

        public:
            /**
             * @brief       Constructs a new TrimmerWidget.
             */
             TrimmerWidget(QWidget *parent=nullptr);

            /**
             * @brief       Destroys the TrimmerWidget.
             */
             ~TrimmerWidget();

        protected:
            /**
             * @brief       Reimplements: QWidget::mousePressEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            auto mousePressEvent(QMouseEvent *event) -> void override;

            /**
             * @brief       Reimplements: QWidget::mouseReleaseEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            auto mouseReleaseEvent(QMouseEvent *event) -> void override;

            /**
             * @brief       Reimplements: QWidget::mouseMoveEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            auto mouseMoveEvent(QMouseEvent *event) -> void override;

            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @param[in]   event the event information.
             */
            auto paintEvent(QPaintEvent *event) -> void override;

            /**
             * @brief       Returns the recommended size for the widget.
             *
             * @returns     the size of the widget.
             */
            QSize sizeHint() const override;

        private:
            State m_editingState;
            double m_viewportPosition;                              //! unit value
            double m_viewportSize;
            double m_origin;
            double m_offset;

    };
};

#endif //NEDRYSOFT_ROUTEANALYSER_TRIMMERWIDGET_H
