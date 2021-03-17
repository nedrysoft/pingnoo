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

#ifndef NEDRYSOFT_CORE_EDITORMANAGER_H
#define NEDRYSOFT_CORE_EDITORMANAGER_H

#include "EditorManagerTabWidget.h"
#include "IEditorManager.h"

#include <QMap>
#include <QObject>
#include <QString>
#include <QWidget>

namespace Nedrysoft::Core {

    /**
     * @brief       The EditorManager class.
     *
     * @details     The editor manager provides a container widget to host IEditor instances.
     */
    class EditorManager :
            public Nedrysoft::Core::IEditorManager {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IEditorManager)

        public:
            /**
             * @brief       Constructs a new EditorManager instance.
             *
             * @param[in]   tabWidget the QTabWidget that is used to contain the editors.
             */
            EditorManager(EditorManagerTabWidget *tabWidget);

            /**
             * @brief       Open an editor window.
             *
             * @param[in]   editor the editor to open.
             *
             * @returns     zero if editor successfully opened; otherwise non-zero.
             */
            auto openEditor(IEditor *editor) -> int override;

        private:
            EditorManagerTabWidget *m_tabWidget;
            int m_previousIndex;
            QMap<QWidget *, IEditor *> m_editorMap;
    };
}

#endif // NEDRYSOFT_CORE_EDITORMANAGER_H
