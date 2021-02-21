/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 25/07/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation∂, either version 3 of the License, or
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
     * @brief       The AboutDialog dialog provides information about the application.
     */
    class AboutDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new AboutDialog instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            explicit AboutDialog(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the AboutDialog.
             */
            ~AboutDialog() override;

        private:
            /**
             * @brief       Returns the compiler information
             *
             * @returns     the compiler information
             */
            auto compiler() -> QString;

        private:
            Ui::AboutDialog *ui;                    //! The generated ui class for the dialog
    };
}

#endif // NEDRYSOFT_CORE_ABOUTDIALOG_H
