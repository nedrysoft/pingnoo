/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft Ribbon Bar. (https://github.com/nedrysoft/qt-ribbon)
 *
 * A cross-platform ribbon bar for Qt applications.
 *
 * Created by Adrian Carpenter on 02/12/2020.
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

#ifndef NEDRYSOFT_THEMESUPPORT_H
#define NEDRYSOFT_THEMESUPPORT_H

#include <QColor>
#include <QObject>
#include <QRgb>
#include <QtGlobal>

namespace Nedrysoft::Utils {
    /**
     * @brief       The ThemeSupport class provides helpers for handling dark/light themes.
     *
     * @details     Provides OS level support for theming, currently only supports macOS but is able to
     *              to detect whether the OS is in light or dark mode and also signal when the
     *              current theme is changed.
     */
    class ThemeSupport :
            public QObject {

        public:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new ThemeSupport instance.
             */
#if defined(Q_OS_MACOS)
            explicit ThemeSupport();
#else
            explicit ThemeSupport() = default;
#endif
            /**
             * @brief       Returns the current OS theme mode.
             *
             * @returns     true if dark mode; otherwise false.
             */
#if defined(Q_OS_MACOS)
            static auto isDarkMode() -> bool;
#else
            static auto isDarkMode() -> bool {
                return false;
            }
#endif

            /**
             * @brief       This signal is emitted when OS theme is changed.
             *
             * @param[in]   isDarkMode true if dark mode; otherwise false.
             */
            Q_SIGNAL void themeChanged(bool isDarkMode);

            /**
             * @brief       Returns the Dark or Light colour for the given array.
             *
             * @note        The array consists of 2 QRgb entries, one entry is for dark and the other for light mode.
             *
             * @returns     the colour.
             */
#if defined(Q_OS_MACOS)
            static auto getColor(const QRgb PushButtonColor[]) -> QColor;
#else
            static auto getColor(const QRgb PushButtonColor[]) -> QColor {
                Q_UNUSED(PushButtonColor)

                return QColor();
            }
#endif
            /**
             * @brief       Returns the highlighted text background color
             *
             * @note        Qt does not return the correct color when using QPalette, this function directly queries
             *              the macOS to discover the real highlighted background color.
             *
             * @returns     the colour
             */
#if defined(Q_OS_MACOS)
            static auto getHighlightedBackground() -> QColor;
#else
            static auto getHighlightedBackground() -> QColor {
                return QColor();
            }
#endif

    };
}

#endif // NEDRYSOFT_THEMESUPPORT_H
