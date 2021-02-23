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

#include "Cache.h"

#include <QDateTime>
#include <QDir>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QStandardPaths>
#include <spdlog/spdlog.h>

constexpr auto cacheDatabase = "Nedrysoft::IPAPIGeoIPProvider::Cache";

Nedrysoft::IPAPIGeoIPProvider::Cache::Cache() {
    auto dataLocations = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);

    if (dataLocations.isEmpty()) {
        SPDLOG_ERROR(QString("error opening database. (Unable to get AppLocalDataLocation)").toStdString());

        return;
    }

    auto dbFileInfo = QFileInfo(dataLocations.at(0), "ip-api-cache.db");

    auto database = QSqlDatabase::addDatabase("QSQLITE", cacheDatabase);

    database.setDatabaseName(dbFileInfo.absoluteFilePath());

    if (!dbFileInfo.dir().exists()) {
        auto dir = QDir();

        if (!dir.mkpath(dbFileInfo.dir().absolutePath())) {
            SPDLOG_ERROR(QString("error opening database. (%1)").arg(database.lastError().text()).toStdString());

            return;
        }
    }

    if (!database.open()) {
        SPDLOG_ERROR(QString("error opening database. (%1)").arg(database.lastError().text()).toStdString());

        return;
    }

    auto query = QSqlQuery(database);

    auto result = query.exec(R"(CREATE TABLE IF NOT EXISTS ip (
                                  id INTEGER PRIMARY KEY,
                                  name TEXT,
                                  creationTime INTEGER,
                                  country TEXT,
                                  countryCode TEXT,
                                  region TEXT,
                                  regionName TEXT,
                                  city TEXT,
                                  zip TEXT,
                                  lat REAL,
                                  lon REAL,
                                  timezone TEXT,
                                  isp TEXT,
                                  org TEXT,
                                  asn TEXT
    ))");

    if (!result) {
        SPDLOG_WARN(QString("error creating table. (%1)").arg(query.lastError().text()).toStdString());
    }

    query.finish();
}

Nedrysoft::IPAPIGeoIPProvider::Cache::Cache::~Cache() {
    QSqlDatabase::removeDatabase(cacheDatabase);
}

auto Nedrysoft::IPAPIGeoIPProvider::Cache::add(QJsonObject object) -> void {
    QSqlDatabase database = QSqlDatabase::database(cacheDatabase);
    QSqlQuery query(database);

    query.prepare(
            "INSERT INTO ip (name, creationTime, country, countryCode, region, regionName, city, zip, lat, lon, timezone, isp, org, asn) "
            "VALUES (:name, :creationTime, :country, :countryCode, :region, :regionName, :city, :zip, :lat, :lon, :timezone, :isp, :org, :asn)");

    query.bindValue(":name", object["query"].toVariant());
    query.bindValue(":creationTime", QDateTime::currentDateTimeUtc().toTime_t());
    query.bindValue(":country", object["country"].toVariant());
    query.bindValue(":countryCode", object["countryCode"].toVariant());
    query.bindValue(":region", object["region"].toVariant());
    query.bindValue(":regionName", object["regionName"].toVariant());
    query.bindValue(":city", object["city"].toVariant());
    query.bindValue(":zip", object["zip"].toVariant());
    query.bindValue(":lat", object["lat"].toVariant());
    query.bindValue(":lon", object["lon"].toVariant());
    query.bindValue(":timezone", object["timezone"].toVariant());
    query.bindValue(":isp", object["isp"].toVariant());
    query.bindValue(":org", object["org"].toVariant());
    query.bindValue(":asn", object["as"].toVariant());

    auto result = query.exec();

    if (!result) {
        SPDLOG_WARN(QString("error adding record.  (%1)").arg(query.lastError().text()).toStdString());
    }

    query.finish();
}

auto Nedrysoft::IPAPIGeoIPProvider::Cache::find(const QString &name, QJsonObject &object) -> bool {
    QSqlDatabase database = QSqlDatabase::database(cacheDatabase);
    QSqlQuery query(database);
    bool queryResult = false;

    query.prepare("SELECT * FROM ip WHERE name=:name");
    query.bindValue(":name", name);

    if (query.exec()) {
        if (query.next()) {
            object["creationTime"] = QJsonValue::fromVariant(query.value("creationTime"));
            object["country"] = QJsonValue::fromVariant(query.value("country"));
            object["countryCode"] = QJsonValue::fromVariant(query.value("countryCode"));
            object["region"] = QJsonValue::fromVariant(query.value("region"));
            object["regionName"] = QJsonValue::fromVariant(query.value("regionName"));
            object["city"] = QJsonValue::fromVariant(query.value("city"));
            object["zip"] = QJsonValue::fromVariant(query.value("zip"));
            object["lat"] = QJsonValue::fromVariant(query.value("lat"));
            object["lon"] = QJsonValue::fromVariant(query.value("lon"));
            object["timezone"] = QJsonValue::fromVariant(query.value("timezone"));
            object["isp"] = QJsonValue::fromVariant(query.value("isp"));
            object["org"] = QJsonValue::fromVariant(query.value("org"));
            object["asn"] = QJsonValue::fromVariant(query.value("asn"));

            queryResult = true;
        }
    }

    return queryResult;
}
