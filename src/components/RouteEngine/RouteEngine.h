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

#ifndef PINGNOO_COMPONENTS_ROUTEENGINE_ROUTEENGINE_H
#define PINGNOO_COMPONENTS_ROUTEENGINE_ROUTEENGINE_H

#include <IRouteEngine>

#include <ICore>
#include <PingResult>
#include <QHostAddress>
#include <QHostInfo>
#include <QList>

class QThread;

namespace Nedrysoft { namespace Core {
    class IPingEngineFactory;
    class IPingEngine;
}}

namespace Nedrysoft { namespace RouteEngine {
    class RouteEngineWorker;

    /**
     * @brief       The RouteEngine provides an implementation of IRouteEngine.
     *
     * @details     Implements the IRouteEngine to determine the list of hops between the source and target.
     */
    class RouteEngine :
            public Nedrysoft::RouteAnalyser::IRouteEngine {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::RouteAnalyser::IRouteEngine)

        public:
            /**
             * @brief       Constructs a RouteEngine.
             */
            RouteEngine();

        public:
            /**
             * @brief       Starts route discovery for a host.
             *
             * @note        Route discovery is a asynchronous operation, the result signal is emitted when the
             *              discovery is completed.
             *
             * @param[in]   engineFactory the ping engine to be used for route discovery.
             * @param[in]   host the target host name or address.
             * @param[in]   ipVersion the IP version to be used for discovery.
             */
            auto findRoute(
                    Nedrysoft::RouteAnalyser::IPingEngineFactory *engineFactory,
                    QString host,
                    Nedrysoft::Core::IPVersion ipVersion = Nedrysoft::Core::IPVersion::V4
            ) -> void override;

        private:
            //! @cond

            Nedrysoft::RouteEngine::RouteEngineWorker *m_routeWorker;
            QThread *m_routeWorkerThread;

            //! @endcond
    };
}}

#endif // PINGNOO_COMPONENTS_ROUTEENGINE_ROUTEENGINE_H
