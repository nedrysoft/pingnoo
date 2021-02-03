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

#include "RegExHostMasker.h"

#include "RegExHostMaskerComponent.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

Nedrysoft::RegExHostMasker::RegExHostMasker::RegExHostMasker() {
    QStringList configPaths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);

    if (configPaths.count()) {
        QString configurationFilename = configPaths.at(0)+"/Nedrysoft/pingnoo/Components/RegExHostMasker.json";

        auto file = QFile(configurationFilename);

        if (file.open(QFile::ReadOnly)) {
            auto jsonDocument = QJsonDocument::fromJson(file.readAll());

            if (jsonDocument.isObject()) {
                loadConfiguration(jsonDocument.object());
            }
        }
    }
}

auto Nedrysoft::RegExHostMasker::RegExHostMasker::applyMask(
        int hop,
        const QString &hostName,
        const QString &hostAddress,
        QString &maskedHostName,
        QString &maskedHostAddress ) -> bool {

    Q_UNUSED(hop)

    auto expressionMatch = QRegularExpressionMatch();
    auto tokenExpression = QRegularExpression(R"(\[\w+\])");
    auto tokenList = QList<QString>();
    auto searchList = QList<unsigned int>() << static_cast<int>(MatchFlags::MatchHost) << static_cast<int>(MatchFlags::MatchAddress);
    QString *outputString = nullptr;
    bool returnValue = false;

    maskedHostName = hostName;
    maskedHostAddress = hostAddress;

    for (auto matchFlag : searchList) {
        for (const auto &maskItem : m_maskList) {
            if (!( maskItem.m_matchFlags & matchFlag )) {
                continue;
            }

            auto matchExpression = QRegularExpression(maskItem.m_matchExpression);

            if (static_cast<MatchFlags>(matchFlag) == MatchFlags::MatchHost) {
                expressionMatch = matchExpression.match(hostName);
            } else {
                if (static_cast<MatchFlags>(matchFlag) == MatchFlags::MatchAddress) {
                    expressionMatch = matchExpression.match(hostAddress);
                } else {
                    continue;
                }
            }

            if (maskItem.m_matchFlags & static_cast<int>(MatchFlags::MaskHost)) {
                outputString = &maskedHostName;
            } else {
                if (maskItem.m_matchFlags & static_cast<int>(MatchFlags::MaskAddress)) {
                    outputString = &maskedHostAddress;
                } else {
                    continue;
                }
            }

            if (expressionMatch.hasMatch()) {
                auto tokenIterator = tokenExpression.globalMatch(maskItem.m_replacementString);

                if (!tokenIterator.hasNext()) {
                    *outputString = maskItem.m_replacementString;

                    returnValue = true;

                    continue;
                }

                tokenList.clear();

                while (tokenIterator.hasNext()) {
                    auto match = tokenIterator.next();

                    if (match.hasMatch()) {
                        tokenList.append(match.capturedTexts());
                    }
                }

                *outputString = maskItem.m_replacementString;

                for (const auto &token : tokenList) {
                    auto searchToken = QString(token).replace(QRegularExpression(R"((\[|\]))"), "");

                    ( *outputString ).replace(token, expressionMatch.captured(searchToken));
                }

                returnValue = true;
            }
        }
    }

    return returnValue;
}

auto Nedrysoft::RegExHostMasker::RegExHostMasker::mask(
        int hop,
        const QString &hostName,
        const QString &hostAddress,
        QString &maskedHostName, QString &maskedHostAddress ) -> bool {

    return applyMask(hop, hostName, hostAddress, maskedHostName, maskedHostAddress);
}

auto Nedrysoft::RegExHostMasker::RegExHostMasker::add(
        unsigned int matchFlags,
        const QString &description,
        const QString &matchExpression,
        const QString &replacementString,
        const QString &hopString ) -> void {

    Nedrysoft::RegExHostMasker::RegExHostMaskerItem item;

    item.m_matchFlags = matchFlags;
    item.m_matchExpression = matchExpression;
    item.m_replacementString = std::move(replacementString);
    item.m_hopString = hopString;
    item.m_description = description;

    m_maskList.append(item);
}

auto Nedrysoft::RegExHostMasker::RegExHostMasker::saveConfiguration() -> QJsonObject {
    auto rootObject = QJsonObject();
    auto itemArray = QJsonArray();

    rootObject.insert("id", this->metaObject()->className());

    for (const auto &item : m_maskList) {
        QJsonObject object;

        object.insert("matchFlags", static_cast<int>(item.m_matchFlags));
        object.insert("matchExpression", item.m_matchExpression);
        object.insert("matchReplacementString", item.m_replacementString);
        object.insert("matchHopString", item.m_hopString);
        object.insert("description", item.m_description);

        itemArray.append(object);
    }

    rootObject.insert("matchItems", itemArray);

    return rootObject;
}

auto Nedrysoft::RegExHostMasker::RegExHostMasker::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    if (configuration["id"] != this->metaObject()->className()) {
        return false;
    }

    m_maskList.clear();

    auto array = configuration["matchItems"].toArray();

    for (auto v : array) {
        auto item = v.toObject();

        add(item["matchFlags"].toVariant().toUInt(),
            item["description"].toString(),
            item["matchExpression"].toString(),
            item["matchReplacementString"].toString(),
            item["matchHopString"].toString() );
    }

    return false;
}

