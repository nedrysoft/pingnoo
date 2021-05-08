/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft MacHelper library.
 *
 * Created by Adrian Carpenter on 04/05/2021.
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

#include "MacHelper/StatusBarHelper.h"

constexpr auto StatusBarIconSize = 20;

@implementation StatusBarHelper

- (void) statusBarItemClicked:(NSStatusBarButton *) sender {
    Q_EMIT m_menuBarIcon->clicked();
}

- (id) initWithMenuBarIcon:(Nedrysoft::MacHelper::MenuBarIcon *) menuBarIcon {
    self = [super init];

    if (!self) {
        return nil;
    }

    NSStatusBar *systemStatusBar = [NSStatusBar systemStatusBar];

    m_menuBarIcon = menuBarIcon;

    m_statusBarItem = [systemStatusBar statusItemWithLength:NSSquareStatusItemLength];

    CGImageRef imageRef;

    if (menuBarIcon->pixmap().width() != StatusBarIconSize) {
        imageRef = menuBarIcon->pixmap().scaled(StatusBarIconSize, StatusBarIconSize).toImage().toCGImage();
    } else {
        imageRef = menuBarIcon->pixmap().toImage().toCGImage();
    }

    m_button = [m_statusBarItem button];

    NSImage *nativeImage = [[NSImage alloc] initWithCGImage:imageRef
                                                       size:NSMakeSize(StatusBarIconSize, StatusBarIconSize)];

    [m_button setImage:nativeImage];

    [m_button setTarget:self];
    [m_button setAction:@selector(statusBarItemClicked:)];

    [nativeImage release];

    return self;
}

- (NSRect) buttonRect {
    return [m_button visibleRect];
}

- (NSView *) button {
    return m_button;
}

- (void) updatePixmap {
    CGImageRef imageRef;

    if (m_menuBarIcon->pixmap().width() != StatusBarIconSize) {
        imageRef = m_menuBarIcon->pixmap().scaled(StatusBarIconSize, StatusBarIconSize).toImage().toCGImage();
    } else {
        imageRef = m_menuBarIcon->pixmap().toImage().toCGImage();
    }

    NSImage *nativeImage = [[NSImage alloc] initWithCGImage:imageRef
                                                       size:NSMakeSize(StatusBarIconSize, StatusBarIconSize)];

    [m_button setImage:nativeImage];

    [nativeImage release];
}

- (void) setVisible:(bool) visible {
    [m_button setHidden: !visible];
}

@end
