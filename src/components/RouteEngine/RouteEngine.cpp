/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
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

#include "RouteEngine.h"

#include "RouteWorker.h"

#include <QThread>

using namespace std::chrono_literals;

constexpr auto DefaultTerminateThreadTimeout = 5s;

Nedrysoft::RouteEngine::RouteEngine::RouteEngine() :
        m_worker(nullptr),
        m_workerThread(nullptr) {

}

Nedrysoft::RouteEngine::RouteEngine::~RouteEngine() {
    if (m_workerThread) {
        m_worker->m_isRunning = false;

        m_workerThread->quit();
        m_workerThread->wait(
                std::chrono::duration_cast<std::chrono::milliseconds>(DefaultTerminateThreadTimeout).count() );

        if (m_workerThread->isRunning()) {
            m_workerThread->terminate();
        }

        delete m_workerThread;

        m_workerThread = nullptr;
    }

    if (m_worker) {
        delete m_worker;

        m_worker = nullptr;
    }
}

auto Nedrysoft::RouteEngine::RouteEngine::findRoute(QString host, Nedrysoft::Core::IPVersion ipVersion) -> void {
    m_workerThread = new QThread();

    m_worker = new Nedrysoft::RouteEngine::RouteWorker(ipVersion);

    m_worker->setHost(host);

    m_worker->moveToThread(m_workerThread);

    connect(m_workerThread, &QThread::started, m_worker, &Nedrysoft::RouteEngine::RouteWorker::doWork);

    connect(m_worker, &Nedrysoft::RouteEngine::RouteWorker::result, this, &Nedrysoft::Core::IRouteEngine::result);

    m_workerThread->start();
}
