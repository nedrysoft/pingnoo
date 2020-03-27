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

#ifndef FIZZYADE_HOSTIPGEOAPITPROVIDER_HOSTIGEOPAPIPROVIDER_H
#define FIZZYADE_HOSTIPGEOAPITPROVIDER_HOSTIGEOPAPIPROVIDER_H

#include "ComponentSystem/IInterface.h"
#include "Core/IGeoIPProvider.h"
#include "HostIPGeoIPProviderSpec.h"
#include <QObject>
#include <QVariantMap>

namespace FizzyAde::HostIPGeoIPProvider
{
    class Cache;

    /**
     * Definition for the built in host masker
     *
     * This host marker accepts a regular expression to match the host name
     * or address and allows the masked output to be generated using capture
     * groups
     *
     */

    class HostIPGeoIPProvider :
        public FizzyAde::Core::IGeoIPProvider
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IGeoIPProvider)

    public:
        HostIPGeoIPProvider();
        ~HostIPGeoIPProvider();

        /**
         * @sa IGeoIPProvider
         */

        virtual void lookup(const QString host);
        virtual void lookup(const QString host, FizzyAde::Core::GeoFunction function);

    private:

    private:
        FizzyAde::HostIPGeoIPProvider::Cache *m_cache;
    };
}

#endif // FIZZYADE_HOSTIPGEOAPITPROVIDER_HOSTIGEOPAPIPROVIDER_H
