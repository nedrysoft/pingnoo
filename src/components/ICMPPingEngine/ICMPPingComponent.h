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

#ifndef NEDRYSOFT_ICMPPINGENGINE_ICMPPINGCOMPONENT_H
#define NEDRYSOFT_ICMPPINGENGINE_ICMPPINGCOMPONENT_H

#include "ComponentSystem/IComponent.h"
#include "ICMPPingEngineSpec.h"

namespace Nedrysoft::ICMPPingEngine {
    class ICMPPingEngineFactory;
}

/**
 * @brief       The ICMPPingComponent class provides a socket based ICMP ping engine for all platforms.
 */
class NEDRYSOFT_ICMPPINGENGINE_DLLSPEC ICMPPingComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT

        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")

        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        /**
         * @brief       Constructs the ICMPPingComponent.
         */
        ICMPPingComponent();

        /**
         * @brief       Destroys the ICMPPingComponent.
         */
        ~ICMPPingComponent();

    public:
        /**
         * @brief       The initialiseEvent is called by the component loader to intialise the ocmponent.
         *
         * @details     Called by the component loader after all components have been loaded, called in load order.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::initialiseEvent
         */
        virtual void initialiseEvent();

    private:
        Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory *m_engineFactory;
};

#endif // NEDRYSOFT_ICMPPINGENGINE_ICMPPINGCOMPONENT_H
