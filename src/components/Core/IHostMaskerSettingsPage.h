/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/02/2021.
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

#ifndef NEDRYSOFT_IHOSTMASKERSETTINGSPAGE_H
#define NEDRYSOFT_IHOSTMASKERSETTINGSPAGE_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QWidget>

namespace Nedrysoft::Core {
    /**
     * @brief       The ISettingsPage interface defines a settings page.
     */
    class NEDRYSOFT_CORE_DLLSPEC IHostMaskerSettingsPage :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       Creates a new instance of the page widget.
             *
             * @returns     the new widget instance.
             */
            virtual auto createWidget() -> QWidget * = 0;

            /**
             * @brief       Returns the display name for the host masker.
             *
             * @returns     the displayed name of the host masker.
             */
            virtual auto displayName() -> QString = 0;

            /**
             * @brief       Returns whether the settings can be accepted.
             */
            virtual auto canAcceptSettings() -> bool = 0;

            /**
             * @brief       Accepts the current settings.
             */
            virtual auto acceptSettings() -> void = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IHostMaskerSettingsPage, "com.nedrysoft.core.IHostMaskerSettingsPage/1.0.0")

#endif //NEDRYSOFT_IHOSTMASKERSETTINGSPAGE_H
