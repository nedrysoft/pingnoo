/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft Ribbon Bar. (https://github.com/nedrysoft/qt-ribbon)
 *
 * A cross-platform ribbon bar for Qt applications.
 *
 * Created by Adrian Carpenter on 31/03/2021.
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

#include "includes/ThemeSupport.h"

#include <QStyle>

#import <AppKit/NSAppearance.h>
#import <AppKit/NSColor.h>

auto Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode() -> bool {
    if (m_themeMode==Nedrysoft::ThemeSupport::ThemeMode::System) {
        if (@available(macOS 10.14, *)) {
            NSAppearance *appearance = nullptr;

            if (@available(macOS 11, *)) {
                appearance = NSAppearance.currentDrawingAppearance;
            } else if (@available(macOS 10.14, *)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
                appearance = NSAppearance.currentAppearance;
#pragma clang diagnostic pop
            }

            NSAppearanceName basicAppearance = [appearance bestMatchFromAppearancesWithNames:@[
                    NSAppearanceNameAqua,
                    NSAppearanceNameDarkAqua
            ]];

            return [basicAppearance isEqualToString:NSAppearanceNameDarkAqua] == YES;
        }

        return false;
    }

    return m_themeMode==Nedrysoft::ThemeSupport::ThemeMode::Dark;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::getHighlightedBackground() -> QColor {
    CGColorRef a = [NSColor systemBlueColor].CGColor;

    const CGFloat *color = CGColorGetComponents(a);

    return QColor::fromRgbF(color[0], color[1], color[2]);
}
