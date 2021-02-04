/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 01/02/2021.
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

#ifndef NEDRYSOFT_COREHOSTMASKERSETTINGSPAGEWIDGET_H
#define NEDRYSOFT_COREHOSTMASKERSETTINGSPAGEWIDGET_H

#include <QWidget>

namespace Nedrysoft::Core {
    namespace Ui {
        class HostMaskerSettingsPageWidget;
    }

    /**
     * @brief       The HostMaskerSettingsPageWidget class provides the widget for host maskers.
     *
     */
    class HostMaskerSettingsPageWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new HostMaskerSettingsPageWidget which is a child of the parent.
             *
             * @param[in]   parent the parent of this widget.
             */
            explicit HostMaskerSettingsPageWidget(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the RegExHostMaskerSettingsPageWidget.
             */
            ~HostMaskerSettingsPageWidget() override;

        public:
            /**
             * @brief       Initialises the host masker widget, searches for HostMasker settings pages and updates
             *              the list.
             */
            auto initialise() -> void;

        private:
            Ui::HostMaskerSettingsPageWidget *ui;
    };
}

#endif //NEDRYSOFT_COREHOSTMASKERSETTINGSPAGEWIDGET_H
