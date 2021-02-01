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

#include "PublicIPHostMasker.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

Nedrysoft::PublicIPHostMasker::PublicIPHostMasker::PublicIPHostMasker() :
        m_eventLoop(nullptr),
        m_networkManager(nullptr) {

}

Nedrysoft::PublicIPHostMasker::PublicIPHostMasker::~PublicIPHostMasker() {
    if (m_eventLoop) {
        delete m_eventLoop;
    }

    if (m_networkManager) {
        delete m_networkManager;
    }
}

auto Nedrysoft::PublicIPHostMasker::PublicIPHostMasker::mask(
        int hop,
        const QString &hostName,
        const QString &hostAddress,
        QString &maskedHostName,
        QString &maskedHostAddress) -> bool {

    Q_UNUSED(hop)
    Q_UNUSED(hostName)
    Q_UNUSED(hostAddress)
    Q_UNUSED(maskedHostName)
    Q_UNUSED(maskedHostAddress)

    // retrieve checkip.dyndns.com

    if (m_publicIP.isNull()) {
        m_eventLoop = new QEventLoop;
        m_networkManager = new QNetworkAccessManager();
        QNetworkRequest request;

        connect(m_networkManager, &QNetworkAccessManager::finished, [=](QNetworkReply *) {
            m_eventLoop->quit();
        });

        request.setUrl(QUrl("http://checkip.dyndns.com"));

        auto reply = m_networkManager->get(request);

        m_eventLoop->exec();

        auto responseTest(reply->readAll());

        auto matchExpression = QRegularExpression(
                R"(Current IP Address: (?<ip>([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})))");

        auto expressionMatch = matchExpression.match(responseTest);

        if (expressionMatch.hasMatch()) {
            m_publicIP = expressionMatch.captured("ip");
        }
    }

    if (hostAddress == m_publicIP) {
        maskedHostName = tr("<hidden>");
        maskedHostAddress = tr("<hidden>");
    }

    return true;
}

auto Nedrysoft::PublicIPHostMasker::PublicIPHostMasker::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::PublicIPHostMasker::PublicIPHostMasker::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}

