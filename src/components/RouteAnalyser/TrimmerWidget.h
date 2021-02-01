/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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
#include <QFlags>

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The TrimmerWidget provides an overview of the dataset along with a viewport for the currently viewed
     *              subset.
     */
    class TrimmerWidget :
            public QWidget {

        private:
            Q_OBJECT

        private:
            enum class State {
                NotEditing,
                MovingViewport,
                MovingViewportStart,
                MovingViewportEnd
            };

        public:
            enum TrimmerFlag {
                FixedStart = 0x1,
                FixedEnd = 0x2,
            };
            Q_DECLARE_FLAGS(TrimmerFlags, TrimmerFlag)

        public:
            /**
             * @brief       Constructs a new TrimmerWidget.
             */
            TrimmerWidget(QWidget *parent=nullptr);

            /**
             * @brief       Destroys the TrimmerWidget.
             */
            ~TrimmerWidget();

            /**
             * @brief      Sets the start and end points of the trimmer viewport..
             *
             * @param[in]  start the start position as a unit value.
             * @param[in]  end the end position as a unit value.
             */
            auto setViewport(double start, double end) -> void;

            /**
             * @brief     Returns the start position of the viewport..
             *
             * @returns   A unit value of the start position.
             */
            auto viewportStart() -> double;

            /**
             * @brief     Returns the start position of the viewport..
             *
             * @returns   A unit value of the start position.
             */
            auto viewportEnd() -> double;

            /**
             * @brief       Returns the size of the viewport.
             *
             * @returns     A unit value of the size of the viewport.
             */
            auto viewportSize() -> double;

            /**
             * @brief       This signal is emitted when the viewport is moved or resized.
             *
             * @param[in]   start the start value of the viewport.
             * @param[in]   end the end value of the viewport.
             */
            Q_SIGNAL void positionChanged(double start, double end);

            /**
             * @brief       Sets the flags for the viewport.
             *
             * @param[in] flags the flags.
             */
            auto setFlags(TrimmerFlags flags) -> void;

            /**
             * @brief       Sets whether the trimmer viewport can be resized.
             *
             * @param[in]   canBeResized true if viewport can be resized; otherwise false.
             */
            auto setResizable(bool canBeResized) -> void;

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
            double m_viewportPosition;                              //! the start position of the viewport (unit value)
            double m_viewportSize;                                  //! the end position of the viewport (unit value)
            double m_origin;                                        //! absolute value for movement, allows a delta to be computed
            double m_viewportEnd;                                   //! the viewpoint end position at the start of a movement
            TrimmerFlags m_flags;
            bool m_canBeResized;                                    //! whether the viewport can be resized.
    };
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Nedrysoft::RouteAnalyser::TrimmerWidget::TrimmerFlags)

#endif //NEDRYSOFT_ROUTEANALYSER_TRIMMERWIDGET_H
