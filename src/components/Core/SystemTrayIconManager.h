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

#ifndef NEDRYSOFT_SYSTEMTRAYICONMANAGER_H
#define NEDRYSOFT_SYSTEMTRAYICONMANAGER_H

#include <QtGlobal>

#include "ISystemTrayIconManager.h"

#include <QIcon>
#include <QPixmap>
#include <QSystemTrayIcon>

#if defined(Q_OS_MACOS)
namespace Nedrysoft { namespace MacHelper {
    class MacMenubarIcon;
    class MacPopover;
}}
#endif

namespace Nedrysoft { namespace Core {
    /**
     * @brief       The ISystemTrayIconManager provides access to the system tray icon that some operations
     *              systems provide.
     */
    class NEDRYSOFT_CORE_DLLSPEC SystemTrayIconManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new SystemTrayIconManager.
             *
             * @param[in]   parent the parent
             */
            SystemTrayIconManager(QObject *parent=nullptr);

            /**
             * @brief       Destroys the SystemTrayIconManager
             */
            ~SystemTrayIconManager();

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
            auto setIconColour(const QColor &newColour) -> void;

        private:
            //! @cond

            QIcon m_icon;
            QPixmap m_basePixmap;
            QPixmap m_activePixmap;
#if defined(Q_OS_MACOS)
            Nedrysoft::MacHelper::MacMenubarIcon *m_menubarIcon;
            Nedrysoft::MacHelper::MacPopover *m_popover;
#else
            QSystemTrayIcon *m_systemTrayIcon;
#endif
            bool m_visible;

            //! @endcond
    };
}}


#endif //NEDRYSOFT_SYSTEMTRAYICONMANAGER_H
