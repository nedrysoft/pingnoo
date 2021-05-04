/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 03/05/2021.
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

#ifndef NEDRYSOFT_POPOVERWIDGET_H
#define NEDRYSOFT_POPOVERWIDGET_H

#include <QWidget>

#import <Foundation/Foundation.h>

@interface StatusBarHelper : NSObject {
    QWidget *m_contentWidget;
    NSPopover *m_entryPopover;
    NSView *m_nativeView;
    NSViewController *m_viewController;
}

- (void) statusBarItemClicked:(NSStatusBarButton *) sender;
- (void) setContentWidget:(QWidget *) contentWidget;
- (void) receivePopoverClosedNotification:(NSNotification *) notification;

@end

#endif //NEDRYSOFT_POPOVERWIDGET_H
