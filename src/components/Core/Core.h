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

#ifndef NEDRYSOFT_CORE_CORE_H
#define NEDRYSOFT_CORE_CORE_H

#include "mainwindow.h"
#include "ICore.h"
#include <QObject>
#include <QString>
#include <QPointer>

/**
 * @brief       Namespace for the core component.
 *
 * @details     The core component namespace contains all definitions and functions relating to the core component.  It is a root dependency of
 *              all other components as it provides all core services.
 */
namespace Nedrysoft::Core {
    /**
     * IP version enumeration
     */
    enum IPVersion {
        V4 = 4,
        V6 = 6
    };

    /**
     * @brief       Core class
     *
     * @details     Provides an implemention of ICore which provides the main window for the
     *              application
     */
    class Core :
            public Nedrysoft::Core::ICore {
        Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::ICore)

        public:
            /**
             * @brief       Constructor
             *
             */
            Core();

            /**
             * @brief       Destructor
             *
             */
            ~Core();

            /**
             * @brief       Returns the main window
             *
             * @details     Returns a pointer to the main window, this function always returns the same
             *              QMainWindow pointer so can be called by any part of the application to get a
             *              handle to the main window
             *
             * @return      returns a pointer to the mainwindow
             */
            virtual QMainWindow *mainWindow();

            /**
             * @brief       Opens the core
             *
             * @details     Should be called after the plugins are loaded to initialise the application,
             *              the Core implemention provides logic to ensure this happens in a sane manner
             *
             */
            void open();

        private:
            QPointer<MainWindow> m_mainWindow;                      //! The QMainWindow smart pointer
    };
}

#endif // NEDRYSOFT_CORE_CORE_H
