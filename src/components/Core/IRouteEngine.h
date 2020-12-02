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

#ifndef NEDRYSOFT_CORE_IROUTEENGINE_H
#define NEDRYSOFT_CORE_IROUTEENGINE_H

#include "CoreSpec.h"
#include "Core.h"
#include "ComponentSystem/IInterface.h"
#include <QObject>
#include <QHostAddress>

namespace Nedrysoft::Core {
    typedef QList<QHostAddress> RouteList;

    /**
     * @brief       Interface definition of a route discovery engine
     *
     * @details     Provides the means of discovering the route to a
     *              given host
     *
     */
    class NEDRYSOFT_CORE_DLLSPEC IRouteEngine :
            public Nedrysoft::ComponentSystem::IInterface {
        Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
            * @brief       Constructor
            *
            */
            virtual ~IRouteEngine() = default;

            /**
             * @brief       Starts route discovery for a host
             *
             * @param[in]   host        the host name or address to be traced
             * @param[in]   ipVersion   the IP version to be used for the trace
             *
             */
            virtual void findRoute(QString host, Nedrysoft::Core::IPVersion ipVersion) = 0;

            /**
             * @brief       Signal emitted when the route discovery is completed
             *
             * @param[in]   result      the discovered route
             *
             */
            Q_SIGNAL void result(const QHostAddress &hostAddress, const Nedrysoft::Core::RouteList &result);
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IRouteEngine, "com.fizzyade.core.IRouteEngine/1.0.0")

#endif // NEDRYSOFT_CORE_IROUTEENGINE_H
