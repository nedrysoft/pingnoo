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

#ifndef FIZZYADE_CORE_IEDITOR_H
#define FIZZYADE_CORE_IEDITOR_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include <QWidget>

namespace FizzyAde::Core
{
    /**
     * Interface definition of a editor
     *
     * An editor is an editor window that contains a widget and is managed
     * by the application.
     *
     */

    class FIZZYADE_CORE_DLLSPEC IEditor :
        public FizzyAde::ComponentSystem::IInterface
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::ComponentSystem::IInterface)

    public:
        virtual ~IEditor() {}

        /**
         * Sets the measurement interval for this engine instance
         *
         * @param[in] interval interval time
         * @return returns true on success, else false
         *
         */
        virtual QWidget *widget() = 0;
    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::IEditor, "com.fizzyade.core.IEditor/1.0.0")

#endif // FIZZYADE_CORE_IEDITOR_H
