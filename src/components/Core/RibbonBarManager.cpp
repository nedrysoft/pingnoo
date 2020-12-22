/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 10/12/2020.
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

#include <QString>

Nedrysoft::Core::RibbonBarManager::RibbonBarManager() :
        m_ribbonWidget(nullptr) {

}

Nedrysoft::Core::RibbonBarManager::RibbonBarManager(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget) :
        m_ribbonWidget(ribbonWidget) {

}

auto Nedrysoft::Core::RibbonBarManager::addPage(QString title, QString id) -> Nedrysoft::Core::IRibbonPage * {
    auto ribbonPage = new Nedrysoft::Core::RibbonPage(this);

    int tabIndex = m_ribbonWidget->addTab(ribbonPage->widget(), title);

    m_ribbonWidget->setTabVisible(tabIndex, false);

    m_pages[id] = ribbonPage;

    return ribbonPage;
}

auto Nedrysoft::Core::RibbonBarManager::page(QString id) -> Nedrysoft::Core::IRibbonPage * {
    if (m_pages.contains(id)) {
        return m_pages[id];
    }

    return nullptr;
}

auto Nedrysoft::Core::RibbonBarManager::setRibbonWidget(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget) -> void {
    m_ribbonWidget = ribbonWidget;
}

auto Nedrysoft::Core::RibbonBarManager::groupAdded(Nedrysoft::Core::RibbonPage *page) -> void {
    for(auto currentIndex=0;currentIndex<m_ribbonWidget->count();currentIndex++) {
        if (m_ribbonWidget->widget(currentIndex)==page->widget()) {
            m_ribbonWidget->setTabVisible(currentIndex, true);
        }
    }
}