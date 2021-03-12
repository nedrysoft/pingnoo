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

#include "LatencySettings.h"

#include "ColourManager.h"
#include "Utils.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

auto constexpr WarningDefaultValue = 0.2;
auto constexpr CriticalDefaultValue = 0.5;

constexpr auto ConfigurationPath = "Nedrysoft/Pingnoo/Components/RouteAnalyser";
constexpr auto ConfigurationFilename = "LatencySettings.json";

Nedrysoft::RouteAnalyser::LatencySettings::LatencySettings() :
        m_warningThreshold(WarningDefaultValue),
        m_criticalThreshold(CriticalDefaultValue),
        m_idealColour(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour()),
        m_warningColour(Nedrysoft::RouteAnalyser::ColourManager::getWarningColour()),
        m_criticalColour(Nedrysoft::RouteAnalyser::ColourManager::getCriticalColour()),
        m_useGradientFill(true) {

}

Nedrysoft::RouteAnalyser::LatencySettings::~LatencySettings() {

}

auto Nedrysoft::RouteAnalyser::LatencySettings::saveConfiguration() -> QJsonObject {
    auto rootObject = QJsonObject();
    auto itemArray = QJsonArray();

    rootObject.insert("id", this->metaObject()->className());

    QJsonObject thresholdsObject;

    thresholdsObject.insert("critical", static_cast<double>(m_criticalThreshold));
    thresholdsObject.insert("warning", static_cast<double>(m_warningThreshold));

    rootObject.insert("thresholds", thresholdsObject);

    QJsonObject coloursObject;

    coloursObject.insert("ideal", QColor(m_idealColour).name());
    coloursObject.insert("warning", QColor(m_warningColour).name());
    coloursObject.insert("critical", QColor(m_criticalColour).name());
    coloursObject.insert("useGradient", m_useGradientFill);

    rootObject.insert("colours", coloursObject);

    return rootObject;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::loadConfiguration(QJsonObject configuration) -> bool {
    if (configuration["id"] != this->metaObject()->className()) {
        return false;
    }

    if (configuration.contains("thresholds")) {
        auto thresholdsObject = configuration["thresholds"].toObject();

        if (thresholdsObject.contains("warning")) {
            m_warningThreshold = thresholdsObject["warning"].toDouble();
        }

        if (thresholdsObject.contains("critical")) {
            m_criticalThreshold = thresholdsObject["critical"].toDouble();
        }
    }

    if (configuration.contains("colours")) {
        auto coloursObject = configuration["colours"].toObject();

        if (coloursObject.contains("ideal")) {
            m_idealColour = QColor(coloursObject["ideal"].toString()).rgb();
        }

        if (coloursObject.contains("warning")) {
            m_warningColour = QColor(coloursObject["warning"].toString()).rgb();
        }

        if (coloursObject.contains("critical")) {
            m_criticalColour = QColor(coloursObject["critical"].toString()).rgb();
        }

        if (coloursObject.contains("useGradient")) {
            m_useGradientFill = coloursObject.value("useGradient").toBool();
        }
    }

    return true;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::warningValue() -> double {
    return m_warningThreshold;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::criticalValue() -> double {
    return m_criticalThreshold;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::loadFromFile(QString filename, bool append) -> bool {
    Q_UNUSED(append)

    QStringList configPaths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);

    if (configPaths.count()) {
        QFile configurationFile;

        if (filename.isNull()) {
            auto filePath = QString("%1/%2/%3")
                    .arg(configPaths.at(0))
                    .arg(ConfigurationPath)
                    .arg(QString(ConfigurationFilename));

            configurationFile.setFileName(QDir::cleanPath(filePath));
        } else {
            configurationFile.setFileName(filename);
        }

        if (configurationFile.open(QFile::ReadOnly)) {
            auto jsonDocument = QJsonDocument::fromJson(configurationFile.readAll());

            if (jsonDocument.isObject()) {
                loadConfiguration(jsonDocument.object());

                return true;
            }
        }
    }

    return false;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::saveToFile(QString filename) -> void {
    QStringList configPaths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);

    if (configPaths.count()) {
        QFile configurationFile;

        if (filename.isNull()) {
            auto filePath = QString("%1/%2/%3")
                    .arg(configPaths.at(0))
                    .arg(ConfigurationPath)
                    .arg(QString(ConfigurationFilename));

            configurationFile.setFileName(QDir::cleanPath(filePath));
        } else {
            configurationFile.setFileName(filename);
        }

        QDir dir(configPaths.at(0));

        if (!dir.exists(ConfigurationPath)) {
            dir.mkpath(ConfigurationPath);
        }

        if (configurationFile.open(QFile::WriteOnly)) {
            QJsonObject configuration = saveConfiguration();

            auto jsonDocument = QJsonDocument(configuration);

            if (jsonDocument.isObject()) {
                configurationFile.write(jsonDocument.toJson());
            }
        }
    }
}

auto Nedrysoft::RouteAnalyser::LatencySettings::setWarningValue(QString value) -> void {
    if (!Nedrysoft::Utils::parseIntervalString(value, m_warningThreshold)) {
        return;
    }
}

auto Nedrysoft::RouteAnalyser::LatencySettings::setCriticalValue(QString value) -> void {
    if (!Nedrysoft::Utils::parseIntervalString(value, m_criticalThreshold)) {
        return;
    }
}

auto Nedrysoft::RouteAnalyser::LatencySettings::idealColour() -> QRgb {
    return m_idealColour;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::warningColour() -> QRgb {
    return m_warningColour;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::criticalColour() -> QRgb {
    return m_criticalColour;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::setIdealColour(QRgb colour) -> void {
    m_idealColour = colour;

    Q_EMIT coloursChanged();
}

auto Nedrysoft::RouteAnalyser::LatencySettings::setWarningColour(QRgb colour) -> void {
    m_warningColour = colour;

    Q_EMIT coloursChanged();
}

auto Nedrysoft::RouteAnalyser::LatencySettings::setCriticalColour(QRgb colour) -> void {
    m_criticalColour = colour;

    Q_EMIT coloursChanged();
}

auto Nedrysoft::RouteAnalyser::LatencySettings::resetColours() -> void {
    m_idealColour = ColourManager::getIdealColour();
    m_warningColour = ColourManager::getWarningColour();
    m_criticalColour = ColourManager::getCriticalColour();

    Q_EMIT coloursChanged();
}

auto Nedrysoft::RouteAnalyser::LatencySettings::resetThresholds() -> void {
    m_warningThreshold = WarningDefaultValue;
    m_criticalThreshold = CriticalDefaultValue;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::setGradientFill(bool useGradient) -> void {
    m_useGradientFill = useGradient;

    Q_EMIT gradientChanged(useGradient);
}

auto Nedrysoft::RouteAnalyser::LatencySettings::gradientFill() -> bool {
    return m_useGradientFill;
}