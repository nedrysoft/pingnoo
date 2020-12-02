/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
 * An open source ping path analyser
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

#include "RibbonGroup.h"
#include "RibbonWidget.h"
#include "RibbonFontManager.h"
#include "ThemeSupport.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QApplication>

#if defined(Q_OS_MACOS)
constexpr int DefaultFontSize = 10;
#else
constexpr int DefaultFontSize = 8;
#endif

Nedrysoft::Ribbon::RibbonGroup::RibbonGroup(QWidget *parent) :
        QWidget(parent),
        m_fontMetrics(QFont()) {
    auto fontManager = RibbonFontManager::getInstance();

    m_font = QFont(fontManager->normalFont(), DefaultFontSize);
    m_fontMetrics = QFontMetrics(m_font);

    setGroupName(QString("Group"));

    QFile styleSheet;

    styleSheet.setFileName(":/Nedrysoft/Ribbon/LightStyle.css");

    if (styleSheet.open(QFile::ReadOnly)) {
        m_lightStyleSheet = styleSheet.readAll();

        styleSheet.close();
    }

    styleSheet.setFileName(":/Nedrysoft/Ribbon/DarkStyle.css");

    if (styleSheet.open(QFile::ReadOnly)) {
        m_darkStyleSheet = styleSheet.readAll();

        styleSheet.close();
    }

    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        this->setStyleSheet(m_darkStyleSheet);
    } else {
        this->setStyleSheet(m_lightStyleSheet);
    }

    connect(qApp, &QApplication::paletteChanged, [=](const QPalette &) {
        if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
            this->setStyleSheet(m_darkStyleSheet);
        } else {
            this->setStyleSheet(m_lightStyleSheet);
        }
    });
}

void Nedrysoft::Ribbon::RibbonGroup::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    auto rect = this->rect();
    auto currentTheme = Nedrysoft::Ribbon::Light;

    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        currentTheme = Nedrysoft::Ribbon::Dark;
    }

    rect.setTop(rect.bottom() - m_textRect.height());

    painter.save();

    painter.setClipRect(event->rect());

    painter.setFont(m_font);

    painter.setPen(Ribbon::TextColor[currentTheme]);

    painter.drawText(rect, m_groupName, Qt::AlignBottom | Qt::AlignHCenter);

    painter.setPen(Ribbon::GroupDividerColor[currentTheme]);

    auto startPoint = QPoint(this->rect().right() - 1, this->rect().top() + Ribbon::GroupDividerMargin);
    auto endPoint = QPoint(this->rect().right() - 1, this->rect().bottom() - Ribbon::GroupDividerMargin);

    painter.drawLine(startPoint, endPoint);

    painter.restore();

    QWidget::paintEvent(event);
}

QString Nedrysoft::Ribbon::RibbonGroup::groupName() const {
    return m_groupName;
}

void Nedrysoft::Ribbon::RibbonGroup::setGroupName(const QString &groupName) {
    m_groupName = groupName;

    m_textRect = m_fontMetrics.boundingRect(m_groupName);

    this->setContentsMargins(0, 0, 2, m_textRect.height());

    update();
}

