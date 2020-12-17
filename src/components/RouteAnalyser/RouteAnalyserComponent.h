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

#ifndef NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSERCOMPONENT_H
#define NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSERCOMPONENT_H

#include "ComponentSystem/IComponent.h"
#include "RouteAnalyserEditor.h"
#include "RouteAnalyserSpec.h"

/**
 * @brief       The RouteAnalyserComponent class provides the route analysis feature to the application.
 */
class NEDRYSOFT_ROUTEANALYSER_DLLSPEC RouteAnalyserComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT

        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")

        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        /**
         * @brief       Constructs a new RouteAnalyserComponent instance.
         */
        RouteAnalyserComponent();

        /**
         * @brief       Destroys the RouteAnalyserComponent.
         */
        ~RouteAnalyserComponent();

        /**
         * @brief       Returns the context id for the route analyser.
         *
         * @return      the context id.
         */
        int contextId();

    public:
        /**
         * @brief       The initialiseEvent is called by the component loader to initialise the component.
         *
         * @details     Called by the component loader after all components have been loaded, called in load order.
         */
        virtual void initialiseEvent();

        /**
         * @brief       The initialisationFinishedEvent function is called by the component loader after all
         *              components have been initialised.
         *
         * @details     Called by the component loader after all components have been
         *              initialised, called in reverse load order.
         */
        virtual void initialisationFinishedEvent();

        friend class Nedrysoft::RouteAnalyser::RouteAnalyserEditor;

    private:
        int m_editorContextId = 0;
};

#endif // NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSERCOMPONENT_H
