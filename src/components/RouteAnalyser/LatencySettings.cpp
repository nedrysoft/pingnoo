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

#include "Utils.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

auto constexpr warningDefaultValue = 0.2;
auto constexpr criticalDefaultValue = 0.5;

constexpr auto millsecondsInSecond = 1000.0;

constexpr auto configurationPath = "Components/RouteAnalyser";
constexpr auto configurationFilename = "LatencySettings.json";

Nedrysoft::RouteAnalyser::LatencySettings::LatencySettings() :
        m_warningThreshold(warningDefaultValue),
        m_criticalThreshold(criticalDefaultValue) {

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

    return true;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::warningValue() -> double {
    return m_warningThreshold;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::criticalValue() -> double {
    return m_criticalThreshold;
}

auto Nedrysoft::RouteAnalyser::LatencySettings::toString(double value) -> QString {
    if (value>=1) {
        return QString(tr("%1 s")).arg(value, 0, 'g', 4, '0');
    } else {
        return QString(tr("%1 ms")).arg(value*millsecondsInSecond, 1, 'f', 0, '0');
    }
}

auto Nedrysoft::RouteAnalyser::LatencySettings::loadFromFile(QString filename, bool append) -> bool {
    QStringList configPaths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);

    if (configPaths.count()) {
        QFile configurationFile;

        if (filename.isNull()) {
            configurationFile.setFileName(QDir::cleanPath(QString("%1/%2/%3").arg(configPaths.at(0)).arg(configurationPath).arg(QString(configurationFilename))));
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
            configurationFile.setFileName(QDir::cleanPath(QString("%1/%2/%3").arg(configPaths.at(0)).arg(configurationPath).arg(QString(configurationFilename))));
        } else {
            configurationFile.setFileName(filename);
        }

        QDir dir(configPaths.at(0));

        if (!dir.exists(configurationPath)) {
            dir.mkpath(configurationPath);
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