/*
 * Copyright (C) 2021 Adrian Carpenter
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

#include "MacHelper/MenuBarIcon.h"
#include "MacHelper/StatusBarHelper.h"

Nedrysoft::MacHelper::MenuBarIcon::MenuBarIcon(QPixmap pixmap) {
    m_pixmap = pixmap;

    m_statusBarHelper = [[StatusBarHelper alloc] initWithMenuBarIcon: this];
}

auto Nedrysoft::MacHelper::MenuBarIcon::pixmap() -> QPixmap {
    return m_pixmap;
}

auto Nedrysoft::MacHelper::MenuBarIcon::buttonRect() -> QRect {
    return QRectF::fromCGRect(NSRectToCGRect([m_statusBarHelper buttonRect])).toRect();
}

auto Nedrysoft::MacHelper::MenuBarIcon::button() -> NSView * {
    return [m_statusBarHelper button];
}

auto Nedrysoft::MacHelper::MenuBarIcon::setPixmap(QPixmap pixmap) -> void {
    m_pixmap = pixmap;

    [m_statusBarHelper updatePixmap];
}

auto Nedrysoft::MacHelper::MenuBarIcon::show() -> void {
    [m_statusBarHelper setVisible: true];
}

auto Nedrysoft::MacHelper::MenuBarIcon::hide() -> void {
    [m_statusBarHelper setVisible: false];
}