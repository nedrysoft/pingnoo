/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 28/03/2021.
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

#ifndef PINGNOO_COMPONENTS_ROUTEENGINE_ROUTEENGINEWORKER_H
#define PINGNOO_COMPONENTS_ROUTEENGINE_ROUTEENGINEWORKER_H

#include <ICore>
#include <IRouteEngine>

#include <QHostAddress>
#include <QObject>
#include <QThread>

namespace Nedrysoft { namespace Core {
    class IPingEngineFactory;
}}

namespace Nedrysoft { namespace RouteEngine {
    /**
     * @brief       The worker object for route discovery.
     */
    class RouteEngineWorker :
            public QObject {

    private:
        Q_OBJECT

    public:
        /**
         * @brief       Constructs a RouteEngineWorker.
         */
        RouteEngineWorker(QString target,
                          Nedrysoft::Core::IPingEngineFactory *pingEngineFactory,
                          Nedrysoft::Core::IPVersion ipVersion );

        /**
         * @brief       Destroys the RouteEngineWorker.
         */
        ~RouteEngineWorker();

        /**
         * @brief       The worker thread.
         */
        auto doWork() -> void;

        /**
         * @brief       This signal is emitted when a route has finished discovery.
         *
         * @param[in]   hostAddress the target that was requested.
         * @param[in]   result the route list.
         */
        Q_SIGNAL void result(const QHostAddress hostAddress, const Nedrysoft::Core::RouteList result);

    private:
        Nedrysoft::Core::IPingEngineFactory *m_pingEngineFactory;
        Nedrysoft::Core::IPVersion m_ipVersion;
        QString m_host;

        bool m_isRunning;
    };
}}

#endif //PINGNOO_COMPONENTS_ROUTEENGINE_ROUTEENGINEWORKER_H

