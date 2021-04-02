/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft Ribbon Bar. (https://github.com/nedrysoft/qt-ribbon)
 *
 * A cross-platform ribbon bar for Qt applications.
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

#include "includes/ThemeSupport.h"

#include <QApplication>
#include <QStyle>

auto Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode() -> bool {
    return m_themeMode==Nedrysoft::ThemeSupport::ThemeMode::Dark;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::getHighlightedBackground() -> QColor {
    return qobject_cast<QApplication *>(QCoreApplication::instance())->style()->standardPalette().color(QPalette::Highlight);
}

