/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 20/12/2020.
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
#ifndef NEDRYSOFT_LATENCYSETTINGSPAGE_H
#define NEDRYSOFT_LATENCYSETTINGSPAGE_H

#include <QIcon>
#include <QString>
#include <QWidget>
#include <SettingsDialog/ISettingsPage.h>

namespace Nedrysoft::RouteAnalyser {
    class LatencySettingsPage :
            public Nedrysoft::SettingsDialog::ISettingsPage {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::SettingsDialog::ISettingsPage)

        public:
            explicit LatencySettingsPage(QWidget *parent = nullptr);

            ~LatencySettingsPage() override;

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
             * @returns     a QIcon
             */
            auto icon() -> QIcon override;

            /**
             * @brief       The widget for the page.
             *
             * @returns     a widget.
             */
            auto widget() -> QWidget * override;

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
    };
}

#endif //NEDRYSOFT_LATENCYSETTINGSPAGE_H
