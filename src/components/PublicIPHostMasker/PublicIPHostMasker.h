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

#ifndef NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKER_H
#define NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKER_H

#include "ComponentSystem/IInterface.h"
#include "Core/IConfiguration.h"
#include "Core/IHostMasker.h"

#include <QObject>

namespace Nedrysoft::PublicIPHostMasker {
    /**
     * @brief       Definition for the built in host masker
     *
     * @details     This host marker accepts a regular expression to match the host name
     *              or address and allows the masked output to be generated using capture
     *              groups
     */
    class PublicIPHostMasker :
            public Nedrysoft::Core::IHostMasker {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IHostMasker)

        public:
            /**
             * @sa          IHostMasker
             */
            virtual bool mask(int hop, const QString &hostName, const QString &hostAddress, QString &maskedHostName,
                              QString &maskedHostAddress);

            /**
             * @sa          IConfiguration
             */
            virtual QJsonObject saveConfiguration();

            virtual bool loadConfiguration(QJsonObject configuration);

        private:
            QString m_publicIP;

    };
}

#endif // NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKER_H
