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
     * IStatusBarManager interface
     *
     * IStatusBarManager handles the management of status bars
     *
     */

    class FIZZYADE_CORE_DLLSPEC IStatusBarManager :
        public FizzyAde::ComponentSystem::IInterface
    {
        Q_OBJECT

    public:
        /**
         * Returns the IStatusBarManager instance
         *
         * @return the IStatusBarManager instance
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
