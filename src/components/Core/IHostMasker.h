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

#ifndef NEDRYSOFT_CORE_IHOSTMASKER_H
#define NEDRYSOFT_CORE_IHOSTMASKER_H

#include "ComponentSystem/IInterface.h"
#include "Core/IConfiguration.h"
#include "CoreSpec.h"

#include <QJsonObject>
#include <QObject>
#include <QString>

namespace Nedrysoft::Core {
    /**
     * @brief       Interface definition of a host masker.
     *
     * @details     A host masker is used to mask the displayed host names & ip addresses in the software user
     *              interface (and any outputs), this allows screen captures to be taken without requiring further
     *              editing/redacting.
     */
    class NEDRYSOFT_CORE_DLLSPEC IHostMasker :
            public Nedrysoft::ComponentSystem::IInterface,
            public Nedrysoft::Core::IConfiguration {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)
            Q_INTERFACES(Nedrysoft::Core::IConfiguration)

        public:

            /**
             * @brief       Masks a host name/ip.
             *
             * @detail      A IHostMasker can redact the hostname and/or host address based of a combination of
             *              host name, host address and the hop number.
             *
             *              If there is no match, then the original values are returned.
             *
             * @param[in]   hop the hop number.
             * @param[in]   hostName the host name to be checked.
             * @param[in]   hostAddress the host IP to be checked.
             * @param[out]  maskedHostName the masked host name.
             * @param[out]  maskedHostAddress the masked host IP.
             *
             * @returns     returns true on replacement; otherwise false.
             */
            virtual auto mask(
                    int hop,
                    const QString &hostName,
                    const QString &hostAddress,
                    QString &maskedHostName,
                    QString &maskedHostAddress ) -> bool = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IHostMasker, "com.nedrysoft.core.IHostMasker/1.0.0")

#endif // NEDRYSOFT_CORE_IHOSTMASKER_H
