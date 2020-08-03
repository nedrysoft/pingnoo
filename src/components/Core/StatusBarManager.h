/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
 * An open source ping path analyser
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

#ifndef FIZZYADE_CORE_STATUSBARMANAGER_H
#define FIZZYADE_CORE_STATUSBARMANAGER_H

#include "IStatusBarManager.h"

namespace FizzyAde::Core
{
    /**
     * @brief       Implementation of IStatusBarManager
     *
     * @details     Manages a status bar instance in the application, components
     *              can use the status bar to display relevant information in any contexts
     *              that they use.
     *
     */
    class StatusBarManager :
        public IStatusBarManager
    {
    public:
        /**
         * @brief       Constructor
         *
         */
        StatusBarManager();
    };
}

#endif // FIZZYADE_CORE_STATUSBARMANAGER_H
