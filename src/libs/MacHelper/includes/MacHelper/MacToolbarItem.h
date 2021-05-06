/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 07/05/2021.
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

#ifndef NEDRYSOFT_MACTOOLBARITEM_H
#define NEDRYSOFT_MACTOOLBARITEM_H

#include <QIcon>
#include <QObject>

class QWidget;
class QWindow;

Q_FORWARD_DECLARE_OBJC_CLASS(NSToolbar);
Q_FORWARD_DECLARE_OBJC_CLASS(ToolbarDelegate);
Q_FORWARD_DECLARE_OBJC_CLASS(NSToolbarItem);
Q_FORWARD_DECLARE_OBJC_CLASS(NSWindow);

namespace Nedrysoft { namespace SettingsDialog {
    /**
     * @brief       The MacToolbarItem class represents a toolbar item.  It is a container for the native
     *              NSToolbarItem.
     */
    class MacToolbarItem :
                public QObject {

        public:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new toolbar item with the given parameters.
             *
             * @note        The identifier should be unique as it is used by the NSToolbar and its delegate to
             *              correlate items.
             *
             *              If label or palette label are omitted, they are set to the identifier value.
             *
             * @param[in]   icon the items icon.
             * @param[in]   identifier the identifier used by the toolbar and delegate.
             * @param[in]   label the label for the item.
             * @param[in]   paletteLabel the label that is displayed in the toolbar customiser.
             */
            MacToolbarItem(const QIcon &icon,
                           const QString &identifier,
                           const QString &label = QString(),
                           const QString &paletteLabel = QString());

            /**
             * @brief       Returns the identifier of the item.
             *
             * @returns     the items identifier.
             */
            auto identifier() -> QString;

            /**
             * @brief       Returns the icon of the item.
             *
             * @returns     the items icon.
             */
            auto icon() -> QIcon;

            /**
             * @brief       Returns the native NSToolBarItem assigned to the item.
             *
             * @note        This will be a nullptr until the toolbar has been attached to the window and the
             *              delegate called to populate the toolbar.
             *
             * @returns     the native NSToolbarItem if available; otherwise nullptr.
             */
            auto item() -> NSToolbarItem *;

            /**
             * @frief       Returns the label that is displayed on the toolbar item.
             *
             * @note        If this has not been set, then it defaults to the items identifier.
             *
             * @returns     the toolbar label.
             */
            auto label() -> QString;

            /**
             * @brief       Returns the label that is displayed on customiser for this item.
             *
             * @note        If this has not been set, then it defaults to the items identifier.
             *
             * @returns     the toolbars palette label.
             */
            auto paletteLabel() -> QString;

            /**
             * @brief       Sets the native toolbar item.
             *
             * @note        Should not be directly called, the delegate will call this when the toolbar is
             *              populated.
             *
             * @param[in]   item the native item to associate with this toolbar item.
             */
            auto setToolbarItem(NSToolbarItem *item) -> void;

        public:
            /**
             * @brief       This signal is emitted when the item is clicked and becomes active.
             */
            Q_SIGNAL void activated();

        private:
            NSToolbarItem *m_toolbarItem;
            QString m_identifier;
            QIcon m_icon;
            QString m_paletteLabel;
            QString m_label;
    };
}}

#endif // NEDRYSOFT_MACTOOLBARITEM_H
