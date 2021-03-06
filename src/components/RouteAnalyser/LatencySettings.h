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

#include "ComponentSystem/IComponentManager.h"

#include <QColor>
#include <QObject>

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       The LatencySettings class is used as the central location for latency settings, this includes
     *              parameters such as the default values for the latency thresholds and the default colour which
     *              is used for the various boundaries.
     */
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

            /**
             * @brief       Resets the colours back to default..
             *
             * @note        Under macOS if the theme is dark mode, then this function will reset to the dark mode
             *              palette, under light mode it resets to the light mode palette.
             * @return
             */
            auto resetColours() -> void;

            /**
             * @brief       Resets the threashold values back to default.
             */
            auto resetThresholds() -> void;

            /**
             * @brief       This signal is emitted when the user changes the default colours.
             */
            Q_SIGNAL void coloursChanged();

            /**
             * @brief       Sets whether gradient or solid fills are used to draw latency.
             *
             * @param[in]   useGradient true if use gradient; otherwise false (solid).
             */
            auto setGradientFill(bool useGradient) -> void;

            /**
             * @brief       Returns whether gradient fills are used.
             *
             * @returns     true if gradient fill; otherwise false (solid).
             */
            auto gradientFill() -> bool;

            /**
             * @brief       Called when the state of the gradient fill changes.
             *
             * @paramin]    useGradient true to use gradient fill; otherwise false.
             */
            Q_SIGNAL void gradientChanged(bool useGradient);

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

        public:
            /**
             * @brief       Returns the global instnace of the latency settings object.
             *
             * @returns     the global instance.
             */
            static auto getInstance() -> LatencySettings * {
                return Nedrysoft::ComponentSystem::getObject<Nedrysoft::RouteAnalyser::LatencySettings>();
            }

        private:
            double m_warningThreshold;
            double m_criticalThreshold;

            QRgb m_idealColour;
            QRgb m_warningColour;
            QRgb m_criticalColour;

            bool m_useGradientFill;
    };
}

#endif //NEDRYSOFT_LATENCYSETTINGS_H
