/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 17/12/2020.
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

#include "Utils.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

constexpr auto millisecondsInSecond = 1000.0;
constexpr auto secondsInMinute = 60.0;
constexpr auto secondsInInHour = secondsInMinute*60.0;
constexpr auto secondsInDay = secondsInInHour*24.0;

constexpr auto timeIntervalRegularExpression = R"(^\s*(?<number>(\d*(\.\d+|\d*)))(\s*(?<units>ms|s|m|h|d)\s*)?$)";

constexpr auto ipAddressRegularExpression =
        R"(^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$)";

constexpr auto hostNameRegularExpression =
        R"(^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$)";

bool Nedrysoft::Utils::parseIntervalString(QString intervalString) {
    double tempValue;

    return parseIntervalString(intervalString, tempValue);
}

bool Nedrysoft::Utils::parseIntervalString(QString intervalString, double &intervalValue) {
    bool numberOk;
    auto match = QRegularExpression(timeIntervalRegularExpression,
                                    QRegularExpression::CaseInsensitiveOption ).match(intervalString);

    if (!match.hasMatch()) {
        return false;
    }

    auto intervalTime = match.captured("number").toDouble(&numberOk);

    if (!numberOk) {
        return false;
    }

    if (!match.captured("units").isNull()) {
        auto unitString = match.captured("units").toLower();

        if (unitString=="ms") {
            intervalTime = intervalTime/millisecondsInSecond;
        } else if (unitString=="s") {
            // nothing to do!
        } else if (unitString=="m") {
            intervalTime = intervalTime * secondsInMinute;
        } else if (unitString=="h") {
            intervalTime = intervalTime * secondsInInHour;
        } else if (unitString=="d") {
            intervalTime = intervalTime * secondsInDay;
        } else {
            return false;
        }
    }

    intervalValue = intervalTime;

    return true;
}

bool Nedrysoft::Utils::checkHostValid(const QString &host) {
    auto hostMatch = QRegularExpression(
            hostNameRegularExpression,
            QRegularExpression::CaseInsensitiveOption).match(host);

    auto ipMatch = QRegularExpression(
            ipAddressRegularExpression,
            QRegularExpression::CaseInsensitiveOption).match(host);

    if ((ipMatch.hasMatch()==false) && (hostMatch.hasMatch()==false)) {
        return false;
    }

    return true;
}

