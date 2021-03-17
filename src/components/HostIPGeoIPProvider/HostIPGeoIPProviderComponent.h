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

#ifndef NEDRYSOFT_HOSTIPGEOIPPROVIDER_HOSTIPGEOIPPROVIDERCOMPONENT_H
#define NEDRYSOFT_HOSTIPGEOIPPROVIDER_HOSTIPGEOIPPROVIDERCOMPONENT_H

#include "ComponentSystem/IComponent.h"
#include "Core/ILogger.h"
#include "HostIPGeoIPProviderSpec.h"

namespace Nedrysoft::HostIPGeoIPProvider {
    class HostIPGeoIPProvider;
}

/**
 * @brief       The HostIPGeoIPProviderComponent class provides a hostip.com geo location lookup.
 */
class NEDRYSOFT_HOSTIPGEOIPPROVIDER_DLLSPEC HostIPGeoIPProviderComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT

        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")

        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        /**
         * @brief       Constructs a HostIPGeoIPProviderComponent.
         */
        HostIPGeoIPProviderComponent();

        /**
         * @brief       Destroys the HostIPGeoIPProviderComponent.
         *
         */
        ~HostIPGeoIPProviderComponent();

        /**
         * @brief       The initialiseEvent is called by the component loader to initialise the component.
         *
         * @details     Called by the component loader after all components have been loaded, called in load order.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::initialiseEvent
         */
        auto initialiseEvent() -> void override;

        /**
         * @brief       The finaliseEvent method is called before the component is unloaded.
         *
         * @notes       The event is called in reverse load order for all loaded components, once every component
         *              has been finalised the component manager then unloads all components in thr same order.
         */
        auto finaliseEvent() -> void override;

    private:

        Nedrysoft::HostIPGeoIPProvider::HostIPGeoIPProvider *m_provider;
};

#endif // NEDRYSOFT_HOSTIPGEOIPPROVIDER_HOSTIPGEOIPPROVIDERCOMPONENT_H
