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

#ifndef PINGNOO_COMPONENTS_CORE_ICONFIGURATION_H
#define PINGNOO_COMPONENTS_CORE_ICONFIGURATION_H

#include "CoreSpec.h"

#include <IInterface>
#include <QJsonObject>
#include <QObject>

namespace Nedrysoft { namespace Core {
    class IPingEngine;

    /**
     * @brief       The IConfiguration interface provides a contract for loading & saving data.
     *
     * @class       Nedrysoft::Core::IConfiguration IConfiguration.h <IConfiguration>
     */
    class NEDRYSOFT_CORE_DLLSPEC IConfiguration {
        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @returns     the JSON configuration.
             */
            virtual auto saveConfiguration() -> QJsonObject = 0;

            /**
             * @brief       Loads the configuration.
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            virtual auto loadConfiguration(QJsonObject configuration) -> bool = 0;

            // Classes with virtual functions should not have a public non-virtual destructor:
            virtual ~IConfiguration() = default;
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IConfiguration, "com.nedrysoft.core.IConfiguration/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_ICONFIGURATION_H
