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

#include "ISystemTrayIcon.h"

#include <QIcon>
#include <QPixmap>
#include <QSystemTrayIcon>

class QMenu;

#if defined(Q_OS_MACOS)
namespace Nedrysoft { namespace MacHelper {
    class MacMenubarIcon;
    class MacPopover;
}}
#endif

namespace Nedrysoft { namespace Core {
    /**
     * @brief       The SystemTrayIcon class creates an system tray icon, on macOS this is in the menu bar and on
     *              windows it is in the notification tray area.  Behaviour and availability will differ under Linux.
     */
    class NEDRYSOFT_CORE_DLLSPEC SystemTrayIcon :
            public Nedrysoft::Core::ISystemTrayIcon {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new SystemTrayIcon using the application icon.
             *
             * @param[in]   parent the parent
             */
            SystemTrayIcon(QObject *parent=nullptr);

            /**
             * @brief       Constructs a new SystemTrayIcon.
             *
             * @param[in]   pixmap the pixmap to use.
             * @param[in]   parent the parent
             */
            SystemTrayIcon(const QPixmap &pixmap, QObject *parent=nullptr);

            /**
             * @brief       Destroys the SystemTrayIcon
             */
            ~SystemTrayIcon();

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
            auto setColour(const QColor &newColour) -> void override;

            /**
             * @brief       Returns the geometry for the system tray icon.
             *
             * @returns     the rectangle.
             */
            auto geometry() -> QRect override;

        protected:
            /**
             * @brief       Creates the icon.
             */
            auto createIcon() -> void;

#if defined(Q_OS_MACOS)
            /**
             * @brief       Returns the menubar icon.
             *
             * @returns     the menubar icon.
             */
            auto menubarIcon() -> Nedrysoft::MacHelper::MacMenubarIcon * override;
#endif
        private:
            //! @cond

            QIcon m_icon;
            QPixmap m_basePixmap;
            QPixmap m_activePixmap;
            QMenu *m_contextMenu;
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
