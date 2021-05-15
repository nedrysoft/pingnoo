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

#ifndef PINGNOO_COMPONENTS_ROUTEANALYSER_LATENCYRIBBONGROUP_H
#define PINGNOO_COMPONENTS_ROUTEANALYSER_LATENCYRIBBONGROUP_H

#include <QWidget>

namespace Nedrysoft { namespace Ribbon {
    class RibbonLineEdit;
}}

namespace Nedrysoft { namespace RouteAnalyser {
    namespace Ui {
        class LatencyRibbonGroup;
    }

    class LineSyntaxHighlighter;

    /**
     * @brief       The LatencyRibbonGroup is a Ribbon group that shows and allows modification of the latency values.
     */
    class LatencyRibbonGroup :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       The types of latency we display.
             */
            enum class LatencyType {
                Ideal,
                Warning,
                Critical
            };

        public:
            /**
             * @brief       Constructs a new LatencyRibbonGroup instance which is a child of the parent.
             *
             * @param[in]   parent the parent.
             */
            explicit LatencyRibbonGroup(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the LatencyRibbonGroup.
             */
            ~LatencyRibbonGroup() override;

            /**
             * @brief       Sets the value of the given field.
             *
             * @param[in]   type the type of latency to set.
             * @param[in]   value the latency (in seconds) to change the given type to.
             */
            auto setValue(LatencyType type, double value) -> void;

        public:
            /**
             * @brief       This signal is emitted when the user changes one of the latency line editors.
             *
             * @param[in]   type the type of latency that was edited.
             * @param[in]   value the value that the editor has been changed to.
             */
            Q_SIGNAL void valueChanged(LatencyType type, double value);

        private:
            /**
             * @brief       Updates a period and makes it effective on editors.
             *
             * @param[in]   type the latency period type to update.
             * @param[in]   lineEdit the line edit to update.
             */
            auto updatePeriod(LatencyType type, Nedrysoft::Ribbon::RibbonLineEdit *lineEdit) -> void;

            /**
             * @brief       Returns the given latency value as a formatted string.
             *
             * @param[in]   type the latency value to return.
             *
             * @returns     the value as as formatted string.
             */
            auto getValueString(LatencyType type) -> QString;

        private:
            //! @cond

            Ui::LatencyRibbonGroup *ui;

            LineSyntaxHighlighter *m_warningHighlighter;
            LineSyntaxHighlighter *m_criticalHighlighter;

            //! @endcond
    };
}}

#endif // PINGNOO_COMPONENTS_ROUTEANALYSER_LATENCYRIBBONGROUP_H
