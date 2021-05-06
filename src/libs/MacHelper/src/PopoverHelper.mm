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

#include "MacHelper/PopoverHelper.h"

@implementation PopoverHelper

- (void) show:(QWidget *) contentWidget
     withView:(NSView *) view
   sourceRect:(NSRect) rect
         size:(NSSize) size
preferredEdge:(Nedrysoft::MacHelper::Popover::Edge) preferredEdge {

    m_contentWidget = contentWidget;

    m_viewController = [[NSViewController alloc] init];

    // create the popover

    m_popover = [[NSPopover alloc] init];

    [m_popover setContentSize: size];
    [m_popover setBehavior: NSPopoverBehaviorTransient];
    [m_popover setAnimates: YES];
    [m_popover setContentViewController: m_viewController];

    auto contentWidgetView = reinterpret_cast<NSView *>(m_contentWidget->winId());

    m_nativeView = [[NSView alloc] init];

    [m_nativeView addSubview: contentWidgetView];

    m_contentWidget->show();

    [m_viewController setView: contentWidgetView];

    // show the popover in the correct place

    NSRectEdge edge;

    if (preferredEdge==Nedrysoft::MacHelper::Popover::Edge::MinXEdge) {
        edge = NSRectEdgeMinX;
    } else if (preferredEdge==Nedrysoft::MacHelper::Popover::Edge::MaxXEdge) {
        edge = NSRectEdgeMaxX;
    } else if (preferredEdge==Nedrysoft::MacHelper::Popover::Edge::MinYEdge) {
        edge = NSRectEdgeMinY;
    } else {
        edge = NSRectEdgeMaxY;
    }

    [m_popover showRelativeToRect: rect
                           ofView: view
                    preferredEdge: edge];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receivePopoverClosedNotification:)
                                                 name:@"NSPopoverDidCloseNotification"
                                               object:m_popover];
}

- (void) receivePopoverClosedNotification:(NSNotification *) notification {
    delete m_contentWidget;
}

@end