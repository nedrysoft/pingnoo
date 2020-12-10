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
     * @brief       Definition for the built in host masker
     *
     * @details     This host marker accepts a regular expression to match the host name
     *              or address and allows the masked output to be generated using capture
     *              groups
     */
    class IPAPIGeoIPProvider :
            public Nedrysoft::Core::IGeoIPProvider {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IGeoIPProvider)

        public:
            /**
             * @brief       Constructor
             */
            IPAPIGeoIPProvider();

            /**
             * @brief       Destructor
             */
            ~IPAPIGeoIPProvider();

            /**
             * @sa          IGeoIPProvider
             */
            virtual void lookup(const QString host);

            virtual void lookup(const QString host, Nedrysoft::Core::GeoFunction function);

        private:
            Nedrysoft::IPAPIGeoIPProvider::Cache *m_cache;
    };
}

#endif // NEDRYSOFT_IPAPIGEOIPPROVIDER_IPAPIGEOIPPROVIDER_H
