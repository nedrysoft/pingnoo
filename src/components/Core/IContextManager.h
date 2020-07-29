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

#ifndef FIZZYADE_CORE_ICONTEXTMANAGER_H
#define FIZZYADE_CORE_ICONTEXTMANAGER_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "ComponentSystem/IComponentManager.h"
#include <QObject>

namespace FizzyAde::Core
{
    using ContextList = QList<int>;

    constexpr int GlobalContext = 0;

    /**
     * Interface definition of an application context manager
     *
     * Manages the context of the application.  A context is a state
     * that the application is in, menu commands are routed appropriately
     * according to the current context.
     *
     */

    class FIZZYADE_CORE_DLLSPEC IContextManager :
        public FizzyAde::ComponentSystem::IInterface
    {
        Q_OBJECT

    public:
        static IContextManager *getInstance()
        {
            return ComponentSystem::getObject<IContextManager>();
        }

        virtual int registerContext(QString contextIdentifier) = 0;
        virtual int setContext(int contextIdentifier) = 0;
        virtual int context() = 0;

        Q_SIGNAL void contextChanged(int newContext, int previousContext);
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IContextManager, "com.fizzyade.core.IContextManager/1.0.0")

#endif // FIZZYADE_CORE_ICONTEXTMANAGER_H
