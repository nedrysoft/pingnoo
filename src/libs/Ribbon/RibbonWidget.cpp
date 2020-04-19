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

#include "RibbonWidget.h"
#include "RibbonTabBar.h"
#include "ThemeSupport.h"
#include <QTabBar>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

FizzyAde::Ribbon::RibbonWidget::RibbonWidget(QWidget *parent) :
    QTabWidget(parent)
{
    m_tabBar = new RibbonTabBar(this);

    setTabBar(m_tabBar);

    this->setMinimumHeight(RibbonBarHeight+TabBarHeight);
    this->setMaximumHeight(RibbonBarHeight+TabBarHeight);

#if defined(Q_OS_MACOS)
    this->setDocumentMode(true);
#endif
}

void FizzyAde::Ribbon::RibbonWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    auto rect = this->rect();
    auto  currentTheme = FizzyAde::Ribbon::Light;

    if (FizzyAde::Utils::ThemeSupport::isDarkMode()) {
        currentTheme = FizzyAde::Ribbon::Dark;
    }

    painter.save();

    painter.fillRect(rect, TabBarBackgroundColor[currentTheme]);

    rect.setTop(rect.bottom());

    painter.fillRect(rect, RibbonBorderColor[currentTheme]);

    painter.restore();
}
