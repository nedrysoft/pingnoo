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

#import "MacHelper/ToolbarDelegate.h"

#include "MacHelper/MacToolbarItem.h"

constexpr auto IconSize = 256;

@implementation ToolbarDelegate

- (NSToolbarItem *) toolbar:(NSToolbar *) toolbar
      itemForItemIdentifier:(NSString *) itemIdentifier
  willBeInsertedIntoToolbar:(BOOL) flag  {
    Nedrysoft::SettingsDialog::MacToolbarItem *foundItem = nil;

    for (auto toolbarItem : m_items) {
        if (toolbarItem->identifier()==QString::fromNSString(itemIdentifier)) {
            foundItem = toolbarItem;

            break;
        }
    }

    assert(foundItem!=nil);

    NSToolbarItem *item = [[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier];

    foundItem->setToolbarItem(item);

    auto pixmap = foundItem->icon().pixmap(IconSize, IconSize);

    NSImage *image = [self imageFromPixmap: pixmap];

    item.label = foundItem->identifier().toNSString();
    item.image = image;
    item.target = self;
    item.action = @selector(itemSelected:);
    item.enabled = true;

    [image release];

    return item;
}

- (void) itemSelected:(NSToolbarItem *) toolbarItem {
    for (auto item : m_items) {
        if (item->item()==toolbarItem) {
            Q_EMIT item->activated();

            return;
        }
    }
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar *) toolbar {
    return [self identifiers];
}

- (NSArray *) toolbarDefaultItemIdentifiers:(NSToolbar *) toolbar {
    return [self identifiers];
}

- (NSArray *) toolbarSelectableItemIdentifiers:(NSToolbar *) toolbar {
    return [self identifiers];
}

- (void) toolbarWillAddItem:(NSNotification *) notification  {
}

- (void) toolbarDidRemoveItem:(NSNotification *) notification {
}

- (NSImage *) imageFromPixmap:(const QPixmap &) pixmap {
    NSImage *image = [[NSImage alloc] initWithCGImage: pixmap.toImage().toCGImage()
                                                 size:CGSizeMake(IconSize, IconSize)];

    return image;
}

- (void) setItems:(QList<Nedrysoft::SettingsDialog::MacToolbarItem *>) items {
    m_items = items;
}

- (NSArray *) identifiers {
    NSMutableArray *array = [[NSMutableArray alloc] init];

    for (auto toolbarItem : m_items) {
        [array addObject: toolbarItem->identifier().toNSString()];
    }

    return array;
}

@end