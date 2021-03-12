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

#ifndef NEDRYSOFT_CORE_CORE_H
#define NEDRYSOFT_CORE_CORE_H

#include "ICore.h"
#include "MainWindow.h"

#include <QObject>
#include <QPointer>
#include <QString>

namespace Nedrysoft::Core {
    /**
     * @brief       The Core class is the root component for the application.
     *
     * @details     Provides an implementation of ICore which provides the main window for the application and
     *              provides the framework of the application (interfaces) which other components use to extend
     *              functionality.
     */
    class Core :
            public Nedrysoft::Core::ICore {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::ICore)

        public:
            /**
             * @brief       Constructs a new Core instance.
             */
            Core();

            /**
             * @brief       Destroys the Core.
             */
            ~Core();

        public:
            /**
             * @brief       Returns the main window instance.
             *
             * @details     Returns a pointer to the main window, this function always returns the same
             *              QMainWindow pointer so can be called by any part of the application to get a
             *              handle to the main window.
             *
             * @see         Nedrysoft::Core::ICore::mainWindow
             *
             * @returns     returns a pointer to the QMainWindow.
             */
            auto mainWindow() -> QMainWindow * override;

            /**
             * @brief       Opens the core.
             *
             * @details     Should be once by the application after the components are loaded.  Components connect
             *              to the Nedrysoft::Core::ICore::coreOpened signal to perform post load initialisation.
             *
             * @see         Nedrysoft::Core::ICore::open
             */
            auto open() -> void;

        private:
            QPointer<MainWindow> m_mainWindow;                      //! The QMainWindow smart pointer
    };
}

#endif // NEDRYSOFT_CORE_CORE_H
