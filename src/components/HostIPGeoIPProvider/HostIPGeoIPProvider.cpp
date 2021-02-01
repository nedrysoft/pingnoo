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
#include "HostIPGeoIPProvider.h"

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

Nedrysoft::HostIPGeoIPProvider::HostIPGeoIPProvider::HostIPGeoIPProvider() :
        m_cache(new Nedrysoft::HostIPGeoIPProvider::Cache()) {

}

Nedrysoft::HostIPGeoIPProvider::HostIPGeoIPProvider::~HostIPGeoIPProvider() {
    delete m_cache;
}

auto Nedrysoft::HostIPGeoIPProvider::HostIPGeoIPProvider::lookup(
        const QString host,
        Nedrysoft::Core::GeoFunction function ) -> void {

    auto cacheResultObject = QJsonObject();

    if (m_cache->find(host, cacheResultObject)) {
        function(host, cacheResultObject.toVariantMap());
    } else {
        auto manager = new QNetworkAccessManager();

        connect(manager, &QNetworkAccessManager::finished, [this, host, function](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                auto resultMap = QVariantMap();

                auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

                if (jsonDocument.isObject()) {
                    auto requiredFields = QStringList() << "country_name" << "city" << "country_code";
                    auto responseValid = true;

                    for (const auto &field : requiredFields) {
                        if (!jsonDocument.object().contains(field)) {
                            responseValid = false;
                            break;
                        }
                    }

                    if (responseValid) {
                        m_cache->add(jsonDocument.object());

                        resultMap["creationTime"] = jsonDocument.object()["country_name"].toVariant();
                        resultMap["city"] = jsonDocument.object()["city"].toVariant();
                        resultMap["countryCode"] = jsonDocument.object()["country_code"].toVariant();
                        resultMap["creationTime"] = QDateTime::currentDateTimeUtc().toTime_t();

                        function(host, resultMap);
                    }
                }
            }

            reply->manager()->deleteLater();
            reply->deleteLater();
        });

        manager->get(QNetworkRequest(QUrl("https://api.hostip.info/get_json.php?ip=" + host)));
    }
}

auto Nedrysoft::HostIPGeoIPProvider::HostIPGeoIPProvider::lookup(const QString host) -> void {
    lookup(host, [=](const QString &hostAddress, const QVariantMap &result) {
        Q_EMIT this->result(hostAddress, result);
    });
}

