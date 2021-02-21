/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/02/2021.
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

#ifndef NEDRYSOFT_PUBLICIPHOSTMASKERSETTINGSPAGEWIDGET_H
#define NEDRYSOFT_PUBLICIPHOSTMASKERSETTINGSPAGEWIDGET_H

#include <QWidget>

namespace Nedrysoft::PublicIPHostMasker {
    namespace Ui {
        class PublicIPHostMaskerSettingsPageWidget;
    }

    /**
     * @brief       The PublicIPHostMaskerSettingsPageWidget class provides the page to configure the public ip
     *              host masker.
     */
    class PublicIPHostMaskerSettingsPageWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new PublicIPHostMaskerSettingsPageWidget which is a child of parent.
             *
             * @param[in]   parent the parent of this child.
             */
            explicit PublicIPHostMaskerSettingsPageWidget(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the PublicIPHostMaskerSettingsPageWidget.
             */
            ~PublicIPHostMaskerSettingsPageWidget() override;

            /**
             * @brief       Called when the data is changed by the user.
             *
             * @note        Under Windows and Linux this function does nothing, on macOS it applies the changes
             *              immediately, this is standard macOS behavior.
             */
            auto updateSettings() -> void;

            /**
             * @brief       Returns whether the current settings can be accepted.
             *
             * @return      true if settings are valid; otherwise false.
             */
            auto canAcceptSettings() -> bool;

            /**
             * @brief       Accepts the current configuration.
             */
            auto acceptSettings() -> void;

        private:
            Ui::PublicIPHostMaskerSettingsPageWidget *ui;
            bool m_loadingConfiguration;
    };
}

#endif //NEDRYSOFT_PUBLICIPHOSTMASKERSETTINGSPAGEWIDGET_H
