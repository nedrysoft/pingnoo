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

#ifndef PINGNOO_COMPONENTS_REGEXHOSTMASKER_REGEXHOSTMASKER_H
#define PINGNOO_COMPONENTS_REGEXHOSTMASKER_REGEXHOSTMASKER_H

#include "RegExHostMaskerSpec.h"

#include <IHostMasker>
#include <IInterface>
#include <QSet>

namespace Nedrysoft { namespace RegExHostMasker {
    /**
     * @brief       The RegExHostMaskerItem class holds the required information needed by a regular expression
     *              substitution of host names/ip address.
     */
    class RegExHostMaskerItem {
        public:
            //! @cond

            QSet<int> m_hops;

            QString m_matchExpression;
            QString m_hostReplacementString;
            QString m_addressReplacementString;
            QString m_hopString;
            QString m_description;
            bool m_enabled;

            unsigned int m_matchFlags;

            //! @endcond
    };

    /**
     * @brief       The TokenReplacement class is used internally by the masker to provide substitution of
     *              capture groups.
     */
    class TokenReplacement {
        public:
            //! @cond

            QString type;
            QString identifier;
            QString value;
            QString format;

            //! @endcond
    };

    /**
     * @brief       The RegExHostMasker class provides a host masker that redacts using a regular expression.
     *
     * @details     This host marker accepts a regular expression to match the host name or address and allows the
     *              masked output to be generated using capture groups.
     */
    class RegExHostMasker :
            public Nedrysoft::Core::IHostMasker {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IHostMasker)

        public:
            /**
             * @brief       the MatchFlags determine how a match should be made and how the mask should be applied.
             */
            enum class MatchFlags {
                MatchHop = ( 1 << 0 ),
                MatchAddress = ( 1 << 1 ),
                MatchHost = ( 1 << 2 ),
                MaskAddress = ( 1 << 3 ),
                MaskHost = ( 1 << 4 )
            };

        public:
            /*
             * @brief       Constructs a new RegExHostMasker.
             */
            RegExHostMasker();

            /**
             * @brief       Loads the configuration from disk.
             *
             * @note        If no filename is passed, then the components configuration is used.
             *
             * @param[in]   filename the file to load.
             * @param[in]   append if true configuration is appended; otherwise it overwrites.
             *
             * @returns     true if configuration was changed; otherwise false.
             */
            auto loadFromFile(QString filename = QString(), bool append = false) -> bool;

            /**
             * @brief       Saves the configuration to disk.
             *
             * @note        If no filename is passed, then the components configuration is used.
             *
             * @param[in]   filename the file to save to.
             */
            auto saveToFile(QString filename = QString()) -> void;

            /**
             * @brief       Adds a host name match expression.
             *
             * @param[in]   matchFlags the bit mask of values from MatchFlags to control masking.
             * @param[in]   description the description displayed for this masker.
             * @param[in]   matchExpression the regular expression used for name matching.
             * @param[in]   hostReplacementString the string to be used for host name replacement.
             * @param[in]   addressReplacementString the string to be used for host address replacement.
             * @param[in]   hopString hop string listing hops this mask is valid for. (optional)
             * @param[in]   enabled whether this entry is enabled.
             */
            auto add(
                    unsigned int matchFlags,
                    const QString &description,
                    const QString &matchExpression,
                    const QString &hostReplacementString,
                    const QString &addressReplacementString,
                    const QString &hopString,
                    const bool enabled= true
            ) -> void;

        public:
            /**
             * @brief       Masks a host name/ip using the defined regular expression.
             *
             * @details     A IHostMasker can redact the hostname and/or host address based of a combination of
             *              host name, host address and the hop number.
             *
             *              If there is no match, then the original values are returned.
             *
             * @see         Nedrysoft::Core::IHostMasker
             *
             * @param[in]   hop the hop number.
             * @param[in]   hostName the host name to be checked.
             * @param[in]   hostAddress the host IP to be checked.
             * @param[out]  maskedHostName the masked host name.
             * @param[out]  maskedHostAddress the masked host IP.
             *
             * @returns     returns true on replacement; otherwise false.
             */
            auto mask(
                    int hop,
                    const QString &hostName,
                    const QString &hostAddress,
                    QString &maskedHostName,
                    QString &maskedHostAddress ) -> bool override;

        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @see         Nedrysoft::Core::IConfiguration::saveConfiguration
             *
             * @returns     the JSON configuration.
             */
            auto saveConfiguration() -> QJsonObject override;

            /**
             * @brief       Loads the configuration.
             *
             * @see         Nedrysoft::Core::IConfiguration::loadConfiguration
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            auto loadConfiguration(QJsonObject configuration) -> bool override;

        private:
            /**
             * @brief       Returns whether the given target matches the mask.
             *
             * @param[in]   hop the hop number
             * @param[in]   hostName the host name to be checked
             * @param[in]   hostAddress the host address to be checked
             * @param[out]  maskedHostName the masked host name
             * @param[out]  maskedHostAddress the masked host address
             *
             * @returns     true on a match; otherwise false.
             */
            auto applyMask(
                    int hop,
                    const QString &hostName,
                    const QString &hostAddress,
                    QString &maskedHostName,
                    QString &maskedHostAddress ) -> bool;

            friend class RegExHostMaskerSettingsPageWidget;

        private:
            //! @cond

            QList<RegExHostMaskerItem> m_maskList;

            //! @endcond

    };
}}

Q_DECLARE_METATYPE(Nedrysoft::RegExHostMasker::RegExHostMaskerItem)

#endif // PINGNOO_COMPONENTS_REGEXHOSTMASKER_REGEXHOSTMASKER_H
