/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 24/05/2021.
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

#ifndef PINGNOO_COMPONENTS_CORE_SYSTEMTRAYICONMANAGER_H
#define PINGNOO_COMPONENTS_CORE_SYSTEMTRAYICONMANAGER_H

#include "ISystemTrayIconManager.h"

#include <QMap>
#include <RibbonWidget>

namespace Nedrysoft { namespace Core {

    /**
     * @brief       The SustemTrayIconManager implements ISystemTrayIconManager.
     */
    class SystemTrayIconManager :
            public Nedrysoft::Core::ISystemTrayIconManager {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::ISystemTrayIconManager)

        public:
            /**
             * @brief       Constructs a SustemTrayIconManager.
             */
            SystemTrayIconManager() = default;

            /**
             * @brief       Destroys the RibbonBarManager.
             */
            ~SystemTrayIconManager() = default;

            /**
             * @brief       Creates a new system tray icon
             *
             * @param[in]   pixmap the pixmap to be used.
             *
             * @returns     the system tray icon instance.
             */
            auto createIcon(const QPixmap &pixmap = QPixmap()) -> Nedrysoft::Core::ISystemTrayIcon * override;

            /**
             * @brief       Creates a new system tray icon using the application icon.
             *
             * @returns     the system tray icon instance.
             */
            auto createIcon() -> Nedrysoft::Core::ISystemTrayIcon * override;
    };
}}

#endif // PINGNOO_COMPONENTS_CORE_SYSTEMTRAYICONMANAGER_H
