/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft Ribbon Bar. (https://github.com/nedrysoft/qt-ribbon)
 *
 * A cross-platform ribbon bar for Qt applications.
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

#include "ThemeSupport.h"

#include <QApplication>
#include <QStyle>

#if defined(Q_OS_MACOS)
#import <AppKit/NSAppearance.h>
#import <AppKit/NSColor.h>
#endif

Nedrysoft::Utils::ThemeSupport::ThemeSupport() {
    connect(qobject_cast<QApplication *>(QCoreApplication::instance()), &QApplication::paletteChanged, [=] (const QPalette &) {
        Q_EMIT themeChanged(Nedrysoft::Utils::ThemeSupport::isDarkMode());
    });
}

auto Nedrysoft::Utils::ThemeSupport::isDarkMode() -> bool{
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

        return [basicAppearance isEqualToString:NSAppearanceNameDarkAqua]==YES;
    }

    return false;
}

auto Nedrysoft::Utils::ThemeSupport::getColor(const QRgb colourPair[]) -> QColor {
    return QColor(colourPair[isDarkMode() ? 1 : 0]);
}

auto Nedrysoft::Utils::ThemeSupport::getHighlightedBackground() -> QColor {
#if defined(Q_OS_MACOS)
    CGColorRef a = [NSColor systemBlueColor].CGColor;

    const CGFloat *color = CGColorGetComponents(a);

    return QColor::fromRgbF(color[0], color[1], color[2]);
#else
    return qobject_cast<QApplication *>(QCoreApplication::instance())->style()->standardPalette().color(QPalette::Highlight);
#endif
}
