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

#include "ICMPAPIPingComponent.h"

#include "ComponentSystem/IComponentManager.h"
#include "ICMPAPIPingEngineFactory.h"

#define _WINSOCKAPI_    // stops windows.h including winsock.h

#include <winsock2.h>
#include <windows.h>
//#include <IPExport.h>
//#include <WS2tcpip.h>

#include <ws2tcpip.h>
#include <spdlog/spdlog.h>

ICMPAPIPingComponent::ICMPAPIPingComponent() = default;

ICMPAPIPingComponent::~ICMPAPIPingComponent() = default;

auto ICMPAPIPingComponent::initialiseEvent() -> void {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2,2), &wsaData)!=0) {
        spdlog::warn("error initialising winsock");
    }

    Nedrysoft::ComponentSystem::addObject(new Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactory());
}


