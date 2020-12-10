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

#ifndef NEDRYSOFT_CORE_ABOUTDIALOG_H
#define NEDRYSOFT_CORE_ABOUTDIALOG_H

#include <QDialog>

namespace Nedrysoft::Core {
    namespace Ui {
        class AboutDialog;
    }

    /**
     * @brief       AboutDialog
     *
     * @details     Dialog that shows the the information about the current
     *              build of the software.
     *
     */
    class AboutDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:

            /**
             * @brief       Constructor
             *
             * @param[in]   parent parent widget
             *
             */
            explicit AboutDialog(QWidget *parent = nullptr);

            /**
             * @brief       Destructor
             *
             */
            ~AboutDialog();

        private:
            /**
             * @brief       Return compiler information
             *
             * @return      the compiler information
             *
             */
            QString compiler();

        private:
            Ui::AboutDialog *ui;                    //! The generated ui class for the dialog
    };
}

#endif // NEDRYSOFT_CORE_ABOUTDIALOG_H
