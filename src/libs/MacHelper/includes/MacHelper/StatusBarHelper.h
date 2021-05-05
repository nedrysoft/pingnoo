/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of the Nedrysoft MacHelper library.
 *
 * An open-source cross-platform traceroute analyser.
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

#ifndef NEDRYSOFT_STATUSBARHELPER_H
#define NEDRYSOFT_STATUSBARHELPER_H

#include <QWidget>
#include <QPixmap>

#import <AppKit/AppKit.h>
#include "MacHelper/MenuBarIcon.h"

@interface StatusBarHelper : NSObject {
    NSStatusItem *m_statusBarItem;
    NSStatusBarButton *m_button;
    Nedrysoft::MacHelper::MenuBarIcon *m_menuBarIcon;
}

/**
 * @brief       Called when the menu bar icons button has been clicked.
 *
 * @param[in]   sender the button that was the source of the click.
 */
- (void) statusBarItemClicked:(NSStatusBarButton *) sender;

/**
 * @brief       Initialises a StatusBarHelper for a given menu bar icon.
 *
 * @param[in]   menuBarIcon the menu bar icon.
 *
 * @returns     the StatusBarHelper instance.
 */
- (id) initWithMenuBarIcon:(Nedrysoft::MacHelper::MenuBarIcon *) menuBarIcon;

/**
 * @brief       Returns the rectangle of the menu bar icons button.
 *
 * @returns     the menu bar buttons rectangle.
 */
- (NSRect) buttonRect;

/**
 * @brief       Returns the NSView of the button.
 *
 * @returns     the button.
 */
- (NSView *) button;

/**
 * @brief       Updates the pixmap.
 *
 * @notes       retrieves the pixmap from the MenuBarIcon that was associated during construction and sets it
 *              to be the current icon.
 */
- (void) updatePixmap;

/**
 * @brief       Displays or hides the icon.
 *
 * @param[in]   visible true if the icon is to be shown; otherwise false.
 */
- (void) setVisible:(bool) visible;

@end

#endif // NEDRYSOFT_STATUSBARHELPER_H
