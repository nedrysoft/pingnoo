/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
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

#include "Cache.h"

#include <QDir>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QStandardPaths>
#include <spdlog/spdlog.h>

Nedrysoft::HostIPGeoIPProvider::Cache::Cache() {
    auto dataLocations = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);

    if (dataLocations.isEmpty()) {
        return;
    }

    auto dbFileInfo = QFileInfo(dataLocations.at(0), "host-ip-cache.db");

    m_database = QSqlDatabase::addDatabase(
            "QSQLITE",
            QString("Nedrysoft::HostIPGeoIPProvider::Cache") );

    m_database.setDatabaseName(dbFileInfo.absoluteFilePath());

    if (!dbFileInfo.dir().exists()) {
        auto dir = QDir();

        if (!dir.mkpath(dbFileInfo.dir().absolutePath())) {
            return;
        }
    }

    if (!m_database.open()) {
        return;
    }

    QSqlQuery query(m_database);

    auto result = query.exec(R"(CREATE TABLE ip (
                                  id INTEGER PRIMARY KEY,
                                  name TEXT,
                                  creationTime INTEGER,
                                  country TEXT,
                                  countryCode TEXT,
                                  city TEXT

    ))");

    if (!result) {
        SPDLOG_ERROR(QString("error creating table. (%1)").arg(query.lastError().text()).toStdString());
    }
}

Nedrysoft::HostIPGeoIPProvider::Cache::Cache::~Cache() {
    m_database.close();

    QSqlDatabase::removeDatabase("Nedrysoft::HostIPGeoIPProvider::Cache");
}

auto Nedrysoft::HostIPGeoIPProvider::Cache::add(QJsonObject object) -> void {
    //QSqlDatabase database = QSqlDatabase::database("Nedrysoft::HostIPGeoIPProvider::Cache");
    QSqlQuery query(m_database);

    query.prepare("INSERT INTO ip (name, creationTime, country, countryCode, city) "
                  "VALUES (:name, :creationTime, :country, :countryCode, :city)");

    query.bindValue(":name", object["ip"].toVariant());
    query.bindValue(":creationTime", QDateTime::currentDateTimeUtc().toTime_t());
    query.bindValue(":country", object["country_name"].toVariant());
    query.bindValue(":countryCode", object["country_code"].toVariant());
    query.bindValue(":city", object["city"].toVariant());

    auto result = query.exec();

    if (!result) {
        SPDLOG_ERROR(QString("error adding record. (%1)").arg(query.lastError().text()).toStdString());
    }
}

auto Nedrysoft::HostIPGeoIPProvider::Cache::find(const QString &name, QJsonObject &object) -> bool {
    //QSqlDatabase database = QSqlDatabase::database("Nedrysoft::HostIPGeoIPProvider::Cache");
    QSqlQuery query(m_database);

    query.prepare("SELECT * FROM ip WHERE name=:name");
    query.bindValue(":name", name);

    if (query.exec()) {
        if (query.next()) {
            object["creationTime"] = QJsonValue::fromVariant(query.value("creationTime"));
            object["country"] = QJsonValue::fromVariant(query.value("country"));
            object["countryCode"] = QJsonValue::fromVariant(query.value("countryCode"));
            object["city"] = QJsonValue::fromVariant(query.value("city"));

            return true;
        }
    }

    return false;
}
