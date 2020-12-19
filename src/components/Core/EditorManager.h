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

#ifndef NEDRYSOFT_CORE_EDITORMANAGER_H
#define NEDRYSOFT_CORE_EDITORMANAGER_H

#include "IEditorManager.h"

#include <QMap>
#include <QObject>
#include <QString>
#include <QTabWidget>

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
             * @brief       Constructs a new CoreComponent instance.
             *
             * @param[in]   tabWidget the QTabWidget that is used to contain the editors.
             */
            EditorManager(QTabWidget *tabWidget);

            /**
             * @brief       Open an editor window.
             *
             * @param[in]   editor the editor to open.
             *
             * @returns     zero if editor successfully opened; otherwise non-zero.
             */
            virtual int openEditor(IEditor *editor);

        private:
            QTabWidget *m_tabWidget;
    };
}

#endif // NEDRYSOFT_CORE_EDITORMANAGER_H
