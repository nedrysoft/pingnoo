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

#include "AppNap.h"

FizzyAde::AppNap::AppNap::AppNap()
{
#if defined(Q_OS_MACOS)
    mac_init();
#endif
}

FizzyAde::AppNap::AppNap *FizzyAde::AppNap::AppNap::getInstance()
{
    static auto instance = new FizzyAde::AppNap::AppNap();

    return instance;
}

void FizzyAde::AppNap::AppNap::prevent(const QString &reason)
{
#if defined(Q_OS_MACOS)
    mac_prevent(reason);
#else
    Q_UNUSED(reason)
#endif
}

void FizzyAde::AppNap::AppNap::allow()
{
#if defined(Q_OS_MACOS)
    mac_allow();
#endif
}
