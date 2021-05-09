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

#include <QtGlobal>

#include "EditorManager.h"

#include "IEditor.h"
#include "ThemeSupport/includes/ThemeSupport.h"

#include <QCheckBox>
#include <QTabBar>
#include <QTabWidget>

constexpr auto MacStylesheet = R"(
    QTabBar::close-button {
        text-align: right;
    }
)";

constexpr auto WindowsStylesheet = R"(
    QTabBar::tab {
        padding-right: 8px;
        text-align: center;
        padding-left: 16px;
        height: 22px;
        border-bottom: 0px;
    }

    QTabBar {
        border-bottom: 4px solid #007dd2;
    }

    QTabBar::tab:selected {
        background-color: #007dd2;
        color: white;
 		border-bottom: 4px solid #007dd2;
    }

    QTabBar::tab:!selected {
        background-color: lightgrey;
        color: black;
    }

    QTabBar::close-button:selected {
        image: url(:/Core/icons/1x/close-button-white.png);
    }

    QTabBar::close-button:!selected {
        image: url(:/Core/icons/1x/close-button-grey.png);
    }

    QTabBar::close-button:selected:hover {
        image: url(:/Core/icons/1x/close-button-dark.png);
    }

    QTabBar::close-button:!selected:hover {
        image: url(:/Core/icons/1x/close-button-dark.png);
    }
)";

constexpr auto LinuxDarkStylesheet = R"(
    QTabBar::tab {
        padding-right: 8px;
        text-align: center;
        padding-left: 16px;
    }

    QTabBar {
        border-bottom: 4px solid #505050;
    }

    QTabBar::tab:selected {
        background-color: #505050;
        color: white;
 		border-bottom: 4px solid #505050;
    }

    QTabBar::tab:!selected {
        background-color: #404040;
        color: lightgrey;
    }
    QTabBar::close-button:selected {
        image: url(:/Core/icons/1x/close-button-grey.png);
    }

    QTabBar::close-button:!selected {
        image: url(:/Core/icons/1x/close-button-grey.png);
    }

    QTabBar::close-button:selected:hover {
        image: url(:/Core/icons/1x/close-button-light.png);
    }

    QTabBar::close-button:!selected:hover {
        image: url(:/Core/icons/1x/close-button-dark.png);
    }
)";

constexpr auto LinuxLightStylesheet = R"(
    QTabBar::tab {
        padding-right: 8px;
        text-align: center;
        padding-left: 16px;
    }

    QTabBar {
        border-bottom: 4px solid #FFFFFF;
    }

    QTabBar::tab:selected {
        background-color: #FFFFFF;
        color: black;
 		border-bottom: 4px solid #FFFFFF;
    }

    QTabBar::tab:!selected {
        background-color: lightgrey;
        color: black;
    }
    QTabBar::close-button:selected {
        image: url(:/Core/icons/1x/close-button-grey.png);
    }

    QTabBar::close-button:!selected {
        image: url(:/Core/icons/1x/close-button-grey.png);
    }

    QTabBar::close-button:selected:hover {
        image: url(:/Core/icons/1x/close-button-light.png);
    }

    QTabBar::close-button:!selected:hover {
        image: url(:/Core/icons/1x/close-button-dark.png);
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
#elif defined(Q_OS_WINDOWS)
    Q_UNUSED(tabIndex);

    m_tabWidget->setStyleSheet(WindowsStylesheet);
#else
    Q_UNUSED(tabIndex);
    if (Nedrysoft::ThemeSupport::ThemeSupport().isDarkMode()) {
        m_tabWidget->setStyleSheet(LinuxDarkStylesheet);
    } else {
        m_tabWidget->setStyleSheet(LinuxLightStylesheet);
    };
#endif

    connect(m_tabWidget, &QTabWidget::tabCloseRequested, [=](int index) {
        m_tabWidget->removeTab(index);

        m_editorMap.remove(editor->widget());
    });

    return 0;
}
