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

#ifndef NEDRYSOFT_CORE_ICORE_H
#define NEDRYSOFT_CORE_ICORE_H

#include "ComponentSystem/IComponentManager.h"
#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QMainWindow>
#include <QObject>

namespace Nedrysoft::Core {
    /**
     * IP version enumeration
     */
    enum class IPVersion {
            V4 = 4,
            V6 = 6
    };
}

namespace Nedrysoft::Core {
    /**
     * @brief       The ICore interface is the root component for the application.
     *
     * @details     ICore should provide the main window for the application and the general framework of the
     *              application (interfaces) which other components use to extend  functionality.
     */
    class NEDRYSOFT_CORE_DLLSPEC ICore :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the Nedrysoft::Core::ICore instance.
             */
            static auto getInstance() -> ICore * {
                return ComponentSystem::getObject<ICore>();
            }

            /**
             * @brief       Returns the main window instance.
             *
             * @details     Returns a pointer to the main window, this function always returns the same
             *              QMainWindow pointer so can be called by any part of the application to get a
             *              handle to the main window.
             *
             * @returns     returns a pointer to the QMainWindow.
             */
            virtual auto mainWindow() -> QMainWindow * = 0;

            /**
             * @brief       This signal is emitted after ICore has initialised.
             *
             * @details     The signal is emitted after all plugins have been loaded and initialised, allows
             *              components to do post-initialisation functions knowing that all plugins have
             *              initialised.
             */
            Q_SIGNAL void coreOpened(void);
    };

    /**
     * @brief       Convenience call to get the main window.
     *
     * @returns     The QMainWindow instance if created; otherwise nullptr;
     */
    inline auto mainWindow() -> QMainWindow * {
        auto core = ICore::getInstance();

        if (core)
            return core->mainWindow();

        return nullptr;
    }
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ICore, "com.nedrysoft.core.ICore/1.0.0")
Q_DECLARE_METATYPE(Nedrysoft::Core::IPVersion);

#endif // NEDRYSOFT_CORE_ICORE_H
