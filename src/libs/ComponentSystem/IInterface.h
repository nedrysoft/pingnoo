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

#ifndef FIZZYADE_COMPONENTSYSTEM_IINTERFACE_H
#define FIZZYADE_COMPONENTSYSTEM_IINTERFACE_H

#include "ComponentSystemSpec.h"
#include <QObject>

namespace FizzyAde::ComponentSystem
{
    /**
     * Root IInterface definition
     */

    class COMPONENT_SYSTEM_DLLSPEC IInterface :
        public QObject
    {
        Q_OBJECT

    public:
        virtual ~IInterface() {}

    };
}

Q_DECLARE_INTERFACE(FizzyAde::ComponentSystem::IInterface, "com.fizzyade.IInterface/1.0.0")

#endif // FIZZYADE_COMPONENTSYSTEM_IINTERFACE_H
