/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft SettingsDialog. (https://github.com/nedrysoft/SettingsDialog)
 *
 * A cross-platform settings dialog
 *
 * Created by Adrian Carpenter on 07/05/2020.
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

#include "MacHelper/MacToolbar.h"

#include "MacHelper/MacToolbarItem.h"

#include <QWidget>
#include <QWindow>

#import <AppKit/AppKit.h>
#import "MacHelper/ToolbarDelegate.h"

constexpr auto nativeWindowHandle(QWidget *widget) {
    widget->winId();
    return widget->windowHandle();
}

Nedrysoft::SettingsDialog::MacToolbar::MacToolbar() :
        m_toolbar(nullptr),
        m_toolbarDelegate(nullptr),
        m_parentWindow(nullptr),
        m_isPreferences(false),
        m_window(nullptr) {

}

Nedrysoft::SettingsDialog::MacToolbar::~MacToolbar() {
    if (m_toolbarDelegate) {
        [m_toolbarDelegate release];
    }
}

auto Nedrysoft::SettingsDialog::MacToolbar::addItem(
        const QIcon &icon,
        const QString &identifier,
        const QString &label,
        const QString &paletteLabel) -> MacToolbarItem * {

    auto toolbarItem = new MacToolbarItem(icon, identifier, label, paletteLabel);

    m_items.append(toolbarItem);

    return toolbarItem;
}

auto Nedrysoft::SettingsDialog::MacToolbar::enablePreferencesToolbar() -> void {
    if (!m_window) {
        m_isPreferences = true;

        return;
    }

    if (m_isPreferences) {
        if (@available(macOS 11, *)) {
            if ([m_window respondsToSelector:@selector(setToolbarStyle:)]) {
                [m_window setToolbarStyle:NSWindowToolbarStylePreference];
            }
        }
    }
}

auto Nedrysoft::SettingsDialog::MacToolbar::attachToWindow(QWidget *parent) -> void {
    m_toolbar = [[NSToolbar alloc] init];

    parent->winId();

    /**
     * this line is strange, there appears to be a bug in CLion that flags the qobject_cast as if it
     * is invisible, this hack ensures that the object is a qobject, but uses the dynamic cast to
     * silence the issue in CLion.
     */

    m_parentWindow = dynamic_cast<QWindow *>(qobject_cast<QWindow *>(parent->windowHandle()));

    assert(m_parentWindow!=NULL);

    NSView *view = reinterpret_cast<NSView *>(parent->winId());

    NSWindow *window = [view window];

    m_toolbarDelegate = [[ToolbarDelegate alloc] init];

    [m_toolbarDelegate setItems: m_items];

    [m_toolbar setDelegate: m_toolbarDelegate];

    [window setToolbar: m_toolbar];

    [m_toolbar release];

    m_window = window;

    if (m_isPreferences) {
        enablePreferencesToolbar();
    }

    if (!m_items.isEmpty()) {
        [m_toolbar setSelectedItemIdentifier:m_items.at(0)->identifier().toNSString()];
    }
}

auto Nedrysoft::SettingsDialog::MacToolbar::items() -> QList<Nedrysoft::SettingsDialog::MacToolbarItem *> {
    return m_items;
}

