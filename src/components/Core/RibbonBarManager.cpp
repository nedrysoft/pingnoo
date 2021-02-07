/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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

constexpr auto ribbonOrderProperty = "nedrysoft.ribbon.order";

Nedrysoft::Core::RibbonBarManager::~RibbonBarManager() {
    qDeleteAll(m_pages);
}

Nedrysoft::Core::RibbonBarManager::RibbonBarManager(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget) :
        m_ribbonWidget(ribbonWidget) {

}

auto Nedrysoft::Core::RibbonBarManager::addPage(QString title, QString id, float order) -> Nedrysoft::Core::IRibbonPage * {
    auto ribbonPage = new Nedrysoft::Core::RibbonPage(this);
    int tabIndex = -1;

    ribbonPage->widget()->setProperty(ribbonOrderProperty, QVariant(order));

    if (m_ribbonWidget->count()) {
        if (order<m_ribbonWidget->widget(0)->property(ribbonOrderProperty).toFloat()) {
            tabIndex = m_ribbonWidget->insertTab(0,ribbonPage->widget(), title);
        } else if (order>m_ribbonWidget->widget(m_ribbonWidget->count()-1)->property(ribbonOrderProperty).toFloat()) {
            tabIndex = m_ribbonWidget->addTab(ribbonPage->widget(), title);
        } else {
            for (auto currentPage=0; currentPage<m_ribbonWidget->count(); currentPage++) {
                auto currentOrder = m_ribbonWidget->widget(currentPage)->property(ribbonOrderProperty).toFloat();

                if (order<currentOrder) {
                    tabIndex = m_ribbonWidget->insertTab(currentPage,ribbonPage->widget(), title);

                    break;
                }

                if (currentOrder>order) {
                    tabIndex = m_ribbonWidget->insertTab(currentPage,ribbonPage->widget(), title);

                    break;
                }

                if (title.compare(m_ribbonWidget->tabText(currentPage))<0) {
                    tabIndex = m_ribbonWidget->insertTab(currentPage,ribbonPage->widget(), title);

                    break;
                }
            }
        }
    }

    if (tabIndex==-1) {
        tabIndex = m_ribbonWidget->addTab(ribbonPage->widget(), title);
    }

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    m_ribbonWidget->setTabVisible(tabIndex, false);
#endif

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
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    for(auto currentIndex=0;currentIndex<m_ribbonWidget->count();currentIndex++) {
        if (m_ribbonWidget->widget(currentIndex)==page->widget()) {
            m_ribbonWidget->setTabVisible(currentIndex, true);
        }
    }
#endif
}

auto Nedrysoft::Core::RibbonBarManager::selectPage(QString id) -> bool {
    if (m_pages.contains(id)) {
        m_ribbonWidget->setCurrentWidget(m_pages[id]->widget());

        return true;
    }

    return false;
}