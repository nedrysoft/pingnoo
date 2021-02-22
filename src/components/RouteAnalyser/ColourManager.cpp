/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/12/2020.
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

#include "ColourManager.h"

#include "ThemeSupport.h"

constexpr auto LightIdealColour = qRgb(0xdc, 0xe5, 0xf4);
constexpr auto LightWarningColour = qRgb(0xfc, 0xef, 0xd7);
constexpr auto LightCriticalColour = qRgb(0xf9, 0xd8, 0xd3);

constexpr auto DarkIdealColour = qRgb(0x92, 0xC8, 0x9F);
constexpr auto DarkWarningColour = qRgb(0xe9, 0xc4, 0x6a);
constexpr auto DarkCriticalColour = qRgb(0xe7, 0x6f, 0x51);

auto Nedrysoft::RouteAnalyser::ColourManager::getIdealColour() -> QRgb {
    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        return DarkIdealColour;
    }

    return LightIdealColour;
}

auto Nedrysoft::RouteAnalyser::ColourManager::getWarningColour() -> QRgb {
    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        return DarkWarningColour;
    }

    return LightWarningColour;
}

auto Nedrysoft::RouteAnalyser::ColourManager::getCriticalColour() -> QRgb {
    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        return DarkCriticalColour;
    }

    return LightCriticalColour;
}
