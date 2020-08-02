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

#ifndef FIZZYADE_HOSTIPGEOIPPROVIDER_CACHE_H
#define FIZZYADE_HOSTIPGEOIPPROVIDER_CACHE_H

#include <QSqlDatabase>
#include <QJsonObject>

namespace FizzyAde::HostIPGeoIPProvider
{
    /**
     * @brief       Simple cache for GEO IP results
     *
     * @details     Provides a simple persistent database for GEOIP results
     *              to prevent unneccesary lookups
     *
     */
    class Cache
    {
    public:
        /**
         * @brief   Constructor
         */
        Cache();

        /**
         * @brief   Destructor
         */
        ~Cache();

        /**
         * @brief           Add a lookup result to the database
         *
         * @param[in]       Json result
         *
         */
        void add(QJsonObject object);

        /**
         * @brief           Check if an IP is cached
         *
         * @param[in]       host to check
         * @param[out]      json object containing the host information
         *
         */
        bool find(const QString &name, QJsonObject &object);

    protected:

    private:

    };
}

#endif // FIZZYADE_HOSTIPGEOIPPROVIDER_CACHE_H
