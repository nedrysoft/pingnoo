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

#include "RibbonBarManager.h"

#include "RibbonPage.h"

#include <QGridLayout>
#include <QString>

Nedrysoft::Core::RibbonBarManager::RibbonBarManager() :
        m_ribbonWidget(nullptr) {

}

Nedrysoft::Core::RibbonBarManager::RibbonBarManager(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget) :
        m_ribbonWidget(ribbonWidget) {

}

Nedrysoft::Core::IRibbonPage *Nedrysoft::Core::RibbonBarManager::addPage(QString title, QString id) {
    auto ribbonPage = new Nedrysoft::Core::RibbonPage();

    m_ribbonWidget->addTab(ribbonPage->widget(), title);

    m_pages[id] = ribbonPage;

    return ribbonPage;
}

Nedrysoft::Core::IRibbonPage *Nedrysoft::Core::RibbonBarManager::page(QString id) {
    if (m_pages.contains(id)) {
        return m_pages[id];
    }

    return nullptr;
}

void Nedrysoft::Core::RibbonBarManager::setRibbonWidget(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget) {
    m_ribbonWidget = ribbonWidget;
}