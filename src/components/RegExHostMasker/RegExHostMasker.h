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

#ifndef NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKER_H
#define NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKER_H

#include "ComponentSystem/IInterface.h"
#include "RegExHostMaskerSpec.h"
#include "Core/IHostMasker.h"
#include <QSet>
#include <QObject>

namespace Nedrysoft::RegExHostMasker {
    class RegExHostMaskerItem {
        public:
            QSet<int> m_hops;

            QString m_matchExpression;
            QString m_replacementString;
            QString m_addressReplacementString;
            QString m_hopString;

            unsigned int m_matchFlags;
    };

    /**
     * @brief       Definition for the regex host masker
     *
     * @details     This host marker accepts a regular expression to match the host name
     *              or address and allows the masked output to be generated using capture
     *              groups
     *
     */
    class RegExHostMasker :
            public Nedrysoft::Core::IHostMasker {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IHostMasker)

        public:
            enum MatchFlags {
                MatchHop = ( 1 << 0 ),
                MatchAddress = ( 1 << 1 ),
                MatchHost = ( 1 << 2 ),
                MaskAddress = ( 1 << 3 ),
                MaskHost = ( 1 << 4 )
            };

        public:
            /**
             * @brief       Adds a host name match mask
             *
             * @param[in]   matchFlags              a bit mask of values from MatchFlags
             * @param[in]   matchExpression         the regular expression used for name matching
             * @param[in]   replacementString       the string to be used for replacement
             * @param[in]   hopString optional      hop string listing hops this mask is valid for
             *
             */
            void add(unsigned int matchFlags, QString matchExpression, QString replacementString,
                     QString hopString = QString());

            /**
             * @sa          IHostMasker
             *
             */
            virtual bool mask(int hop, const QString &hostName, const QString &hostAddress, QString &maskedHostName,
                              QString &maskedHostAddress);

            /**
             * @sa          IConfiguration
             *
             */
            virtual QJsonObject saveConfiguration();

            virtual bool loadConfiguration(QJsonObject configuration);

        private:
            /**
             * @brief       Attempts to apply a mask to a string
             *
             * @param[in]   hop                     the hop number
             * @param[in]   hostName                the host name to be checked
             * @param[in]   hostAddress             the host address to be checked
             * @param[out]  maskedHostName          the masked host name
             * @param[out]  maskedHostAddress       the masked host address
             *
             * @return      true on a match, else false
             *
             */
            bool applyMask(int hop, const QString &hostName, const QString &hostAddress, QString &maskedHostName,
                           QString &maskedHostAddress);

        private:
            QList<RegExHostMaskerItem> m_maskList;

    };
}

#endif // NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKER_H
