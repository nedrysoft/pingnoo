/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/03/2020.
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
     * @brief       The IEditor interface defines the contract of an editor
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
             * @brief       Destroys the IEditor.
             */
            virtual ~IEditor() = default;

            /**
             * @brief       Returns the widget for the editor.
             *
             * @returns     the widget
             */
            virtual auto widget() -> QWidget * = 0;

            /**
             * @brief       Returns the display name for the editor.
             *
             * @details     Returns the user displayable name of the editor, this is usually used by the
             *              Nedrysoft::Core::IEditorManager to display the name of the widget in the title.
             *
             * @returns     the displayed name of the editor.
             */
            virtual auto displayName() -> QString = 0;

            /**
             * @brief       The editor manager calls this method when an editor is activated. (i.e becomes the active
             *              editor)
             */
            virtual auto activated() -> void = 0;

            /**
             * @brief       The editor manager calls this method when an editor is deactivated. (i.e focus is lost)
             *              editor)
             */
            virtual auto deactivated() -> void = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IEditor, "com.nedrysoft.core.IEditor/1.0.0")

#endif // NEDRYSOFT_CORE_IEDITOR_H
