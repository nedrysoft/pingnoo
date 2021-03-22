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

#include "PingCommandPingTarget.h"

#include "PingCommandPingEngine.h"

#include <QElapsedTimer>
#include <QHostAddress>
#include <QProcess>
#include <QRegularExpression>
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QThread>
#else
#include <QTimer>
#include <thread>
#endif
#include <cassert>

constexpr auto replyTimeout = 3;
constexpr auto nanosecondsInMillisecond = 1.0e6;
constexpr auto packetLostRegularExpression = R"(100% packet loss)";
constexpr auto ttlExceededRegularExpression = R"(From\ (?<ip>[\d\.]*)\ .*exceeded)";

Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::PingCommandPingTarget(
        Nedrysoft::PingCommandPingEngine::PingCommandPingEngine *engine,
        QHostAddress hostAddress,
        int ttl) :
            m_userdata(nullptr),
            m_quitThread(false),
            m_engine(engine),
            m_ttl(ttl),
            m_hostAddress(hostAddress) {

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    m_workerThread = QThread::create([=]() {
#else
    m_workerThread = new std::thread([=]() {
#endif
        auto pingArguments = QStringList() <<
                "-W" << QString("%1").arg(replyTimeout) <<
                "-D" <<
                "-c" << "1" <<
                "-t" << QString("%1").arg(ttl) <<
                m_hostAddress.toString();

        int sampleNumber = 0;

        while(!m_quitThread) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
            QThread *pingThread = QThread::create([sampleNumber, pingArguments, engine, pingThread, this]() {
#else
            std::thread *pingThread = new std::thread([sampleNumber, pingArguments, engine, pingThread, this]() {
#endif
                QProcess pingProcess;
                qint64 started, finished;
                QElapsedTimer timer;

                std::chrono::system_clock::time_point epoch;

               // pingProcess = new QProcess();

                pingProcess.start("ping", pingArguments);

                pingProcess.waitForStarted();

                started = timer.nsecsElapsed();

                epoch = std::chrono::system_clock::now();

                pingProcess.waitForFinished();

                finished = timer.nsecsElapsed();

                auto roundTripTime = static_cast<double>(finished - started) / nanosecondsInMillisecond;

                auto commandOutput = pingProcess.readAll();

                QRegularExpression ttlExceededRegEx(ttlExceededRegularExpression);
                QRegularExpression packetLostRegEx(packetLostRegularExpression);

                if (pingProcess.exitCode() == 0) {
                    auto pingResult = Nedrysoft::Core::PingResult(
                            sampleNumber,
                            Nedrysoft::Core::PingResult::ResultCode::Ok,
                            m_hostAddress,
                            epoch,
                            std::chrono::duration<double, std::milli>(roundTripTime),
                            this);

                    engine->emitResult(pingResult);
                } else {
                    auto ttlExceededMatch = ttlExceededRegEx.match(commandOutput);
                    auto packetLostMatch = packetLostRegEx.match(commandOutput);

                    if (ttlExceededMatch.hasMatch()) {
                        auto pingResult = Nedrysoft::Core::PingResult(
                                sampleNumber,
                                Nedrysoft::Core::PingResult::ResultCode::TimeExceeded,
                                QHostAddress(ttlExceededMatch.captured("ip")),
                                epoch,
                                std::chrono::duration<double, std::milli>(roundTripTime),
                                this);

                        engine->emitResult(pingResult);
                    } else if (packetLostMatch.hasMatch()) {
                        auto pingResult = Nedrysoft::Core::PingResult(
                                sampleNumber,
                                Nedrysoft::Core::PingResult::ResultCode::NoReply,
                                QHostAddress(packetLostMatch.captured("ip")),
                                epoch,
                                std::chrono::duration<double, std::milli>(roundTripTime),
                                this);

                        engine->emitResult(pingResult);
                    } else {
                        // some other error
                    }
                }
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
                pingThread->deleteLater();
#else
                QTimer::singleShot(0, [=]() {
                    delete pingThread;
                });
#endif
            });
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
            QObject::connect(pingThread, &QThread::finished, pingThread, &QThread::deleteLater);

            pingThread->start();

            QThread::msleep(engine->interval().count());
#else
            std::this_thread::sleep_for(engine->interval());
#endif
            sampleNumber++;
        }
    });

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    m_workerThread->start();
#endif
}

Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::~PingCommandPingTarget() {

}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::setHostAddress(QHostAddress hostAddress) -> void {
    m_hostAddress = hostAddress;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::hostAddress() -> QHostAddress {
    return m_hostAddress;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::engine() -> Nedrysoft::Core::IPingEngine * {
    return m_engine;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::saveConfiguration() -> QJsonObject {
    return QJsonObject();
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::loadConfiguration(QJsonObject configuration) -> bool {
    Q_UNUSED(configuration)

    return false;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::ttl() -> uint16_t {
    return m_ttl;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::userData() -> void * {
    return m_userdata;
}

auto Nedrysoft::PingCommandPingEngine::PingCommandPingTarget::setUserData(void *data) -> void {
    m_userdata = data;
}

