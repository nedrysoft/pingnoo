/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/03/2020.
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

#ifndef NEDRYSOFT_ROUTEENGINE_ROUTEENGINECOMPONENT_H
#define NEDRYSOFT_ROUTEENGINE_ROUTEENGINECOMPONENT_H

#include "ComponentSystem/IComponent.h"
#include "RouteEngineFactory.h"
#include "RouteEngineSpec.h"

/**
 * @brief       The RouteEngineComponent class provides a IRouteEngine implementation.
 *
 * @details     This component provides a route engine implementation which  discovers the route to a host.
 */
class NEDRYSOFT_ROUTEENGINE_DLLSPEC RouteEngineComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT

        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")

        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        /**
         * @brief       Constructs a new RouteEngineComponent.
         */
        RouteEngineComponent();

        /**
         * @brief       Destroys the RouteEngineComponent.
         */
        ~RouteEngineComponent();

        /**
         * @brief       The initialiseEvent is called by the component loader to initialise the component.
         *
         * @details     Called by the component loader after all components have been loaded, called in load order.
         */
        auto initialiseEvent() -> void override;

        /**
         * @brief       The finaliseEvent function is called by the component system during shutdown.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::finaliseEvent
         */
        auto finaliseEvent() -> void override;

    private:
        Nedrysoft::RouteEngine::RouteEngineFactory *m_routeEngineFactory;

};

#endif // NEDRYSOFT_ROUTEENGINE_ROUTEENGINECOMPONENT_H
