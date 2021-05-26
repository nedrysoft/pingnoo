/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 03/05/2021.
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

#ifndef PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICON_H
#define PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICON_H

#include "CoreSpec.h"

#include <IInterface>

namespace Nedrysoft { namespace MacHelper {
    class MacMenubarIcon;
}}

namespace Nedrysoft { namespace Core {
    /**
     * @brief       The ISystemTrayIcon represents a single system tray icon, the application (depending on OS
     *              support may have more than 1 icon.
     *
     * @class       Nedrysoft::Core::ISystemTrayIcon ISystemTrayIcon.h <ISystemTrayIcon>
     */
    class NEDRYSOFT_CORE_DLLSPEC ISystemTrayIcon :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            enum class MouseButton {
                Left,
                Right
            };

        public:
            /**
             * @brief       Sets whether the system tray icon is visible.
             *
             * @param[in]   visible if true the icon is shown; otherwise false.
             */
            auto setVisible(bool visible) -> void;

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

            /**
             * @brief       Returns the geometry for the system tray icon.
             *
             * @returns     the rectangle.
             */
            virtual auto geometry() -> QRect = 0;

#if defined(Q_OS_MACOS)
            /**
             * @brief       Returns the menu bar icon.
             *
             * @returns     the menu bar icon.
             */
            virtual auto menubarIcon() -> Nedrysoft::MacHelper::MacMenubarIcon * = 0;
#endif

            /**
             * @brief       This signal is emitted when the user clicks on the system tray icon.
             *
             * @param[in]   button the mouse button that caused the signal.
             */
            Q_SIGNAL void clicked(const Nedrysoft::Core::ISystemTrayIcon::MouseButton &button);
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ISystemTrayIcon, "com.nedrysoft.core.ISystemTrayIcon/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICON_H
