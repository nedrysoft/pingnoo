/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 31/03/2021.
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

#ifndef NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_H
#define NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_H

#include <QColor>
#include <QObject>
#include <QRgb>
#include <QtGlobal>

#if ( defined(NEDRYSOFT_LIBRARY_THEMESUPPORT_EXPORT))
#define NEDRYSOFT_THEMESUPPORT_DLLSPEC Q_DECL_EXPORT
#else
#define NEDRYSOFT_THEMESUPPORT_DLLSPEC Q_DECL_IMPORT
#endif

namespace Nedrysoft::ThemeSupport {
    enum class ThemeMode {
        System,
        Light,
        Dark
    };

    /**
     * @brief       The ThemeSupport class provides information about the operating system theme.
     */
    class NEDRYSOFT_THEMESUPPORT_DLLSPEC ThemeSupport :
            public QObject {

        public:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new ThemeSupport instance.
             */
            explicit ThemeSupport();

            /**
             * @brief       Sets the application theme mode.
             *
             * @param[in]   mode the mode to set.
             */
             auto setMode(ThemeMode mode) -> void;

            /**
             * @brief       Returns the current OS theme mode.
             *
             * @returns     true if dark mode; otherwise false.
             */
            static auto isDarkMode() -> bool;

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
            static auto getColor(const QRgb PushButtonColor[]) -> QColor;

            /**
             * @brief       Returns the highlighted text background color
             *
             * @note        Qt does not return the correct color when using QPalette, this function directly queries
             *              the macOS to discover the real highlighted background color.
             *
             * @returns     the colour
             */
            static auto getHighlightedBackground() -> QColor;

        private:
            static ThemeMode m_themeMode;
    };
}

#endif // NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_H
