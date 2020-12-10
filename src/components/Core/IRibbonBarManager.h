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

#ifndef NEDRYSOFT_CORE_IRIBBONBARMANAGER_H
#define NEDRYSOFT_CORE_IRIBBONBARMANAGER_H

#include "ComponentSystem/IInterface.h"
#include "Core/IContextManager.h"
#include "CoreSpec.h"

namespace Nedrysoft::Core {
    /**
     * @brief       IRibbonBarManager interface
     *
     * @details     IRibbonBarManager handles the management of status bars
     */
    class NEDRYSOFT_CORE_DLLSPEC IRibbonBarManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the IRibbonBarManager instance
             *
             * @return      the IRibbonBarManager singleton instance
             */
            static IRibbonBarManager *getInstance() {
                return ComponentSystem::getObject<IRibbonBarManager>();
            }

        public:

    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IRibbonBarManager, "com.nedrysoft.core.IRibbonBarManager/1.0.0")

#endif // NEDRYSOFT_CORE_IRIBBONBARMANAGER_H
