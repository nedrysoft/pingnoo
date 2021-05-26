/*
 * Copyright (C) 2021 Adrian Carpenter
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

#ifndef PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICONMANAGER_H
#define PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICONMANAGER_H

#include "CoreSpec.h"

#include <IComponentManager>
#include <IInterface>
#include <QPixmap>

namespace Nedrysoft { namespace Core {
    class ISystemTrayIcon;

    /**
     * @brief       The ISystemTrayIconManager class is used to manage the lifecycle of system tray icons.
     *
     * @class       Nedrysoft::Core::ISystemTrayIconManager ISystemTrayIconManager.h <ISystemTrayIconManager>
     */
    class NEDRYSOFT_CORE_DLLSPEC ISystemTrayIconManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the ISystemTrayIconManager instance.
             *
             * @returns     the ISystemTrayIconManager instance.
             */
            static auto getInstance() -> ISystemTrayIconManager * {
                return ComponentSystem::getObject<ISystemTrayIconManager>();
            }

            /**
             * @brief       Creates a new system tray icon
             *
             * @param[in]   pixmap the pixmap to be used, if not supplied the application icon is used.
             *
             * @returns     the system tray icon instance.
             */
            virtual auto createIcon(const QPixmap &pixmap) -> Nedrysoft::Core::ISystemTrayIcon * = 0;

             /**
             * @brief       Creates a new system tray icon using the application icon.
             *
             * @returns     the system tray icon instance.
             */
            virtual auto createIcon() -> Nedrysoft::Core::ISystemTrayIcon * = 0;
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ISystemTrayIconManager, "com.nedrysoft.core.ISystemTrayIconManager/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICONMANAGER_H
