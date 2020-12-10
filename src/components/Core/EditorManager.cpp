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

#include "EditorManager.h"

#include "IEditor.h"

#include <QTabBar>
#include <QTabWidget>

Nedrysoft::Core::EditorManager::EditorManager(QTabWidget *tabWidget) :
        m_tabWidget(tabWidget) {

    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setDocumentMode(true);
}

int Nedrysoft::Core::EditorManager::openEditor(IEditor *editor) {
    auto tabIndex = m_tabWidget->addTab(editor->widget(), editor->displayName());

    m_tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, m_tabWidget->tabBar()->tabButton(tabIndex, QTabBar::LeftSide));

    connect(m_tabWidget, &QTabWidget::tabCloseRequested, [=](int index) {
        this->m_tabWidget->removeTab(index);

        editor->deleteLater();
    });

    return 0;
}
