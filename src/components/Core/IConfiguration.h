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

#ifndef NEDRYSOFT_CORE_ICONFIGURATION_H
#define NEDRYSOFT_CORE_ICONFIGURATION_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QJsonObject>
#include <QObject>

namespace Nedrysoft::Core {
    class IPingEngine;

    /**
     * @brief       The IConfiguration interface provides a contract for laoding & saving data.
     */
    class NEDRYSOFT_CORE_DLLSPEC IConfiguration {
        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @returns     the JSON configuration.
             */
            virtual QJsonObject saveConfiguration() = 0;

            /**
             * @brief       Loads the configuration.
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            virtual bool loadConfiguration(QJsonObject configuration) = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IConfiguration, "com.nedrysoft.core.IConfiguration/1.0.0")

#endif // NEDRYSOFT_CORE_ICONFIGURATION_H
