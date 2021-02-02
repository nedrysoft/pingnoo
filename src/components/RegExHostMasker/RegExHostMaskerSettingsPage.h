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

#ifndef NEDRYSOFT_REGEXHOSTMASKERSETTINGSPAGE_H
#define NEDRYSOFT_REGEXHOSTMASKERSETTINGSPAGE_H

#include "Core/IHostMaskerSettingsPage.h"

namespace Nedrysoft::RegExHostMasker {
    /**
     * @brief       The RegExHostMaskerSettingsPage provides the configuration page for the regular expression host
     *              masker.
     */
    class RegExHostMaskerSettingsPage :
        public Nedrysoft::Core::IHostMaskerSettingsPage {
            private:
                Q_OBJECT

                Q_INTERFACES(Nedrysoft::Core::IHostMaskerSettingsPage)

            public:
                /**
                 * @brief       Destroys the RegExHostMaskerSettingsPage.
                 */
                ~RegExHostMaskerSettingsPage() = default;

                /**
                 * @brief       Returns the widget for the settings page.
                 *
                 * @returns     the widget
                 */
                virtual auto widget() -> QWidget *;

                /**
                 * @brief       Returns the display name for the settings page.
                 *
                 * @returns     the displayed name of the settings page.
                 */
                virtual auto displayName() -> QString;
    };
}

#endif //NEDRYSOFT_REGEXHOSTMASKERSETTINGSPAGE_H
