/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo
 *
 * Created by Adrian Carpenter on 05/12/2020.
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

constexpr auto LightLowColour = qRgb(229, 240, 220);
constexpr auto LightMidColour = qRgb(252, 239, 215);
constexpr auto LightHighColour = qRgb(249, 216, 211);

constexpr auto DarkLowColour = qRgb(0x92,0xC8,0x9F);
constexpr auto DarkMidColour = qRgb(0xe9, 0xc4, 0x6a);
constexpr auto DarkHighColour = qRgb(0xe7, 0x6f, 0x51);

QRgb Nedrysoft::RouteAnalyser::ColourManager::getMinColour() {
    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        return DarkLowColour;
    }

    return LightLowColour;
}

QRgb Nedrysoft::RouteAnalyser::ColourManager::getMidColour() {
    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        return DarkMidColour;
    }

    return LightMidColour;
}

QRgb Nedrysoft::RouteAnalyser::ColourManager::getMaxColour() {
    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        return DarkHighColour;
    }

    return LightHighColour;
}
