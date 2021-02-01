/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/01/2021.
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

#ifndef NEDRYSOFT_CORE_ILOGGER_H
#define NEDRYSOFT_CORE_ILOGGER_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <spdlog/spdlog.h>

namespace Nedrysoft::Core {
    /**
     * @brief       The ILogger class is used to allow spdlog to be managed in the main application.
     */
    class NEDRYSOFT_CORE_DLLSPEC ILogger {

        public:
            ILogger() {

            };

            /**
             * @brief       Returns the spdlog logger.
             *
             * @returns     the logger
             */
            virtual auto logger() -> std::shared_ptr<spdlog::logger> = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ILogger, "com.nedrysoft.core.ILogger/1.0.0")

#endif // NEDRYSOFT_CORE_ILOGGER_H
