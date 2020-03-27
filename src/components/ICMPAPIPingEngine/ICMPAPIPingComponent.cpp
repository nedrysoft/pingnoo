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

#include "ICMPAPIPingComponent.h"
#include "ComponentSystem/IComponentManager.h"
#include "ICMPAPIPingEngineFactory.h"
#include <WS2tcpip.h>
#include <IPExport.h>
#include <QDebug>

ICMPAPIPingComponent::ICMPAPIPingComponent() = default;

ICMPAPIPingComponent::~ICMPAPIPingComponent() = default;

void ICMPAPIPingComponent::initialiseEvent()
{
#if defined(Q_OS_WIN)
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2,2), &wsaData)!=0)
    {
        qDebug() << "error initialising winsock";
    }
#endif
    //FizzyAde::ComponentSystem::addObject(new FizzyAde::Pingnoo::ICMPAPIPingEngineFactory());
}


