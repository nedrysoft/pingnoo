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

#ifndef FIZZYADE_CORE_IPINGTARGET_H
#define FIZZYADE_CORE_IPINGTARGET_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "IConfiguration.h"
#include <QHostAddress>

namespace FizzyAde::Core
{
    class IPingEngine;

    /**
     * @brief       Interface definition of a ping target
     *
     * @details     A ping target is used by an IPingEngine to keep track
     *              of destinations to be pinged.
     *
     */
    class FIZZYADE_CORE_DLLSPEC IPingTarget :
        public FizzyAde::ComponentSystem::IInterface,
        public FizzyAde::Core::IConfiguration
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::ComponentSystem::IInterface)
        Q_INTERFACES(FizzyAde::Core::IConfiguration)

    public:
        /**
         * Destructor
         *
         */
        virtual ~IPingTarget() = default;

        /**
         * Sets host address for this target
         *
         * @param[in]   hostAddress     the host address to be pinged
         *
         */
        virtual void setHostAddress(QHostAddress hostAddress) = 0;

        /**
         * Returns the host address for this target
         *
         * @return      the host address for this target
         *
         */
        virtual QHostAddress hostAddress() = 0;

        /**
         * Returns the IPingEngine that created this target
         *
         * @return      the IPingEngine that created this target
         *
         */
        virtual IPingEngine *engine() = 0;

        /**
         * Returns user data attached to this target
         *
         * @return      the attached user data
         *
         */
        virtual void *userData() = 0;

        /**
         * Attaches user data to this target
         *
         * @param[in]   data            the user data
         *
         */
        virtual void setUserData(void *data) = 0;
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IPingTarget, "com.fizzyade.core.IPingTarget/1.0.0")

#endif // FIZZYADE_CORE_IPINGTARGET_H
