/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 31/12/2020.
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

#include "EditorManagerTabWidget.h"

#include <QFontDatabase>
#include <QPaintEvent>
#include <QPainter>

constexpr auto fontPath = ":/Nedrysoft/Core/OpenSans/OpenSans-Regular.ttf";
constexpr auto fontName = "Open Sans";
#if defined(Q_OS_MACOS)
constexpr auto fontSize = 14;
#else
constexpr auto fontSize = 12;
#endif
constexpr auto fontWeight = QFont::Normal;

Nedrysoft::Core::EditorManagerTabWidget::EditorManagerTabWidget(QWidget *parent) :
        QTabWidget(parent),
        m_openSansRegular(QFontDatabase::addApplicationFont(fontPath)) {

    setTabsClosable(true);
}

auto Nedrysoft::Core::EditorManagerTabWidget::setText(QString text) -> void {
    m_text = text;
}

void Nedrysoft::Core::EditorManagerTabWidget::paintEvent(QPaintEvent *event) {
    QTabWidget::paintEvent(event);

    QPainter painter(this);

    painter.setFont(QFont(fontName, fontSize, fontWeight));

    painter.drawText(rect(), Qt::AlignVCenter | Qt::AlignHCenter, m_text);
}