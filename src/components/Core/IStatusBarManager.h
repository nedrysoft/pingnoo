/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/07/2020.
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

#ifndef NEDRYSOFT_CORE_ISTATUSBARMANAGER_H
#define NEDRYSOFT_CORE_ISTATUSBARMANAGER_H

#include "ComponentSystem/IInterface.h"
#include "Core/IContextManager.h"
#include "CoreSpec.h"

namespace Nedrysoft::Core {
    /**
     * @brief       The IStatusBarManager describes a manager for status bars.
     *
     * @details     The status bar manager handles the management of the application status bar, components may use
     *              the status bar to display information about tasks or results that are running.
     *
     *              The status bar is normally context sensitive although components may created fixed entries that
     *              are shown regardless of the current application context.
     */
    class NEDRYSOFT_CORE_DLLSPEC IStatusBarManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the IStatusBarManager instance.
             *
             * @returns     the IStatusBarManager instance.
             */
            static auto getInstance() -> IStatusBarManager * {
                return ComponentSystem::getObject<IStatusBarManager>();
            }
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IStatusBarManager, "com.nedrysoft.core.IStatusBarManager/1.0.0")

#endif // NEDRYSOFT_CORE_ISTATUSBARMANAGER_H
