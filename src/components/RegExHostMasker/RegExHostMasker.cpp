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

#include "RegExHostMasker.h"
#include "RegExHostMaskerComponent.h"
#include <QRegularExpressionMatch>
#include <QJsonArray>
#include <QDebug>

bool FizzyAde::RegExHostMasker::RegExHostMasker::applyMask(int hop, const QString &hostName, const QString &hostAddress, QString &maskedHostName, QString &maskedHostAddress)
{
    Q_UNUSED(hop)
    auto expressionMatch = QRegularExpressionMatch();
    auto tokenExpression = QRegularExpression(R"(\[\w+\])");
    auto tokenList = QList<QString>();
    auto searchList = QList<unsigned int>() << MatchHost << MatchAddress;
    QString *outputString = nullptr;
    bool returnValue = false;

    for(auto matchFlag : searchList) {
        for(const auto &maskItem : m_maskList) {
            if (!(maskItem.m_matchFlags & matchFlag)) {
                continue;
            }

            auto matchExpression = QRegularExpression(maskItem.m_matchExpression);

            if (matchFlag==MatchHost) {
                expressionMatch = matchExpression.match(hostName);
            }
            else {
                if (matchFlag==MatchAddress) {
                    expressionMatch = matchExpression.match(hostAddress);
                } else {
                    continue;
                }
            }

            if (maskItem.m_matchFlags & MaskHost) {
                outputString = &maskedHostName;
            }
            else {
                if (maskItem.m_matchFlags & MaskAddress) {
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

                for(const auto &token : tokenList) {
                    auto searchToken = QString(token).replace(QRegularExpression(R"((\[|\]))"), "");

                    (*outputString).replace(token, expressionMatch.captured(searchToken));
                }

                returnValue = true;
            }
        }
    }

    return returnValue;
}

bool FizzyAde::RegExHostMasker::RegExHostMasker::mask(int hop, const QString &hostName, const QString &hostAddress, QString &maskedHostName, QString &maskedHostAddress)
{
    return applyMask(hop, hostName, hostAddress, maskedHostName, maskedHostAddress);
}

void FizzyAde::RegExHostMasker::RegExHostMasker::add(unsigned int matchFlags, QString matchExpression, QString replacementString, QString hopString)
{
    FizzyAde::RegExHostMasker::RegExHostMaskerItem item;

    item.m_matchFlags = matchFlags;
    item.m_matchExpression = std::move(matchExpression);
    item.m_replacementString = std::move(replacementString);
    item.m_hopString = std::move(hopString);

    m_maskList.append(item);
}

QJsonObject FizzyAde::RegExHostMasker::RegExHostMasker::saveConfiguration()
{
    auto rootObject = QJsonObject();
    auto itemArray = QJsonArray();

    rootObject.insert("id", this->metaObject()->className());

    for (const auto &item : m_maskList) {
        QJsonObject object;

        object.insert("matchFlags", static_cast<int>(item.m_matchFlags));
        object.insert("matchExpression", item.m_matchExpression);
        object.insert("matchReplacementString", item.m_replacementString);
        object.insert("matchHopString", item.m_hopString);

        itemArray.append(object);
    }

    rootObject.insert("matchItems", itemArray);

    return rootObject;
}

bool FizzyAde::RegExHostMasker::RegExHostMasker::loadConfiguration(QJsonObject configuration)
{
    Q_UNUSED(configuration)

    if (configuration["id"]!=this->metaObject()->className()) {
        return false;
    }

    auto array = configuration["matchItems"].toArray();

    for(auto v : array) {
        auto item = v.toObject();

        add(item["matchFlags"].toVariant().toUInt(), item["matchExpression"].toString(), item["matchReplacementString"].toString(), item["matchHopString"].toString());
    }

    return false;
}

