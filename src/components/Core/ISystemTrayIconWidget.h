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

#ifndef PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICONWIDGET_H
#define PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICONWIDGET_H

#include "CoreSpec.h"

#include <IInterface>

namespace Nedrysoft { namespace Core {
    /**
     * @brief       The ISystemTrayIconWidget is an item that is displayed the in the popup window of the
     *              operating systems system tray (on macOS this is a menu bar item)
     */
    class NEDRYSOFT_CORE_DLLSPEC ISystemTrayIconWidget :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the widget for this item.
             *
             * @returns     the widget.
             */
            //virtual auto widget() -> QWidget * = nullptr;
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ISystemTrayIconWidget, "com.nedrysoft.core.ISystemTrayIconWidget/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_ISYSTEMTRAYICONWIDGET_H
