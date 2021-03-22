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

#include "TargetSettings.h"

#include "ComponentSystem/IComponentManager.h"
#include "Core/IPingEngineFactory.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

constexpr auto ConfigurationPath = "Nedrysoft/Pingnoo/Components/RouteAnalyser";
constexpr auto ConfigurationFilename = "TargetSettings.json";

constexpr auto DefaultHostTarget = "1.1.1.1";
constexpr auto DefaultIPVersion = Nedrysoft::Core::IPVersion::V4;
constexpr auto DefaultPingInterval = 2.5;

Nedrysoft::RouteAnalyser::TargetSettings::TargetSettings() :
        m_defaultPingEngine(QString::Null()),
        m_defaultHostTarget(DefaultHostTarget),
        m_defaultPingInterval(DefaultPingInterval),
        m_defaultIPVersion(DefaultIPVersion) {

}

Nedrysoft::RouteAnalyser::TargetSettings::~TargetSettings() {

}

auto Nedrysoft::RouteAnalyser::TargetSettings::saveConfiguration() -> QJsonObject {
    auto rootObject = QJsonObject();
    auto itemArray = QJsonArray();

    rootObject.insert("id", this->metaObject()->className());

    QJsonObject targetObject;

    targetObject.insert("defaultHost", m_defaultHostTarget);
    targetObject.insert("defaultPingEngine", m_defaultPingEngine);
    targetObject.insert("pingInterval", m_defaultPingInterval);
    targetObject.insert("ipVersion", static_cast<int>(m_defaultIPVersion));

    rootObject.insert("target", targetObject);

    return rootObject;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::loadConfiguration(QJsonObject configuration) -> bool {
    if (configuration["id"] != this->metaObject()->className()) {
        return false;
    }

    if (configuration.contains("target")) {
        auto targetObject = configuration["target"].toObject();

        if (targetObject.contains("defaultHost")) {
            m_defaultHostTarget = targetObject["defaultHost"].toString();
        }

        if (targetObject.contains("defaultPingEngine")) {
            m_defaultPingEngine = targetObject["defaultPingEngine"].toString();
        }

        if (targetObject.contains("pingInterval")) {
            m_defaultPingInterval = targetObject["pingInterval"].toDouble();
        }

        if (targetObject.contains("ipVersion")) {
            m_defaultIPVersion = static_cast<Nedrysoft::Core::IPVersion>(targetObject["ipVersion"].toInt());
        }
    }

    return true;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::loadFromFile(QString filename, bool append) -> bool {
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

auto Nedrysoft::RouteAnalyser::TargetSettings::saveToFile(QString filename) -> void {
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

auto Nedrysoft::RouteAnalyser::TargetSettings::setDefaultHost(QString host) -> void {
    m_defaultHostTarget = host;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::defaultHost() -> QString {
    if (m_defaultHostTarget.isEmpty()) {
        auto pingEngineFactories = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IPingEngineFactory>();

        int priority = 0;
        QString engineId;

        for (auto pingEngineFactory : pingEngineFactories) {
            if (pingEngineFactory->priority() > priority) {
                engineId = pingEngineFactory->metaObject()->className();
                priority = pingEngineFactory->priority();
            }
        }

        m_defaultHostTarget = engineId;
    }

    return m_defaultHostTarget;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::setDefaultPingEngine(QString id) -> void {
    m_defaultPingEngine = id;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::defaultPingEngine() -> QString {
    QString selectedPingEngine = m_defaultPingEngine;

    if (selectedPingEngine.isEmpty()) {
        auto pingEngines = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IPingEngineFactory>();

        if (pingEngines.count()) {
            int priority = pingEngines.at(0)->priority();

            for (auto pingEngine : pingEngines) {

                if (pingEngine->priority() < priority) {
                    priority = pingEngine->priority();
                    selectedPingEngine = pingEngine->metaObject()->className();
                }
            }
        }
    }

    return selectedPingEngine;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::setDefaultPingInterval(double interval) -> void {
    m_defaultPingInterval = interval;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::defaultPingInterval() -> double {
    return m_defaultPingInterval;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::setDefaultIPVersion(Nedrysoft::Core::IPVersion version) -> void {
    m_defaultIPVersion = version;
}

auto Nedrysoft::RouteAnalyser::TargetSettings::defaultIPVersion() -> Nedrysoft::Core::IPVersion {
    return m_defaultIPVersion;
}
