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

#ifndef NEDRYSOFT_CORE_IGEOIPPROVIDER_H
#define NEDRYSOFT_CORE_IGEOIPPROVIDER_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QHostAddress>

namespace Nedrysoft::Core {
    using GeoFunction = std::function<void(const QString &, const QVariantMap &)>;

    /**
     * @brief       Interface definition of a Geo IP provider
     *
     * @details     A Geo IP provider gives a location for the given IP address, accuracy of the data will vary
     *              from provider to provider.
     */
    class NEDRYSOFT_CORE_DLLSPEC IGeoIPProvider :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       Destroys the IGeoIPProvider.
             */
            virtual ~IGeoIPProvider() = default;

            /**
             * @brief       Performs a host lookup using IP address or hostname.
             *
             * @details     The operation is asynchronous and the result is provided via the
             *              Nedrysoft::Core::IGeoIPProvider::result signal.
             *
             * @param[in]   host the host address to be looked up.
             */
            virtual auto lookup(const QString host) -> void = 0;

            /**
             * @brief       Performs a host lookup using IP address or hostname.
             *
             * @details     This overloaded function uses a std::function to obtain the result, this can be
             *              a callback function or a lambda function.
             *
             * @param[in]   host the host address to be looked up.
             * @param[in]   function the function called when a result is available.
             */
            virtual auto lookup(const QString host, Nedrysoft::Core::GeoFunction function) -> void = 0;

            /**
             * @brief       Signals that a result is available.
             *
             * @notes       The data returned from the GEO IP provider will vary, however, a IGeoIPProvider is expected
             *              to produce a variant map with the following named fields (if available).
             *
             *              * creationTime
             *              * country
             *              * countryCode
             *              * region
             *              * regionName
             *              * city
             *              * zip
             *              * lat
             *              * lon
             *              * timezone
             *              * isp
             *              * org
             *
             * @param[out]  host the host that was looked up.
             * @param[out]  result the result as a QVariantMap
             */
            Q_SIGNAL void result(const QString host, const QVariantMap result);
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IGeoIPProvider, "com.nedrysoft.core.IGeoIPProvider/1.0.0")

#endif // NEDRYSOFT_CORE_IGEOIPPROVIDER_H
