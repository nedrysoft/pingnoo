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

#ifndef NEDRYSOFT_ROUTEANALYSER_NEWTARGETDIALOG_H
#define NEDRYSOFT_ROUTEANALYSER_NEWTARGETDIALOG_H

#include <QDialog>
#include "Core/IPingEngineFactory.h"

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class NewTargetDialog;
    }

    /**
     * @brief       NewTargetDialog
     *
     * @details     The dialog displayed when creating a new route analyser instance, allows
     *              the user to select the target, ping engine, interval etc.
     *
     */
    class NewTargetDialog :
            public QDialog {
        Q_OBJECT

        public:
            /**
             * @brief       Constructor
             *
             * @param[in]   parent          parent widget
             *
             */
            explicit NewTargetDialog(QWidget *parent = nullptr);

            /**
             * @brief       Destructor
             *
             */
            ~NewTargetDialog();

            /**
             * @brief       The ping engine factory selected
             *
             * @return      the selected IPingEngineFactory
             *
             */
            Nedrysoft::Core::IPingEngineFactory *pingEngineFactory();

            /**
             * @brief       The selected target
             *
             * @return      the target (host or ip address)
             *
             */
            QString pingTarget();

            /**
             * @brief       The selected IP version
             *
             * @return      the ip version (V4 or V6)
             *
             */
            Nedrysoft::Core::IPVersion ipVersion();

            /**
             * @brief       The ping interval
             *
             * @details     The ping interval is the number of seconds between ping clusters,
             *              this can be a sub-second value.
             *
             * @return      the ping internal in seconds
             *
             */
            double interval();

        private:
            Ui::NewTargetDialog *ui;
    };
}

#endif // NEDRYSOFT_ROUTEANALYSER_NEWTARGETDIALOG_H
