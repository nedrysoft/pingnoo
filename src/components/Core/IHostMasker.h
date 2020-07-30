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

#ifndef FIZZYADE_CORE_IHOSTMASKER_H
#define FIZZYADE_CORE_IHOSTMASKER_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "Core/IConfiguration.h"
#include <QObject>
#include <QString>
#include <QJsonObject>

namespace FizzyAde::Core
{
    /**
     * Interface definition of a host masker
     *
     * A host masker is used to mask the displayed host names & ip addresses
     * in the softwares user interface, this allows screen captures to be
     * taken without requiring further editing
     *
     */

    class FIZZYADE_CORE_DLLSPEC IHostMasker :
        public FizzyAde::ComponentSystem::IInterface,
        public FizzyAde::Core::IConfiguration
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::ComponentSystem::IInterface)
        Q_INTERFACES(FizzyAde::Core::IConfiguration)

    public:

        /**
         * Replaces a host name/ip
         *
         * @param[in]   hop                 the hop number for this host & address
         * @param[in]   hostName            the host name to be checked
         * @param[in]   hostAddress         the host IP to be checked
         * @param[out]  maskedHostName      the masked host name
         * @param[out]  maskedHostAddress   the masked host IP
         *
         * @return      returns true on replacement, else false
         *
         */
        virtual bool mask(int hop, const QString &hostName, const QString &hostAddress, QString &maskedHostName, QString &maskedHostAddress) = 0;
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IHostMasker, "com.fizzyade.core.IHostMasker/1.0.0")

#endif // FIZZYADE_CORE_IHOSTMASKER_H
