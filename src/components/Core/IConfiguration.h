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

#ifndef FIZZYADE_CORE_ICONFIGURATION_H
#define FIZZYADE_CORE_ICONFIGURATION_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include <QObject>
#include <QJsonObject>

namespace FizzyAde::Core
{
    class IPingEngine;

    /**
     * Interface definition of a ping engine
     *
     * An engine implements the logic of transmitting, receiving
     * and associating replies to ping requests, it then signals
     * when a ping result is available
     *
     */

    class FIZZYADE_CORE_DLLSPEC IConfiguration
    {

    public:

        /**
         * Saves the configuration
         *
         * @return      the configuration
         *
         */
        virtual QJsonObject saveConfiguration() = 0;

        /**
         * Loads the configuration
         *
         * @param[in]   configuration   the configuration
         *
         * @return      true if loaded, false otherwise
         *
         */
        virtual bool loadConfiguration(QJsonObject configuration) = 0;
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IConfiguration, "com.fizzyade.core.IConfiguration/1.0.0")

#endif // FIZZYADE_CORE_ICONFIGURATION_H
