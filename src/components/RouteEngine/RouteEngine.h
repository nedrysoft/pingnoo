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

#ifndef FIZZYADE_ROUTEENGINE_ROUTEENGINE_H
#define FIZZYADE_ROUTEENGINE_ROUTEENGINE_H

#include "Core/IRouteEngine.h"
#include <QObject>
#include <QList>
#include <QHostAddress>
#include <Core/Core.h>

class QThread;

namespace FizzyAde::RouteEngine
{
    class RouteWorker;

    /**
     * IRouteEngine implementation
     *
     * Implements the IRouteEngine to determine the list of
     * hops between the source and target
     *
     */

    class RouteEngine :
        public FizzyAde::Core::IRouteEngine
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IRouteEngine)

    public:
        /**
         * Default constructor
         */
        RouteEngine();

        /**
         * destructor
         */
        ~RouteEngine();

        /**
         * @sa IRouteEngine
         */
        virtual void findRoute(QString host, FizzyAde::Core::IPVersion ipVersion=FizzyAde::Core::IPVersion::V4);

    private:
        QThread *m_workerThread;                     //! The route finder thread
        RouteWorker *m_worker;                       //! The thread worker for route finder
        FizzyAde::Core::IPVersion m_ipVersion;       //! The IP version to use for the route finder instance
    };
}

#endif // FIZZYADE_ROUTEENGINE_ROUTEENGINE_H
