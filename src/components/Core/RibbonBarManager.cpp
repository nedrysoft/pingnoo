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

#include "ICommandManager.h"
#include "RibbonActionProxy.h"
#include "RibbonPage.h"

constexpr auto RibbonOrderProperty = "nedrysoft.ribbon.order";

Nedrysoft::Core::RibbonBarManager::~RibbonBarManager() {
    qDeleteAll(m_pages);
}

Nedrysoft::Core::RibbonBarManager::RibbonBarManager() :
        m_ribbonWidget(nullptr) {

}

auto Nedrysoft::Core::RibbonBarManager::addPage(QString title, QString id, float order) -> Nedrysoft::Core::IRibbonPage * {
    auto ribbonPage = new Nedrysoft::Core::RibbonPage(this);
    int tabIndex = -1;

    ribbonPage->widget()->setProperty(RibbonOrderProperty, QVariant(order));

    if (m_ribbonWidget->count()) {
        if (order<m_ribbonWidget->widget(0)->property(RibbonOrderProperty).toFloat()) {
            tabIndex = m_ribbonWidget->insertTab(0,ribbonPage->widget(), title);
        } else if (order>m_ribbonWidget->widget(m_ribbonWidget->count()-1)->property(RibbonOrderProperty).toFloat()) {
            tabIndex = m_ribbonWidget->addTab(ribbonPage->widget(), title);
        } else {
            for (auto currentPage=0; currentPage<m_ribbonWidget->count(); currentPage++) {
                auto currentOrder = m_ribbonWidget->widget(currentPage)->property(RibbonOrderProperty).toFloat();

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

#if (QT_VERSION >= QT_VERSION_CHECK(5,15,0))
    m_ribbonWidget->setTabVisible(tabIndex, false);
#else
    RibbonPageVisibility page;

    page.title = title;
    page.page = ribbonPage;
    page.visible = false;

    m_visibleList.insert(tabIndex, page);

    m_ribbonWidget->removeTab(tabIndex);
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

auto Nedrysoft::Core::RibbonBarManager::groupAdded(Nedrysoft::Core::RibbonPage *page) -> void {
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    for(auto currentIndex=0;currentIndex<m_ribbonWidget->count();currentIndex++) {
        if (m_ribbonWidget->widget(currentIndex)==page->widget()) {
            m_ribbonWidget->setTabVisible(currentIndex, true);
        }
    }
#else
    for (auto tabIndex=0;tabIndex<m_visibleList.count();tabIndex++) {
        if (m_visibleList[tabIndex].page==page) {
            if (!m_visibleList[tabIndex].visible) {
                m_visibleList[tabIndex].visible = true;
                m_ribbonWidget->insertTab(
                        tabIndex,
                        m_visibleList[tabIndex].page->widget(),
                        m_visibleList[tabIndex].title);
            }
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

auto Nedrysoft::Core::RibbonBarManager::registerAction(
        Nedrysoft::Ribbon::RibbonAction *action,
        QString commandId,
        int contextId) -> Nedrysoft::Ribbon::RibbonAction * {

    auto actionName = QString("%1.%2").arg(commandId).arg(contextId);

    auto contextManager = Nedrysoft::Core::IContextManager::getInstance();

    assert(contextManager!=nullptr);

    if (m_commandMap.contains(commandId)) {
        if (!m_actionMap.contains(actionName)) {
            m_actionMap[actionName] = action;
        }

        if (contextId==contextManager->context()) {
            m_commandMap[commandId]->setActive(action);
        }

        return m_commandMap[commandId];
    }

    auto newActionProxy = new RibbonActionProxy;

    connect(
        contextManager,
        &Nedrysoft::Core::IContextManager::contextChanged,
        [=](int newContext, int previousContext) {
            auto actionName = QString("%1.%2").arg(commandId).arg(newContext);

            newActionProxy->setActive(m_actionMap[actionName]);
    });

    m_commandMap[commandId] = newActionProxy;
    m_actionMap[actionName] = action;

    if (contextId==contextManager->context()) {
        newActionProxy->setActive(action);
    }

    return newActionProxy;
}

auto  Nedrysoft::Core::RibbonBarManager::setRibbonBar(Nedrysoft::Ribbon::RibbonWidget *widget) -> void {
    m_ribbonWidget = widget;
}

