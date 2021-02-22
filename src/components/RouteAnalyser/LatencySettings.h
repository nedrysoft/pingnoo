/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 21/02/2021.
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
#ifndef NEDRYSOFT_LATENCYSETTINGS_H
#define NEDRYSOFT_LATENCYSETTINGS_H

#include "Core/IConfiguration.h"
#include <QColor>
#include <QObject>

namespace Nedrysoft::RouteAnalyser {
    class LatencySettings : public
            QObject,
            Nedrysoft::Core::IConfiguration {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IConfiguration)

        public:
            /**
             * @brief       Constructs a new LatencySettings.
             */
            LatencySettings();

            /**
             * @brief       Destroys the LatencySettings.
             */
            ~LatencySettings();

            /**
             * @brief       Returns the warning value in seconds.
             *
             * @returns     the warning value.
             */
            auto warningValue() -> double;

            /**
             * @brief       Returns the critical value in seconds.
             *
             * @returns     the critical value.
             */
            auto criticalValue() -> double;

            /**
             * @brief       Returns the value as a formatted string.
             *
             * @returns     the value as a formatted string.
             */
            auto toString(double value) -> QString;

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
             * @brief       Sets the warning value by parsing the given string.
             *
             * @param[in]   value the warning threshold.
             */
            auto setWarningValue(QString value) -> void;

            /**
             * @brief       Sets the warning value by parsing the given string.
             *
             * @param[in]   value the warning threshold.
             */
            auto setCriticalValue(QString value) -> void;

            /**
             * @brief       Returns the colour used for ideal latency values.
             *
             * @returns     the colour.
             */
            auto idealColour() -> QRgb;

            /**
             * @brief       Returns the colour used for warning latency values.
             *
             * @returns     the colour.
             */
            auto warningColour() -> QRgb;

            /**
             * @brief       Returns the colour used for critical latency values.
             *
             * @returns     the colour.
             */
            auto criticalColour() -> QRgb;

            /**
             * @brief       Sets the ideal colour for latency values.
             *
             * @param[in]   colour the colour.
             */
            auto setIdealColour(QRgb colour) -> void;

            /**
             * @brief       Sets the warning colour for latency values.
             *
             * @param[in]   colour the colour.
             */
            auto setWarningColour(QRgb colour) -> void;

            /**
             * @brief       Sets the critical colour for latency values.
             *
             * @param[in]   colour the colour.
             */
            auto setCriticalColour(QRgb colour) -> void;

        public:
            /**
              * @brief       Saves the configuration to a JSON object.
              *
              * @returns     the JSON configuration.
              */
            virtual auto saveConfiguration() -> QJsonObject;

            /**
             * @brief       Loads the configuration.
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            virtual auto loadConfiguration(QJsonObject configuration) -> bool;

        private:
            double m_warningThreshold;
            double m_criticalThreshold;

            QRgb m_idealColour;
            QRgb m_warningColour;
            QRgb m_criticalColour;
    };
}

#endif //NEDRYSOFT_LATENCYSETTINGS_H
