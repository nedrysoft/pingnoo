/*
 * Copyright (C) 2020 Adrian Carpenter
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

#ifndef NEDRYSOFT_COREHOSTMASKERSETTINGSPAGE_H
#define NEDRYSOFT_COREHOSTMASKERSETTINGSPAGE_H

#include "SettingsDialog/ISettingsPage.h"

#include <QIcon>
#include <QString>
#include <QWidget>

namespace Nedrysoft::Core {
    class HostMaskerSettingsPageWidget;

    /**
     * @brief       HostMaskerSettingsPage class provides a settings page for host maskers.
     */
    class HostMaskerSettingsPage :
            public Nedrysoft::SettingsDialog::ISettingsPage {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::SettingsDialog::ISettingsPage)

        public:
            /**
             * @brief       Constructs a new HostMaskerSettingsPage instance which is a child of the parent.
             *
             * @param[in]   parent the parent of this widget.
             */
            explicit HostMaskerSettingsPage(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the HostMaskerSettingsPage.
             */
            ~HostMaskerSettingsPage() override;

        public:
            /**
             * @brief       The section name that this settings page should appear in, first level grouping.
             *
             * @returns     a string containing the name.
             */
            auto section() -> QString override;

            /**
             * @brief       The category name that this settings page should appear in, second level grouping.
             *
             * @returns     a string containing the name.
             */
            auto category() -> QString override;

            /**
             * @brief       The descriptive label for this settings page.
             *
             * @returns     a string containing the name.
             */
            auto description() -> QString override;

            /**
             * @brief       The icon for this settings page.
             *
             * @param[in[   isDarkMode set to true to retries the dark mode icon; otherwise false.
             *
             * @returns     a QIcon
             */
            auto icon(bool isDarkMode) -> QIcon override;

            /**
             * @brief       Creates a new instance of the page widget.
             *
             * @returns     the new widget instance.
             */
            auto createWidget() -> QWidget * override;

            /**
             * @brief       Checks if the settings can be applied.
             *
             * @returns     true if the settings can be applied (i.e valid); otherwise false.
             */
            auto canAcceptSettings() -> bool override;

            /**
             * @brief       Applies the current settings.
             */
            auto acceptSettings() -> void override;

        private:
            HostMaskerSettingsPageWidget *m_settingsWidget;
    };
}

#endif //NEDRYSOFT_COREHOSTMASKERSETTINGSPAGE_H
