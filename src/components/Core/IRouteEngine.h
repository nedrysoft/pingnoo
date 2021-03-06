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

#ifndef NEDRYSOFT_CORE_IROUTEENGINE_H
#define NEDRYSOFT_CORE_IROUTEENGINE_H

#include "ComponentSystem/IInterface.h"
#include "Core.h"
#include "CoreSpec.h"

#include <QHostAddress>
#include <QObject>

namespace Nedrysoft::Core {
    typedef QList<QHostAddress> RouteList;
    class IPingEngineFactory;

    /**
     * @brief       The IRouteEngine interface describes the mechanism of finding the route to a host.
     */
    class NEDRYSOFT_CORE_DLLSPEC IRouteEngine :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       Destroys the IRouteEngine.
             */
            virtual ~IRouteEngine() = default;

            /**
             * @brief       Starts route discovery for a host.
             *
             * @notes       Route discovery is a asynchronous operation, the result signal is emitted when the
             *              discovery is completed.
             *
             * @param[in]   engineFactory the ping engine to be used for discoveru.
             * @param[in]   host the target host name or address.
             * @param[in]   ipVersion the IP version to be used for discovery.
             */
            virtual auto findRoute(Nedrysoft::Core::IPingEngineFactory *engineFactory, QString host, Nedrysoft::Core::IPVersion ipVersion) -> void = 0;

            /**
             * @brief       Signal emitted when the route discovery is completed.
             *
             * @param[in]   result the discovered route.
             */
            Q_SIGNAL void result(const QHostAddress &hostAddress, const Nedrysoft::Core::RouteList result);
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IRouteEngine, "com.nedrysoft.core.IRouteEngine/1.0.0")

#endif // NEDRYSOFT_CORE_IROUTEENGINE_H
