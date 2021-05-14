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

#ifndef PINGNOO_COMPONENTS_CORE_ISTATUSBARMANAGER_H
#define PINGNOO_COMPONENTS_CORE_ISTATUSBARMANAGER_H

#include "CoreSpec.h"
#include "IContextManager.h"

#include <IInterface>

namespace Nedrysoft { namespace Core {
    /**
     * @brief       The IStatusbarManager describes a manager for status bars.
     *
     * @details     The status bar manager handles the management of the application status bar, components may use
     *              the status bar to display information about tasks or results that are running.
     *
     *              The status bar is normally context sensitive although components may created fixed entries that
     *              are shown regardless of the current application context.
     */
    class NEDRYSOFT_CORE_DLLSPEC IStatusbarManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the IStatusbarManager instance.
             *
             * @returns     the IStatusbarManager instance.
             */
            static auto getInstance() -> IStatusbarManager * {
                return ComponentSystem::getObject<IStatusbarManager>();
            }
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IStatusbarManager, "com.nedrysoft.core.IStatusbarManager/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_ISTATUSBARMANAGER_H
