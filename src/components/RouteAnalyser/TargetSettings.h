/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 22/02/2021.
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

#ifndef NEDRYSOFT_TARGETSETTINGS_H
#define NEDRYSOFT_TARGETSETTINGS_H

#include "Core/Core.h"
#include "Core/IConfiguration.h"

#include <QObject>

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The TargetSettings class is used as the central location for target settings.
     */
    class TargetSettings : public
            QObject,
            Nedrysoft::Core::IConfiguration {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IConfiguration)

        public:
            /**
             * @brief       Constructs a new TargetSettings.
             */
            TargetSettings();

            /**
             * @brief       Destroys the TargetSettings.
             */
            ~TargetSettings();

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
            auto loadFromFile(QString filename = QString::Null(), bool append = false) -> bool;

            /**
             * @brief       Saves the configuration to disk.
             *
             * @note        If no filename is passed, then the components configuration is used.
             *
             * @param[in]   filename the file to save to.
             */
            auto saveToFile(QString filename = QString::Null()) -> void;

            /**
             * @brief       Sets the default target that is shown on the ribbon and new target dialogs
             *
             * @param[in]   host the IP or hostname of the target.
             */
            auto setDefaultHost(QString host) -> void;

            /**
             * @brief       Returns the default target.
             *
             * @returns     the host or ip of the default target.
             */
            auto defaultHost() -> QString;

            /**
             * @brief       Sets the default ping engine.
             *
             * @param[in]   id the identifier of the ping engine.
             */

            auto setDefaultPingEngine(QString id) -> void;

            /**
             * @brief       Returns the default ping engine identifier.
             *
             * @returns     the identifier of the ping engine.
             */
            auto defaultPingEngine() -> QString;

            /**
            * @brief       Sets the default interval between pings.
            *
            * @param[in]   inverval the interval in seconds between pings.
            */
            auto setDefaultPingInterval(double interval) -> void;

            /**
            * @brief       Returns the default interval between pings.
            *
            * @returns      the interval in seconds between pings.
            */
            auto defaultPingInterval() -> double;

            /**
             * @brief       Sets the default IP version
             *
             * param[in]    version is the IP version, 4 or 6.
             */
             auto setDefaultIPVersion(Nedrysoft::Core::IPVersion version) -> void;

            /**
             * @brief       Returns the default IP version
             */
             auto defaultIPVersion() -> Nedrysoft::Core::IPVersion;

        public:
            /**
              * @brief       Saves the configuration to a JSON object.
              *
              * @returns     the JSON configuration.
              */
            auto saveConfiguration() -> QJsonObject override;

            /**
             * @brief       Loads the configuration.
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            auto loadConfiguration(QJsonObject configuration) -> bool override;

        private:
            QString m_defaultPingEngine;
            QString m_defaultHostTarget;
            double m_defaultPingInterval;
            Nedrysoft::Core::IPVersion m_defaultIPVersion;

    };
}

#endif //NEDRYSOFT_TARGETSETTINGS_H
