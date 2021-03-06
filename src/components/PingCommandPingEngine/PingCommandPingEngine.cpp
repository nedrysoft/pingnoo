/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 22/01/2021.
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

#include "PingCommandPingEngine.h"

#include "PingCommandPingTarget.h"

#include <QElapsedTimer>
#include <QMutex>
#include <QProcess>
#include <QRegularExpression>
#include <QThread>

constexpr auto DefaultReceiveTimeout = 1000;
constexpr auto DefaultTerminateThreadTimeout = 5000;
constexpr auto DefaultTTL = 64;

constexpr auto PacketLostRegularExpression = R"(100% packet loss)";
constexpr auto TtlExceededRegularExpression = R"(From\ (?<ip>[\d\.]*)\ .*exceeded)";

Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::PingCommandPingEngine(Nedrysoft::Core::IPVersion version) {
    Q_UNUSED(version)

}

Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::~PingCommandPingEngine() {

}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::addTarget(
        QHostAddress hostAddress ) -> Nedrysoft::RouteAnalyser::IPingTarget * {

    return addTarget(hostAddress, DefaultTTL);
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::addTarget(
        QHostAddress hostAddress,
        int ttl ) -> Nedrysoft::RouteAnalyser::IPingTarget * {

    auto newTarget = new Nedrysoft::PingCommandPingEngine::PingCommandPingTarget(
        this,
        hostAddress,
        ttl
    );

    m_pingTargets.append(newTarget);

    return newTarget;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::removeTarget(
        Nedrysoft::RouteAnalyser::IPingTarget *target ) -> bool {

    Q_UNUSED(target)

    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::start() -> bool {
    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::stop() -> bool {
    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::setInterval(int interval) -> bool {
    m_interval = interval;

    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::interval() -> int {
    return m_interval;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::setTimeout(int timeout) -> bool {
    Q_UNUSED(timeout)

    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::epoch() -> QDateTime {
    return QDateTime::currentDateTime();
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::saveConfiguration() -> QJsonObject {
   return QJsonObject();
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return true;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::emitResult(
        Nedrysoft::RouteAnalyser::PingResult pingResult) -> void {

    Q_EMIT result(pingResult);
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::targets() ->
        QList<Nedrysoft::RouteAnalyser::IPingTarget *> {

    QList<Nedrysoft::RouteAnalyser::IPingTarget *> list;

    for (auto target : m_pingTargets) {
        list.append(target);
    }

    return list;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingEngine::singleShot(
        QHostAddress hostAddress,
        int ttl,
        double timeout) -> Nedrysoft::RouteAnalyser::PingResult {

    QProcess pingProcess;
    QElapsedTimer timer;
    QDateTime epoch;

    auto pingArguments = QStringList() <<
                                       "-W" << QString("%1").arg(timeout) <<
                                       "-D" <<
                                       "-c" << "1" <<
                                       "-t" << QString("%1").arg(ttl) <<
                                       hostAddress.toString();

    pingProcess.start("ping", pingArguments);

    pingProcess.waitForStarted();

    timer.restart();

    epoch = QDateTime::currentDateTime();

    pingProcess.waitForFinished();

    auto roundTripTime = timer.elapsed()/1e9;

    auto commandOutput = pingProcess.readAll();

    QRegularExpression ttlExceededRegEx(TtlExceededRegularExpression);
    QRegularExpression packetLostRegEx(PacketLostRegularExpression);

    Nedrysoft::RouteAnalyser::PingResult pingResult;

    if (pingProcess.exitCode() == 0) {
        pingResult = Nedrysoft::RouteAnalyser::PingResult(
            0,
            Nedrysoft::RouteAnalyser::PingResult::ResultCode::Ok,
            hostAddress,
            epoch,
            roundTripTime,
            nullptr
        );

    } else {
        auto ttlExceededMatch = ttlExceededRegEx.match(commandOutput);
        auto packetLostMatch = packetLostRegEx.match(commandOutput);

        if (ttlExceededMatch.hasMatch()) {
            pingResult = Nedrysoft::RouteAnalyser::PingResult(
                0,
                Nedrysoft::RouteAnalyser::PingResult::ResultCode::TimeExceeded,
                QHostAddress(ttlExceededMatch.captured("ip")),
                epoch,
                roundTripTime,
                nullptr
            );
        } else if (packetLostMatch.hasMatch()) {
            pingResult = Nedrysoft::RouteAnalyser::PingResult(
                0,
                Nedrysoft::RouteAnalyser::PingResult::ResultCode::NoReply,
                QHostAddress(packetLostMatch.captured("ip")),
                epoch,
                roundTripTime,
                nullptr
            );
        } else {
            // some other error
        }
    }

    return pingResult;
}
