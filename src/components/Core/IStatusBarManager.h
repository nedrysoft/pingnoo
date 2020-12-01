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

#ifndef FIZZYADE_CORE_ISTATUSBARMANAGER_H
#define FIZZYADE_CORE_ISTATUSBARMANAGER_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "Core/IContextManager.h"

namespace FizzyAde::Core
{
    /**
     * @brief       Interface definition of a status bar manager
     *
     * @details     The status bar manager handles the management of the application status bar, components may use the status bar to display information
     *              about tasks or results that are running.
     *
     *              The status bar is normally context sensistve although components may created fixed entries that are shown regardless of the current
     *              application context.
     */
    class FIZZYADE_CORE_DLLSPEC IStatusBarManager :
        public FizzyAde::ComponentSystem::IInterface
    {
        Q_OBJECT

    public:
        /**
         * @brief       Gets the FizzyAde::Core::IStatusBarManager instance.
         *
         * @details     A convenience function to return the first FizzyAde::Core::IStatusBarManager instance found in the object registry.
         */
        static IStatusBarManager *getInstance()
        {
            return ComponentSystem::getObject<IStatusBarManager>();
        }

    public:

    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IStatusBarManager, "com.fizzyade.core.IStatusBarManager/1.0.0")

#endif // FIZZYADE_CORE_ISTATUSBARMANAGER_H
