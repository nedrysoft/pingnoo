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

#ifndef NEDRYSOFT_IPAPIGEOIPPROVIDER_IPAPIGEOIPPROVIDER_H
#define NEDRYSOFT_IPAPIGEOIPPROVIDER_IPAPIGEOIPPROVIDER_H

#include "ComponentSystem/IInterface.h"
#include "Core/IGeoIPProvider.h"
#include "IPAPIGeoIPProvider.h"
#include "IPAPIGeoIPProviderSpec.h"

#include <QObject>
#include <QVariantMap>

namespace Nedrysoft::IPAPIGeoIPProvider {
    class Cache;

    /**
     * @brief       The IPAPIGeoIPProvider provides a ip-api.com geo IP lookup.
     *
     * @details     Uses ip-api.com to lookup an IP address to determine the geo location.
     */
    class IPAPIGeoIPProvider :
            public Nedrysoft::Core::IGeoIPProvider {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IGeoIPProvider)

        public:
            /**
             * @brief       Constructs a IPAPIGeoIPProvider.
             */
            IPAPIGeoIPProvider();

            /**
             * @brief       Destroys the IPAPIGeoIPProvider.
             */
            ~IPAPIGeoIPProvider();

            /**
             * @brief       Performs a host lookup using IP address or hostname.
             *
             * @details     The operation is asynchronous and the result is provided via the
             *              Nedrysoft::Core::IGeoIPProvider::result signal.
             *
             * @see         Nedrysoft::Core::IGeoIPProvider::lookup
             *
             * @param[in]   host the host address to be looked up.
             */
             auto lookup(const QString host) -> void override;

            /**
             * @brief       Performs a host lookup using IP address or hostname.
             *
             * @details     This overloaded function uses a std::function to obtain the result, this can be
             *              a callback function or a lambda function.
             *
             * @see         Nedrysoft::Core::IGeoIPProvider::lookup
             *
             * @param[in]   host the host address to be looked up.
             * @param[in]   function the function called when a result is available.
             */
            auto lookup(const QString host, Nedrysoft::Core::GeoFunction function) -> void override;


        private:
            Nedrysoft::IPAPIGeoIPProvider::Cache *m_cache;
    };
}

#endif // NEDRYSOFT_IPAPIGEOIPPROVIDER_IPAPIGEOIPPROVIDER_H
