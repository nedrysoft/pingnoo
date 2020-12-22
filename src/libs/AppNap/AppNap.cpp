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

Nedrysoft::AppNap::AppNap::AppNap() {
#if defined(Q_OS_MACOS)
    mac_init();
#endif
}

auto Nedrysoft::AppNap::AppNap::getInstance() -> Nedrysoft::AppNap::AppNap * {
    static auto instance = new Nedrysoft::AppNap::AppNap();

    return instance;
}

void Nedrysoft::AppNap::AppNap::prevent(const QString &reason) {
#if defined(Q_OS_MACOS)
    mac_prevent(reason);
#else
    Q_UNUSED(reason)
#endif
}

void Nedrysoft::AppNap::AppNap::allow() {
#if defined(Q_OS_MACOS)
    mac_allow();
#endif
}
