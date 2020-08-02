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

#ifndef FIZZYADE_CORE_IGEOIPPROVIDER_H
#define FIZZYADE_CORE_IGEOIPPROVIDER_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include <QHostAddress>

namespace FizzyAde::Core
{
    using GeoFunction = std::function <void (const QString &, const QVariantMap &)>;

    /**
     * @brief       Interface definition of a ping target
     *
     * @details     A ping target is used by an IPingEngine to keep track
     *              of destinations to be pinged.
     *
     */
    class FIZZYADE_CORE_DLLSPEC IGeoIPProvider :
        public FizzyAde::ComponentSystem::IInterface
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::ComponentSystem::IInterface)

    public:
        /**
         * @brief Destructor
         */
        virtual ~IGeoIPProvider() = default;

        /**
         * @brief       Performs a host lookup
         *
         * @param[in]   host        the host address to be looked up
         *
         */
        virtual void lookup(const QString host) = 0;

        /**
         * @brief       Performs a host lookup
         *
         * @param[in]   host        the host address to be looked up
         * @param[in]   function    callback function
         *
         */
        virtual void lookup(const QString host, FizzyAde::Core::GeoFunction function) = 0;

        /**
         * @brief       Signal for result
         *
         * @param[out]  host        the host
         * @param[out]  result      the result
         *
         */
        Q_SIGNAL void result(const QString host, const QVariantMap result);
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IGeoIPProvider, "com.fizzyade.core.IGeoIPProvider/1.0.0")

#endif // FIZZYADE_CORE_IGEOIPPROVIDER_H
