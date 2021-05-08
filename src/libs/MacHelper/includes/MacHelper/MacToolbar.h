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

#ifndef NEDRYSOFT_MACTOOLBAR_H
#define NEDRYSOFT_MACTOOLBAR_H

#include <QIcon>
#include <QObject>

class QWidget;
class QWindow;

Q_FORWARD_DECLARE_OBJC_CLASS(NSToolbar);
Q_FORWARD_DECLARE_OBJC_CLASS(ToolbarDelegate);
Q_FORWARD_DECLARE_OBJC_CLASS(NSToolbarItem);
Q_FORWARD_DECLARE_OBJC_CLASS(NSWindow);

namespace Nedrysoft { namespace SettingsDialog {
    class MacToolbarItem;

    /**
     * @brief       The MacToolbar class provides an implementation of a native NSToolbar that is attached to a
     *              a window derived from QWindow.
     */

    class MacToolbar {
        public:
            /**
             *  @brief      Constructs a new MacToolbar instance.
             */
            MacToolbar();

            /**
             * @brief       Destroys a MacToolbar.
             */
            ~MacToolbar();

            /**
             * @brief       Adds an item to the toolbar.
             *
             * @notes       Items are not constructed until the toolbar is attached to the window.  macOS toolbars
             *              are populated by a delegate at the point of attaching the toolbar to the window.  It
             *              is important that you do not attach the toolbar until the items have been added
             *              by this method.
             *
             *              Every item requires a unique identifier string which is used by the toolbar and the
             *              delegate as the method of correlating items.
             *
             * @param[in]   icon a QIcon of the image to be displayed on the toolbar item.
             * @param[in]   identifier the identifier of the toolbar item.
             * @param[in]   label the label that is to appear on the toolbar item.
             * @param[in]   paletteLabel the label that is used in the NSToolbar customisation palette.
             *
             * @returns     a MacToolbarItem instance.
             */
            auto addItem(const QIcon &icon,
                         const QString &identifier,
                         const QString &label=QString(),
                         const QString &paletteLabel=QString()) -> Nedrysoft::SettingsDialog::MacToolbarItem *;

            /**
             * @brief       Attaches the toolbar to the window.
             *
             * @notes       Must be called after the items have been added, calling this will result in the
             *              NSToolbar calling the delegate to populate the toolbar.
             *
             *              The window must be a QWindow subclass.
             *
             * @param[in]   window the window to attach the window to.
             */
            auto attachToWindow(QWidget *window) -> void;

            /**
             * @brief       Returns the list of MacToolbarItems that have been added to the toolbar.
             *
             * @returns     the list of toolbar items.
             */
            auto items() -> QList<Nedrysoft::SettingsDialog::MacToolbarItem *>;

            /**
             * @brief       Instructs macOS that this is a preferences style toolbar.
             *
             * @notes       Under macOS 11 (Big Sur) or later, the style of a preferences toolbar is different
             *              to a normal toolbar, this function will enable this style.
             */
            auto enablePreferencesToolbar() -> void;

        private:
            NSToolbar *m_toolbar;
            NSWindow *m_window;
            ToolbarDelegate *m_toolbarDelegate;

            QList<Nedrysoft::SettingsDialog::MacToolbarItem *> m_items;
            QWindow *m_parentWindow;
            bool m_isPreferences;
    };
}}

#endif // NEDRYSOFT_MACTOOLBAR_H
