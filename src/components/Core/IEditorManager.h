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

#ifndef NEDRYSOFT_CORE_IEDITORMANAGER_H
#define NEDRYSOFT_CORE_IEDITORMANAGER_H

#include "ComponentSystem/IComponentManager.h"
#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QObject>

namespace Nedrysoft::Core {
    class IEditor;

    /**
     * @brief       Interface definition of an application editor manager
     *
     * @details     Manages the open editors in the application.
     *
     */
    class NEDRYSOFT_CORE_DLLSPEC IEditorManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Gets the Nedrysoft::Core::IEditorManager instance.
             */
            static IEditorManager *getInstance() {
                return ComponentSystem::getObject<IEditorManager>();
            }

            /**
             * @brief       Open an editor window
             *
             * @param[in]   editor the editor to open
             *
             * @returns     zero
             */
            virtual int openEditor(IEditor *editor) = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IEditorManager, "com.nedrysoft.core.IEditorManager/1.0.0")

#endif // NEDRYSOFT_CORE_IEDITORMANAGER_H
