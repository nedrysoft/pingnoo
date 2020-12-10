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

#ifndef NEDRYSOFT_CORE_IEDITOR_H
#define NEDRYSOFT_CORE_IEDITOR_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QWidget>

namespace Nedrysoft::Core {
    /**
     * @brief       Interface definition of a editor
     *
     * @details     An editor is an editor window that contains a widget and is managed
     *              by the application.
     */
    class NEDRYSOFT_CORE_DLLSPEC IEditor :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       Destructor
             */
            virtual ~IEditor() = default;

            /**
             * @brief       Gets the widget for this editor.
             *
             * @return      the widget
             */
            virtual QWidget *widget() = 0;

            /**
             * @brief       Geets the display name for this editor
             *
             * @details     Gets the user displayable name of the editor, this is usually used by the Nedrysoft::Core::IEditorManager to display
             *              the name of the widget in the title.
             *
             * @returns     the displayed name of the editor.
             */
            virtual QString displayName() = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IEditor, "com.nedrysoft.core.IEditor/1.0.0")

#endif // NEDRYSOFT_CORE_IEDITOR_H
