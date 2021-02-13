/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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

#include <QCheckBox>
#include <QTabBar>
#include <QTabWidget>

constexpr auto macStylesheet = R"(
    QTabBar::close-button {
        text-align: right;
    }
)";

constexpr auto otherStylesheet = R"(
    QTabBar::tab {
        padding-right:8px;
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

Nedrysoft::Core::EditorManager::EditorManager(EditorManagerTabWidget *tabWidget) :
        m_tabWidget(tabWidget),
        m_previousIndex(-1) {

    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setDocumentMode(true);

    connect(m_tabWidget, &EditorManagerTabWidget::currentChanged, [=](int index) {
        static IEditor *previousEditor = nullptr;
        IEditor *newEditor = nullptr;

        if (index!=-1) {
            if (m_editorMap.contains(m_tabWidget->widget(index))) {
                newEditor = m_editorMap[m_tabWidget->widget(index)];
            }
        }

        if (previousEditor) {
            previousEditor->deactivated();
        }

        if (newEditor) {
            newEditor->activated();
        }

        previousEditor = newEditor;
    });
}

auto Nedrysoft::Core::EditorManager::openEditor(IEditor *editor) -> int {
    m_editorMap[editor->widget()] = editor;

    auto tabIndex = m_tabWidget->addTab(editor->widget(), editor->displayName());

#if defined(Q_OS_MACOS)
    m_tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, m_tabWidget->tabBar()->tabButton(tabIndex, QTabBar::LeftSide));
#else
    Q_UNUSED(tabIndex);

    m_tabWidget->setStyleSheet(otherStylesheet);
#endif

    connect(m_tabWidget, &QTabWidget::tabCloseRequested, [=](int index) {
        m_tabWidget->removeTab(index);

        m_editorMap.remove(editor->widget());
        //editor->closed();
    });

    return 0;
}
