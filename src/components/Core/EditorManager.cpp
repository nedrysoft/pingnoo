/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/07/2020.
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

#include "EditorManager.h"

#include "IEditor.h"

#include <QTabBar>
#include <QTabWidget>

constexpr auto stylesheet = R"(
QTabBar::tab {
    padding-right:16px;
    text-align: center;
    margin-left: 16px;
}

QTabBar::close-button {
    image: url(:/RibbonIcons/Icons/close-button-light.png);
}

QTabBar::close-button:hover {
    image: url(:/RibbonIcons/Icons/close-button-dark.png);
}

)";

Nedrysoft::Core::EditorManager::EditorManager(QTabWidget *tabWidget) :
        m_tabWidget(tabWidget) {

    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setDocumentMode(true);
}

auto Nedrysoft::Core::EditorManager::openEditor(IEditor *editor) -> int {
    auto tabIndex = m_tabWidget->addTab(editor->widget(), editor->displayName());
    m_tabWidget->setStyleSheet(stylesheet);
#if defined(Q_OS_MACOS)
    m_tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, m_tabWidget->tabBar()->tabButton(tabIndex, QTabBar::LeftSide));
#else
    Q_UNUSED(tabIndex)
#endif

    connect(m_tabWidget, &QTabWidget::tabCloseRequested, [=](int index) {
        m_tabWidget->removeTab(index);

        //editor->closed();
    });

    return 0;
}
