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

#ifndef FIZZYADE_CORE_ICOMMANDMANAGER_H
#define FIZZYADE_CORE_ICOMMANDMANAGER_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "ComponentSystem/IComponentManager.h"
#include "Core/IContextManager.h"
#include "Core/IMenu.h"
#include "ICommand.h"
#include <QObject>
#include <QAction>
#include <QList>

namespace FizzyAde::Core
{
    /**
     * ICommandManager interface
     *
     * ICommandManager manages instances of ICommand, handling context
     * changes.
     *
     */

    class FIZZYADE_CORE_DLLSPEC ICommandManager :
        public FizzyAde::ComponentSystem::IInterface
    {
        Q_OBJECT

    public:
        /**
         * Returns the ICommandManager instance
         *
         * @return the ICommandManager instance
         */
        static ICommandManager *getInstance()
        {
            return ComponentSystem::getObject<ICommandManager>();
        }

        /**
         * Registers an action to the command
         *
         * @param[in] action the action
         * @param[in] id the identifier of the command
         * @param[in] contexts the contexts this action is valid in
         * @return the configuration
         */
        virtual FizzyAde::Core::ICommand *registerAction(QAction *action, QString id, const FizzyAde::Core::ContextList &contexts) = 0;

        /**
         * Registers an action to the command
         *
         * @param[in] action the action
         * @param[in] id the identifier of the command
         * @param[in] context the contexts this action is valid in
         * @return the configuration
         */
        virtual FizzyAde::Core::ICommand *registerAction(QAction *action, QString id, int contextId = FizzyAde::Core::GlobalContext)
        {
            return registerAction(action, id, FizzyAde::Core::ContextList() << contextId);
        }

        /**
         * Sets the currently active context
         *
         * Updates all commands registered so that they connect to the
         * correct QAction for the context
         *
         * @param[in] contextId the context
         */
        virtual void setContext(int contextId) = 0;

        virtual FizzyAde::Core::IMenu *createMenu(const QString &identifier, IMenu *parentMenu=nullptr) = 0;
        virtual FizzyAde::Core::IMenu *findMenu(const QString &identifier) = 0;
        virtual FizzyAde::Core::ICommand *findCommand(const QString &identifier) = 0;
    public:

    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::ICommandManager, "com.fizzyade.core.ICommandManager/1.0.0")

#endif // FIZZYADE_CORE_ICOMMANDMANAGER_H
