/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 06/03/2021.
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

#include "TargetManager.h"

#include "Core/ICore.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

constexpr auto ConfigurationPath = "Nedrysoft/Pingnoo/Components/RouteAnalyser";
constexpr auto ConfigurationFilename = "Favourites.json";

Nedrysoft::RouteAnalyser::TargetManager::TargetManager() {
    loadFavourites();
}

auto Nedrysoft::RouteAnalyser::TargetManager::getInstance() -> Nedrysoft::RouteAnalyser::TargetManager * {
    static auto instance = new Nedrysoft::RouteAnalyser::TargetManager;

    return instance;
}

auto Nedrysoft::RouteAnalyser::TargetManager::addFavourite(
        QString host,
        QString name,
        QString description,
        Nedrysoft::Core::IPVersion ipVersion ) -> void {

    QVariantMap newFavourite;

    newFavourite["host"] = host;
    newFavourite["name"] = name;
    newFavourite["description"] = description;
    newFavourite["ipVersion"].setValue<Nedrysoft::Core::IPVersion>(ipVersion);

    for (auto favourite : m_favouriteList) {
        if (favourite["host"]==host) {
            m_favouriteList.removeFirst();
            break;
        }
    }

    m_favouriteList.insert(0, newFavourite);

    Q_EMIT favouritesChanged();
}

auto Nedrysoft::RouteAnalyser::TargetManager::addRecent(
        QString host,
        QString name,
        QString description,
        Nedrysoft::Core::IPVersion ipVersion ) -> void {

    QVariantMap newRecent;

    newRecent["host"] = host;
    newRecent["name"] = name;
    newRecent["description"] = description;
    newRecent["ipVersion"].setValue<Nedrysoft::Core::IPVersion>(ipVersion);

    for (auto recent : m_recentsList) {
        if (recent["host"]==host) {
            m_recentsList.removeFirst();
            break;
        }
    }

    m_recentsList.insert(0, newRecent);

    Q_EMIT recentsChanged();
}

auto Nedrysoft::RouteAnalyser::TargetManager::favourites() -> QList<QVariantMap> {
    return m_favouriteList;
}

auto Nedrysoft::RouteAnalyser::TargetManager::recents() -> QList<QVariantMap> {
    return m_recentsList;
}

auto Nedrysoft::RouteAnalyser::TargetManager::saveConfiguration() -> QJsonObject {
    auto rootObject = QJsonObject();

    rootObject.insert("id", this->metaObject()->className());

    auto favouritesArray = QJsonArray();

    for (auto favourite : m_favouriteList) {
        QJsonObject favouriteObject;

        favouriteObject["host"] = favourite["host"].toJsonValue();
        favouriteObject["name"] = favourite["name"].toJsonValue();
        favouriteObject["description"] = favourite["description"].toJsonValue();
        favouriteObject["interval"] = favourite["description"].toJsonValue();
        favouriteObject["ipVersion"] = favourite["ipVersion"].toJsonValue();

        favouritesArray.append(favouriteObject);
    }

    rootObject.insert("favourites", favouritesArray);

    return rootObject;
}

auto Nedrysoft::RouteAnalyser::TargetManager::loadConfiguration(QJsonObject configuration) -> bool {
    if (configuration["id"] != this->metaObject()->className()) {
        return false;
    }

    if (configuration.contains("favourites")) {
        auto favourites = configuration["favourites"].toArray();

        for (auto favourite : favourites) {
            QVariantMap favouriteMap;
            QJsonObject favouriteObject = favourite.toObject();

            favouriteMap["name"] = favouriteObject["name"].toString();
            favouriteMap["description"] = favouriteObject["description"].toString();
            favouriteMap["host"] = favouriteObject["host"].toString();
            favouriteMap["interval"] = favouriteObject["interval"].toInt();
            favouriteMap["ipVersion"].setValue<Nedrysoft::Core::IPVersion>(
                    favouriteObject["ipVersion"].toVariant().value<Nedrysoft::Core::IPVersion>());

            m_favouriteList.append(favouriteMap);
        }
    }

    return true;
}

auto Nedrysoft::RouteAnalyser::TargetManager::loadFavourites(QString filename, bool append) -> bool {
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
                if (!append) {
                    m_favouriteList.clear();
                }

                loadConfiguration(jsonDocument.object());

                return true;
            }
        }
    }

    return false;
}