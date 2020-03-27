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

#ifndef FIZZYADE_CORE_IMENU_H
#define FIZZYADE_CORE_IMENU_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include <QMenu>
#include <QMenuBar>

namespace FizzyAde::Core
{
    class ICommand;

    enum MenuTypes {
        isMenuBar,
        isMenu,
        isSubMenu
    };

    class FIZZYADE_CORE_DLLSPEC IMenu :
        public FizzyAde::ComponentSystem::IInterface
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::ComponentSystem::IInterface)

    public:
        virtual QMenu *menu() = 0;
        virtual QMenuBar *menuBar() = 0;

        virtual void addCommand(FizzyAde::Core::ICommand *command) = 0;

    public:

        virtual FizzyAde::Core::MenuTypes type() = 0;
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IMenu, "com.fizzyade.core.IMenu/1.0.0")

#endif // FIZZYADE_CORE_IMENU_H
