/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 11/03/2021.
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

#ifndef NEDRYSOFT_OPENFAVOURITEDIALOG_H
#define NEDRYSOFT_OPENFAVOURITEDIALOG_H

#include <QDialog>

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class OpenFavouriteDialog;
    }

    /**
     * @brief       The OpenFavouriteDialog class provides a list of favourites which the user can use
     *              to locate a specific entry.
     */
    class OpenFavouriteDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new OpenFavouriteDialog
             *
             * @param[in]   parent the parent dialog.
             */
            explicit OpenFavouriteDialog(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the OpenFavouriteDialog
             */
            ~OpenFavouriteDialog() override;

        private:
            Ui::OpenFavouriteDialog *ui;
    };
}
#endif //NEDRYSOFT_OPENFAVOURITEDIALOG_H
