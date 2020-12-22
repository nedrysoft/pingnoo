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

#include "AppNap.h"

#import <Foundation/Foundation.h>

void Nedrysoft::AppNap::AppNap::mac_init() {
    m_activity = nil;
    m_count = 0;
}

void Nedrysoft::AppNap::AppNap::mac_prevent(const QString &reason) {
    QMutexLocker mutexLocker(&m_mutex);

    if (m_activity == nil) {
        if ([[NSProcessInfo processInfo] respondsToSelector:@selector(beginActivityWithOptions:reason:)]) {
            m_activity = [[NSProcessInfo processInfo] beginActivityWithOptions:NSActivityUserInitiatedAllowingIdleSystemSleep reason:reason.toNSString()];
        }

        if (m_activity)
            m_count++;
    }
}

void Nedrysoft::AppNap::AppNap::mac_allow() {
    QMutexLocker mutexLocker(&m_mutex);

    if (m_count) {
        m_count--;

        if (m_count == 0) {
            if (m_activity != nil) {
                [[NSProcessInfo processInfo] endActivity:m_activity];

                m_activity = nil;
            }
        }
    }
}



