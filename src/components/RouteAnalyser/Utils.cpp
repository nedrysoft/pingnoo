/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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

#include <QHostAddress>
#include <QObject>
#include <QRegularExpression>

constexpr auto MillisecondsInSecond = 1000.0;
constexpr auto SecondsInMinute = 60.0;
constexpr auto SecondsInInHour = SecondsInMinute*60.0;
constexpr auto SecondsInDay = SecondsInInHour*24.0;

constexpr auto timeIntervalRegularExpression =
        R"(^\s*(?<number>(\d*(\.\d+|\d*)))(\s*(?<units>ms|s|m|h|d|sec(s?)|second(s?)|min(s?)|minute(s?)|hour(s?)|day(s?))\s*)?$)";

constexpr auto ipAddressRegularExpression =
        R"(^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$)";

constexpr auto hostNameRegularExpression =
        R"(^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$)";

auto Nedrysoft::Utils::parseIntervalString(QString intervalString) -> bool {
    double tempValue;

    return parseIntervalString(intervalString, tempValue);
}

auto Nedrysoft::Utils::parseIntervalString(QString intervalString, double &intervalValue) -> bool {
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
            intervalTime = intervalTime/MillisecondsInSecond;
        } else if ((unitString=="s") || (unitString=="sec" ) || (unitString=="secs" ) || (unitString=="second") || (unitString=="seconds")) {
            // nothing to do!
        } else if ((unitString=="m") || (unitString=="min") || (unitString=="mins") || (unitString=="minute") || (unitString=="minutes")) {
            intervalTime = intervalTime * SecondsInMinute;
        } else if ((unitString=="h") || (unitString=="hour") || (unitString=="hours")){
            intervalTime = intervalTime * SecondsInInHour;
        } else if ((unitString=="d") || (unitString=="day") || (unitString=="days")) {
            intervalTime = intervalTime * SecondsInDay;
        } else {
            return false;
        }
    }

    intervalValue = intervalTime;

    return true;
}

auto Nedrysoft::Utils::checkHostValid(const QString &host, QAbstractSocket::NetworkLayerProtocol *protocol) -> bool {
    QHostAddress hostAddress(host);

    if (hostAddress.protocol()!=QAbstractSocket::UnknownNetworkLayerProtocol) {
        if (protocol) {
            *protocol = hostAddress.protocol();
        }

        return true;
    } else {
        auto hostMatch = QRegularExpression(
                hostNameRegularExpression,
                QRegularExpression::CaseInsensitiveOption).match(host);

        if (hostMatch.hasMatch()) {
            if (protocol) {
                *protocol = QAbstractSocket::AnyIPProtocol;
            }

            return true;
        }
    }

    if (protocol) {
        *protocol = QAbstractSocket::UnknownNetworkLayerProtocol;
    }

    return false;
}

auto Nedrysoft::Utils::intervalToString(double value) -> QString {
    if (value>=1) {
        return QString(QObject::tr("%1 s")).arg(value, 0, 'g', 4, '0');
    } else {
        return QString(QObject::tr("%1 ms")).arg(value*MillisecondsInSecond, 1, 'f', 0, '0');
    }
}
