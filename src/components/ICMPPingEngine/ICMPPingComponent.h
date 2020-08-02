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

#ifndef FIZZYADE_ICMPPINGENGINE_ICMPPINGCOMPONENT_H
#define FIZZYADE_ICMPPINGENGINE_ICMPPINGCOMPONENT_H

#include "ICMPPingEngineSpec.h"
#include "ComponentSystem/IComponentInterface.h"

namespace FizzyAde::ICMPPingEngine
{
    class ICMPPingEngineFactory;
}

/**
 * @brief       ICMPPingComponent class
 *
 * @details     Implements an ICMP ping (for all platforms)
 *
 */
class FIZZYADE_ICMPPINGENGINE_DLLSPEC ICMPPingComponent :
    public QObject,
    public FizzyAde::ComponentSystem::IComponentInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID FizzyAdeComponentInterfaceIID FILE "metadata.json")
    Q_INTERFACES(FizzyAde::ComponentSystem::IComponentInterface)

public:
    /**
     * @brief       Constructor
     *
     */
    ICMPPingComponent();

    /**
     * @brief       Destructor
     *
     */
    ~ICMPPingComponent();

    /**
     * @sa          FizzyAde::ComponentSystem::IComponentInterface
     *
     */
    virtual void initialiseEvent();

private:
    FizzyAde::ICMPPingEngine::ICMPPingEngineFactory *m_engineFactory;
};

#endif // FIZZYADE_ICMPPINGENGINE_ICMPPINGCOMPONENT_H
