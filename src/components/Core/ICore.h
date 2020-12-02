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

#ifndef NEDRYSOFT_CORE_ICORE_H
#define NEDRYSOFT_CORE_ICORE_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include "ComponentSystem/IComponentManager.h"
#include <QObject>
#include <QMainWindow>

namespace Nedrysoft::Core {
    /**
     * @brief       Interface definition of of the core component object
     *
     * @details     provides a mechanism for components to know when the system has
     *              been initialised, including access to core objects.
     *
     */
    class NEDRYSOFT_CORE_DLLSPEC ICore :
            public Nedrysoft::ComponentSystem::IInterface {
        Q_OBJECT

        public:
            /**
             * @brief       Gets the Nedrysoft::Core::ICore instance.
             *
             */
            static ICore *getInstance() {
                return ComponentSystem::getObject<ICore>();
            }

            /**
             * @brief       Gets the main window.
             *
             * @details     Returns a pointer to a QMainWindow object which is the main window of
             *              the application
             *
             * @return      the QMainWindow pointer
             *
             */
            virtual QMainWindow *mainWindow() = 0;

            /**
             * @brief       Core opened signal
             *
             * @details     The signal is emitted after all plugins have been loaded and initialised, allows
             *              components to do post-initialisation functions knowing that all plugins have
             *              initialised
             *
             */
            Q_SIGNAL void coreOpened(void);
    };

    inline QMainWindow *mainWindow() {
        auto core = ICore::getInstance();

        if (core)
            return core->mainWindow();

        return nullptr;
    }
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ICore, "com.fizzyade.core.ICore/1.0.0")

#endif // NEDRYSOFT_CORE_ICORE_H
