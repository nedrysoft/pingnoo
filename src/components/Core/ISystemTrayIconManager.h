/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/05/2021.
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

#include <IInterface>

namespace Nedrysoft { namespace Core {
    /**
     * @brief       The ISystemTrayIconManager provides access to the system tray icon that some operations
     *              systems provide.
     *
     * @class       Nedrysoft::Core::ISystemTrayIconManager ISystemTrayIconManager.h <ISystemTrayIconManager>
     */
    class NEDRYSOFT_CORE_DLLSPEC ISystemTrayIconManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Sets the colour of the system tray icon.
             *
             * @note        The operating system may not honour the colour due to the actual window manager
             *              and theming.  Each pixel is mixed with the selected colour, setting the alpha
             *              will set the transparency of the resulting image.
             *
             * @param[in]   newColour the desired colour.
             */
            virtual auto setColour(const QColor &newColour) -> void = 0;
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ISystemTrayIconManager, "com.nedrysoft.core.ISystemTrayIconManager/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICONMANAGER_H
