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

#include "RouteEngine.h"

#include <IPingEngine>
#include <IPingEngineFactory>
#include <IPingTarget>
#include "RouteEngineWorker.h"

#include <QThread>
#include <QTimer>

#include <cassert>

using namespace std::chrono_literals;

Nedrysoft::RouteEngine::RouteEngine::RouteEngine() :
        m_routeWorkerThread(nullptr),
        m_routeWorker(nullptr) {

}

auto Nedrysoft::RouteEngine::RouteEngine::findRoute(
        Nedrysoft::Core::IPingEngineFactory *engineFactory,
        QString host,
        Nedrysoft::Core::IPVersion ipVersion) -> void {

    m_routeWorker = new Nedrysoft::RouteEngine::RouteEngineWorker(host, engineFactory, ipVersion);

    m_routeWorkerThread = new QThread();

    m_routeWorker->moveToThread(m_routeWorkerThread);

    connect(m_routeWorkerThread,
            &QThread::started,
            m_routeWorker,
            &Nedrysoft::RouteEngine::RouteEngineWorker::doWork );

    connect(m_routeWorkerThread,
            &QThread::finished,
            [=]() {
                m_routeWorkerThread->deleteLater();
    });

    connect(m_routeWorker,
            &Nedrysoft::RouteEngine::RouteEngineWorker::result,
            this,
            &Nedrysoft::RouteEngine::RouteEngine::result );

    m_routeWorkerThread->start();
}
