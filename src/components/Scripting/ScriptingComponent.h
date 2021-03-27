/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 24/03/2021.
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

#ifndef NEDRYSOFT_SCRIPTING_SCRIPTINGCOMPONENT_H
#define NEDRYSOFT_SCRIPTING_SCRIPTINGCOMPONENT_H

#include "ComponentSystem/IComponent.h"
#include "ScriptingSpec.h"

/**
 * @brief       The ScriptingComponent class provides provides a javascript scripting component .
 */
class NEDRYSOFT_SCRIPTING_DLLSPEC ScriptingComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT

        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")

        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        /**
         * @brief       Constructs the ScriptingComponent.
         */
        ScriptingComponent();

        /**
         * @brief       Destroys the ScriptingComponent.
         */
        ~ScriptingComponent();

    public:
        /**
         * @brief       The initialiseEvent is called by the component loader to initialise the component.
         *
         * @details     Called by the component loader after all components have been loaded, called in load order.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::initialiseEvent
         */
        auto initialiseEvent() -> void override;

        /**
         *  @brief       The finaliseEvent is called by the component loader to de-initialise the component.
         *
         *  @details    Called by the component loader in reverse load order to shutdown the component.
         *
         *  @see         Nedrysoft::ComponentSystem::IComponent::finaliseEvent
         */
        auto finaliseEvent() -> void override;
};

#endif // NEDRYSOFT_SCRIPTING_SCRIPTINGCOMPONENT_H
