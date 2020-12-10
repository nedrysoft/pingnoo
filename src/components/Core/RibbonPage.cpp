/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo
 *
 * Created by Adrian Carpenter on 11/12/2020.
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

#include "RibbonPage.h"

#include "Ribbon/RibbonGroup.h"

#include <QDebug>

Nedrysoft::Core::RibbonPage::RibbonPage() :
        m_pageWidget(new QWidget()),
        m_pageLayout(new QHBoxLayout()) {

    m_pageLayout->setSpacing(0);

    m_pageLayout->setContentsMargins(0, 0, 0, 6);

    m_pageWidget->setLayout(m_pageLayout);
}

QWidget *Nedrysoft::Core::RibbonPage::widget() {
    return m_pageWidget;
}

Nedrysoft::Core::IRibbonGroup *Nedrysoft::Core::RibbonPage::addGroup(QString title, QString id, QWidget *widget) {
    auto ribbonGroup = new Nedrysoft::Ribbon::RibbonGroup();

    ribbonGroup->setGroupName(title);

    m_pageLayout->addWidget(ribbonGroup);
    m_pageLayout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    m_pageLayout->setStretch(1, 2);

    auto groupLayout = new QGridLayout();

    groupLayout->setSpacing(0);
    groupLayout->setMargin(0);
    groupLayout->addWidget(widget);

    ribbonGroup->setLayout(groupLayout);

    return nullptr;
}
