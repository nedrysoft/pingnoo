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

#ifndef NEDRYSOFT_FAVOURITEEDITORDIALOG_H
#define NEDRYSOFT_FAVOURITEEDITORDIALOG_H

#include <QDialog>
#include <QVariantMap>

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class FavouriteEditorDialog;
    }

    /**
     * @brief       The FavouriteEditorDialog provides an editor for the settings of a favourite, the title can be
     *              set according to the function (Add New Favourite/Edit Favourite)
     */
    class FavouriteEditorDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new favourites editor with the given title, settings & parent
             *
             * @param[in]   title the title of the window.
             * @param[in]   parameters the favourite settings parameters to edit.
             * @param[in]   parent the parent widget.
             */
            explicit FavouriteEditorDialog( QString title, QVariantMap parameters, QWidget *parent = nullptr);

            /**
             * @brief       Destroys the favourites editor.
             */
            ~FavouriteEditorDialog() override;

            /**
             * @brief       Returns the parameter map
             *
             * @returns     the parameter map.
             */
            auto map() -> QVariantMap;

        private:
            Ui::FavouriteEditorDialog *ui;

            QVariantMap m_parametersMap;
    };
}

#endif //NEDRYSOFT_FAVOURITEEDITORDIALOG_H
