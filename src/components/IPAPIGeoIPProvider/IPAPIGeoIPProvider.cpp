/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
 * An open source ping path analyser
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

#include "IPAPIGeoIPProvider.h"

#include "Cache.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

Nedrysoft::IPAPIGeoIPProvider::IPAPIGeoIPProvider::IPAPIGeoIPProvider() :
        m_cache(new Nedrysoft::IPAPIGeoIPProvider::Cache()) {

}

Nedrysoft::IPAPIGeoIPProvider::IPAPIGeoIPProvider::~IPAPIGeoIPProvider() {
    delete m_cache;
}

void Nedrysoft::IPAPIGeoIPProvider::IPAPIGeoIPProvider::lookup(const QString host, Nedrysoft::Core::GeoFunction function) {
    auto cacheResultObject = QJsonObject();

    if (m_cache->find(host, cacheResultObject)) {
        function(host, cacheResultObject.toVariantMap());
    } else {
        const auto mapFields =
                QStringList() << "creationTime" << "country" << "countryCode" << "region" << "regionName" << "city"
                              << "zip" << "lat" "lon" << "timezone" << "isp" << "org";
        auto manager = new QNetworkAccessManager();

        connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                auto resultMap = QVariantMap();
                auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

                if (!jsonDocument.isObject()) {
                    auto requiredFields = QStringList(mapFields) << "as";
                    auto responseValid = true;

                    for (const auto &field : requiredFields) {
                        if (!jsonDocument.object().contains(field)) {
                            responseValid = false;
                            break;
                        }
                    }

                    if (responseValid) {
                        m_cache->add(jsonDocument.object());

                        for (const auto &field : mapFields) {
                            resultMap[field] = jsonDocument.object()[field].toVariant();
                        }

                        resultMap["creationTime"] = QDateTime::currentDateTimeUtc().toTime_t();
                        resultMap["asn"] = jsonDocument.object()["as"].toVariant();

                        function(host, resultMap);
                    }
                }
            }

            reply->deleteLater();

            auto manager = qobject_cast<QNetworkAccessManager *>(sender());

            if (manager) {
                manager->deleteLater();
            }
        });

        manager->get(QNetworkRequest(QUrl("http://ip-api.com/json/" + host)));
    }
}

void Nedrysoft::IPAPIGeoIPProvider::IPAPIGeoIPProvider::lookup(const QString host) {
    lookup(host, [=](const QString &hostAddress, const QVariantMap &result) {
        emit this->result(hostAddress, result);
    });
}

