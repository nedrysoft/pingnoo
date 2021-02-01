/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 22/01/2020.
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

#include "CommandRouteEngine.h"

#include <QElapsedTimer>
#include <QObject>
#include <QProcess>
#include <QRegularExpression>
#include <QThread>

using namespace std::chrono_literals;

constexpr auto nanosecondsInMillisecond = 1.0e6;

constexpr auto DefaultTerminateThreadTimeout = 5s;

Nedrysoft::CommandRouteEngine::CommandRouteEngine::CommandRouteEngine() :
        m_worker(nullptr),
        m_workerThread(nullptr) {

}

Nedrysoft::CommandRouteEngine::CommandRouteEngine::~CommandRouteEngine() {
}

auto Nedrysoft::CommandRouteEngine::CommandRouteEngine::findRoute(QString host, Nedrysoft::Core::IPVersion ipVersion) -> void {
    m_workerThread = QThread::create([=]() {
        Nedrysoft::Core::RouteList routeList;

        for (auto ttl=1;ttl<64;ttl++) {
            QProcess pingProcess;

            pingProcess.start("ping", QStringList() <<  "-w" << "1" << "-D" << "-c" << "1" << "-t" << QString("%1").arg(ttl) << host);

            if (pingProcess.waitForStarted()) {
                if (pingProcess.waitForFinished()) {
                    auto commandOutput = pingProcess.readAll();

                    constexpr auto exceededRegex = R"(From\ (?<ip>[\d\.]*)\ .*exceeded)";

                    QRegularExpression re(exceededRegex);

                    if (pingProcess.exitCode() == 0) {
                        Q_EMIT result(QHostAddress(host), routeList << QHostAddress(host));

                        return;
                    } else {
                        auto match = re.match(commandOutput);

                        if (match.hasMatch()) {
                            routeList << QHostAddress(match.captured("ip"));
                        } else {
                            routeList << QHostAddress();
                        }
                    }
                }
            }
        }
    });

    m_workerThread->start();
}
