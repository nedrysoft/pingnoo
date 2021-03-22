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
#if defined(Q_OS_MACOS)
#include "MacHelper/MacHelper.h"
#endif

#include <QAbstractButton>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#if !defined(Q_OS_MACOS)
#include <QMessageBox>
#endif
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
    newFavourite["ipversion"].setValue<Nedrysoft::Core::IPVersion>(ipVersion);

    for (auto favourite : m_favouriteList) {
        if (favourite["host"]==host) {
            m_favouriteList.removeFirst();
            break;
        }
    }

    m_favouriteList.insert(0, newFavourite);

    Q_EMIT favouritesChanged();
}

auto Nedrysoft::RouteAnalyser::TargetManager::addRecent(QVariantMap parameters) -> void {
    QVariantMap newRecent;
    QList<int> indices;

    for (auto recentIndex=0; recentIndex<m_recentsList.size(); recentIndex++) {
        if (m_recentsList.at(recentIndex)["host"].toString().compare(parameters["host"].toString(), Qt::CaseInsensitive)==0) {
            indices.push_front(recentIndex);
        }
    }

    for (auto index : indices) {
        m_recentsList.removeAt(index);
    }

    m_recentsList.insert(0, parameters);

    saveFavourites();

    Q_EMIT recentsChanged();
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
    newRecent["ipversion"].setValue<Nedrysoft::Core::IPVersion>(ipVersion);

    addRecent(newRecent);
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

        favouriteObject["host"] = favourite["host"].toString();
        favouriteObject["name"] = favourite["name"].toString();
        favouriteObject["description"] = favourite["description"].toString();
        favouriteObject["interval"] = favourite["interval"].toInt();
        favouriteObject["ipversion"] = favourite["ipversion"].toInt();

        favouritesArray.append(favouriteObject);
    }

    rootObject.insert("favourites", favouritesArray);

    auto recentsArray = QJsonArray();

    for (auto recent : m_recentsList) {
        QJsonObject recentObject;

        recentObject["host"] = recent["host"].toString();
        recentObject["name"] = recent["name"].toString();
        recentObject["description"] = recent["description"].toString();
        recentObject["interval"] = recent["interval"].toInt();
        recentObject["ipversion"] = recent["ipversion"].toInt();

        recentsArray.append(recentObject);
    }

    rootObject.insert("recents", recentsArray);

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
            favouriteMap["ipversion"].setValue<Nedrysoft::Core::IPVersion>(
                    favouriteObject["ipversion"].toVariant().value<Nedrysoft::Core::IPVersion>());

            m_favouriteList.append(favouriteMap);
        }
    }

    if (configuration.contains("recents")) {
        auto recents = configuration["recents"].toArray();

        for (auto recent : recents) {
            QVariantMap recentMap;
            QJsonObject recentObject = recent.toObject();

            recentMap["name"] = recentObject["name"].toString();
            recentMap["description"] = recentObject["description"].toString();
            recentMap["host"] = recentObject["host"].toString();
            recentMap["interval"] = recentObject["interval"].toInt();
            recentMap["ipversion"].setValue<Nedrysoft::Core::IPVersion>(
                    recentObject["ipversion"].toVariant().value<Nedrysoft::Core::IPVersion>());

             m_recentsList.append(recentMap);
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

auto Nedrysoft::RouteAnalyser::TargetManager::saveFavourites(QString filename) -> bool {
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

        QDir dir(QString("%1/%2").arg(configPaths.at(0)).arg(ConfigurationPath));

        if (!dir.exists()) {
            dir.mkpath(dir.path());
        }

        if (configurationFile.open(QFile::WriteOnly)) {
            QJsonDocument favouritesDocument;

            favouritesDocument.setObject(saveConfiguration());

            configurationFile.write(favouritesDocument.toJson());

            return true;
        }
    }

    return false;
}

auto Nedrysoft::RouteAnalyser::TargetManager::importFavourites(QWidget *parent) -> void {
    auto filename = QFileDialog::getOpenFileName(parent);

    if (!filename.isNull()) {
#if defined(Q_OS_MACOS)
        auto append = false;

        Nedrysoft::MacHelper::nativeAlert(
            parent,
            tr("Import Favourites"),
            tr("Would you like to append or overwrite the existing favourites?"),
            QStringList() << "Overwrite" << "Append" << "Cancel",

            [=](Nedrysoft::AlertButton::AlertButtonResult result) {
                auto append = false;

                switch (result) {
                    case Nedrysoft::AlertButton::Button(1): {
                        break;
                    }

                    case Nedrysoft::AlertButton::Button(2): {
                        append = true;
                        break;
                    }

                    default: {
                        return;
                    }
                }

                loadFavourites(filename, append);
        });
#else
        auto append = false;

        QMessageBox messageBox;

        auto appendButton =
                reinterpret_cast<QAbstractButton *>(messageBox.addButton(tr("Append"), QMessageBox::YesRole));
//         auto overwriteButton =
                reinterpret_cast<QAbstractButton *>(messageBox.addButton(tr("Owerwrite"), QMessageBox::NoRole));
        auto cancelButton =
                reinterpret_cast<QAbstractButton *>(messageBox.addButton(tr("Cancel"), QMessageBox::RejectRole));

        messageBox.setIcon(QMessageBox::Question);
        messageBox.setWindowTitle("Import Favourites");
        messageBox.setText("Would you like to append or overwrite the existing favourites?");

        messageBox.exec();

        if (messageBox.clickedButton()) {
            if (messageBox.clickedButton()==cancelButton) {
                return;
            } else if (messageBox.clickedButton()==appendButton) {
                append = true;
            } else {
                return;
            }
        }
#endif
        loadFavourites(filename, append);
    }
}

auto Nedrysoft::RouteAnalyser::TargetManager::exportFavourites(QWidget *parent) -> void {
    auto filename = QFileDialog::getSaveFileName(parent);

    if (!filename.isNull()) {
        saveFavourites(filename);
    }
}

auto Nedrysoft::RouteAnalyser::TargetManager::setFavourites(QList<QVariantMap> favourites) -> void {
    m_favouriteList = favourites;

    saveFavourites();
}