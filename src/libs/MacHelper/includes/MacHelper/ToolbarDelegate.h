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

#import <AppKit/AppKit.h>

#include "MacToolbar.h"

#include <QPixmap>

/**
 * @brief       The ToolbarDelegate object is used by the NSToolbar to populate the toolbar, as macOS allows
 *              customisation of the toolbar, items are not directly create but instead, the delegate is
 *              called to return data which macOS uses to populate the toolbar.
 */

@interface ToolbarDelegate : NSObject <NSToolbarDelegate> {
    NSToolbar *m_toolbar;
    QList<Nedrysoft::SettingsDialog::MacToolbarItem *> m_items;
}

/**
 * @brief       Called by macOS when populating the toolbar, this returns the native toolbar item.
 *
 * @param[in]   toolbar is the toolbar that will own this item.
 * @param[in]   itemIdentifier the identifier used to correlate items with instances.
 * @param[in]   flag true if the item is to be immediately inserted into the toolbar; false if it is to
 *              added to the customisation palette instead.
 *
 * @returns     the native NSToolbarItem.
 */
- (NSToolbarItem *) toolbar:(NSToolbar *) toolbar
      itemForItemIdentifier:(NSString *) itemIdentifier
  willBeInsertedIntoToolbar:(BOOL) flag;

/**
 * @brief       Returns the list of toolbar items that can be inserted into the toolbar.
 *
 * @param[in]   toolbar the toolbar.
 *
 * @returns     an array with the identifiers of the toolbar items that can be inserted.
 */
- (NSArray *) toolbarAllowedItemIdentifiers:(NSToolbar *) toolbar;

/**
 * @brief       Returns the list of toolbar items that appear in the toolbar as default.
 *
 * @param[in]   toolbar the toolbar.
 *
 * @returns     an array with the identifiers of the default toolbar items.
 */
- (NSArray *) toolbarDefaultItemIdentifiers:(NSToolbar *) toolbar;

/**
 * @brief       Returns the list of toolbar items that can be selected.
 *
 * @param[in]   toolbar the toolbar.
 *
 * @returns     an array with the identifiers of the selectable toolbar items.
 */
- (NSArray *) toolbarSelectableItemIdentifiers:(NSToolbar *) toolbar;

/**
 * @brief       Called before an item is inserted into the toolbar.
 *
 * @param[inn]  notification a willAddItemNotification.
 */
- (void) toolbarWillAddItem:(NSNotification *) notification;

/**
 * @brief       Called after an item has been removed from the toolbar.
 *
 * @param[inn]  notification a didRemoveItemNotification.
 */
- (void) toolbarDidRemoveItem:(NSNotification *) notification;

/**
 * @brief       Returns the list of identifiers used on the toolbar.
 *
 * @returns     the list of items in an array.
 **/
- (NSArray *) identifiers;

/**
 * @brief       Returns an native NSImage from a QPixmap.
 *
 * @param[in]   pixmap the pixmap to be converted.
 *
 * @returns     the resulting NSImage.
 */
- (NSImage *) imageFromPixmap:(const QPixmap &) pixmap;

/**
 * @brief       Sets the list of toolbar icons.
 *
 * @param[in]   items the list of items.
 *
 * @note        The items are not directly added to the toolbar, instead their identifiers are returned from a
 *              delegate which then calls the appropriate functions to allow creation of the native toolbar
 *              items.
 */
- (void) setItems:(QList<Nedrysoft::SettingsDialog::MacToolbarItem *>) items;

/**
 * @brief       Called when the user selects a toolbar item.
 *
 * @param[in]   toolbarItem is the Nedrysoft::SettingsDialog::MacToolbarItem instance for the selected iten.
 */
- (void) itemSelected:(NSToolbarItem *) toolbarItem;

@end


