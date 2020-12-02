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

#ifndef NEDRYSOFT_ROUTEENGINE_ROUTEWORKER_H
#define NEDRYSOFT_ROUTEENGINE_ROUTEWORKER_H

#include "RouteEngine.h"

#include <QHostAddress>
#include <QList>
#include <QObject>

namespace Nedrysoft::RouteEngine {
    /**
     * @brief       Route finder thread worker implementation
     *
     * @details     Pings the target with an increasing TTL until reaching the target
     *              machine.
     *
     */
    class RouteWorker :
            public QObject {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructor
             *
             * @details     creates a route worker with the selected IP version
             *
             * @param[in]   ipVersion           the ip version to be used
             *
             */
            RouteWorker(Nedrysoft::Core::IPVersion ipVersion);

            /**
             * @brief       Destructor
             *
             */
            ~RouteWorker();

            /**
             * @brief       Sets the target host to find the route for
             *
             * @param[in]   host                the host
             *
             */
            void setHost(QString host);

        private:

            /**
             * Pings an IPv4 address with the given ttl
             *
             * Helper function which returns the hop address and whether it was the
             * final destination.
             *
             * @param[in]   hostAddress         the host to ping
             * @param[in]   ttl                 the TTL used for this ping
             * @param[out]  returnAddress       the address that the ping response came from
             * @param[out]  isComplete          true if final hop, false if not
             *
             * @return      true if ping sent, otherwise false
             *
             */
            bool ping_v4(const QHostAddress &hostAddress, int ttl, QHostAddress *returnAddress, bool *isComplete);

            /**
             * Pings an IPv4 address with the given hop limit
             *
             * Helper function which returns the hop address and whether it was the
             * final destination.
             *
             * @param[in]   hostAddress         the host to ping
             * @param[in]   hopLimit            the hop limit used for this ping
             * @param[out]  returnAddress       the address that the ping response came from
             * @param[out]  isComplete          true if final hop, false if not
             *
             * @return      true if ping sent, otherwise false
             *
             */
            bool ping_v6(const QHostAddress &hostAddress, int hopLimit, QHostAddress *returnAddress, bool *isComplete);

        public slots:

            /**
             * @brief       The receiver thread worker
             *
             */
            void doWork(void);

        signals:

            /**
             * @brief       Signals when a route is available
             *
             * @param[in]   hostAddress         the host address of the target
             * @param[in]   result              the list of hops
             *
             */
            void result(const QHostAddress &hostAddress, const Nedrysoft::Core::RouteList &result);

            friend class RouteEngine;

        protected:
            bool m_isRunning;                   //! Whether the host is running

        private:
            QString m_host;                     //! The host that is being queried
            Nedrysoft::Core::IPVersion m_ipVersion; //! The IP version to be used

    };
}

#endif // NEDRYSOFT_ROUTEENGINE_ROUTEWORKER_H
