/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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

#ifndef NEDRYSOFT_LATENCYSETTINGSPAGEWIDGET_H
#define NEDRYSOFT_LATENCYSETTINGSPAGEWIDGET_H

#include <QIcon>
#include <QString>
#include <QWidget>

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class LatencySettingsPageWidget;
    }

    /**
     * @brief       The LatencySettingsPageWidget provides a settings page for modifying latency defaults.
     */
    class LatencySettingsPageWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs the LatencySettingsPageWidget settings page.
             *
             * @param[in]   parent the parent of this child.
             */
            explicit LatencySettingsPageWidget(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the LatencySettingsPageWidget.
             */
            ~LatencySettingsPageWidget() override;

            /**
             * @brief       Checks if the settings can be applied.
             *
             * @returns     true if the settings can be applied (i.e valid); otherwise false.
             */
            auto canAcceptSettings() -> bool;

            /**
             * @brief       Applies the current settings.
             */
            auto acceptSettings() -> void;

        private:
            Ui::LatencySettingsPageWidget *ui;

            QList<QMetaObject::Connection> m_connections;
    };
}

#endif //NEDRYSOFT_LATENCYSETTINGSPAGEWIDGET_H
