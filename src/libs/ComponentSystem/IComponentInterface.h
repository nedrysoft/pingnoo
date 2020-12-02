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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTINTERFACE_H
#define NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTINTERFACE_H

#include "ComponentSystemSpec.h"
#include <QObject>

#define NedrysoftComponentInterfaceIID  "com.fizzyade.IComponentInterface/1.0"

namespace Nedrysoft::ComponentSystem {
    class ComponentManager;

    /**
     * @brief       IComponentInterface
     *
     * @details     Interface that a component must implement, the plugin system
     *              will call various functions to load and initialise the plugin
     *              at the appropriate time.
     *
     */
    class COMPONENT_SYSTEM_DLLSPEC IComponentInterface {
        public:
            /**
             * @brief Destructor
             *
             */
            virtual ~IComponentInterface();

            /**
             * @brief       initialiseEvent
             *
             * @details     Called by the component loader after all components have been
             *              loaded, called in load order.
             *
             */
            virtual void initialiseEvent();

            /**
             * @brief       initialisationFinishedEvent
             *
             * @details     Called by the component loader after all components have been
             *              initialised, called in reverse load order.
             *
             */
            virtual void initialisationFinishedEvent();
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::ComponentSystem::IComponentInterface, NedrysoftComponentInterfaceIID)

#endif // NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTINTERFACE_H
