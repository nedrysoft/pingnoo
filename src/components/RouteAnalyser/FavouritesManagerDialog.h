/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 23/02/2021.
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

#ifndef NEDRYSOFT_FAVOURITESMANAGERDIALOG_H
#define NEDRYSOFT_FAVOURITESMANAGERDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class FavouritesManagerDialog;
    }

    /**
     * @brief       The FavouritesManagerDialog provides a dialog for managing saved targets.
     */
    class FavouritesManagerDialog :
            public QDialog {

        private:
            Q_OBJECT

        private:
            enum TableFields {
                Name,
                Description,
                Host,
                IPVersion,
                Interval
            };

        public:
            /**
             * @brief       Constructs a new FavouritesManagerDialog that is a child of the parent.
             *
             * @param[in]   parent the parent for this child.
             */
            explicit FavouritesManagerDialog(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the FavouritesManagerDialog.
             */
            ~FavouritesManagerDialog() override;

        private:
            /**
             * @brief       Creates the items for the favourite ready for adding to the list.
             *
             * @param[in]   favourite the favourite fields.
             *
             * @returns     the list of items to insert.
             */
            auto createFavourite(QVariantMap favourite) -> QList<QStandardItem *>;

        private:
            /**
             * @brief       Updates the buttons to reflect the current state of the editor.
             */
            auto updateButtons() -> void;

            /**
             * @brief       Applies any changes and saves them.
             */
            auto applyChanges() -> void;

            /**
             * @brief       This slot is called when an item has been selected for editing.
             *
             * @param[in]   index the model index of the edited item.
             */
            Q_SLOT void onEditFavourite(const QModelIndex &index);

            /**
             * @brief       This slot is called when the cancel button is clicked.
             *
             * @param[in]   checked true if the button is checked; otherwise false.
             */
            Q_SLOT void onCancelClicked(bool checked);

            /**
             * @brief       This slot is called when the ok button is clicked.
             *
             * @param[in]   checked true if the button is checked; otherwise false.
             */
            Q_SLOT void onOkClicked(bool checked);

        private:
            Ui::FavouritesManagerDialog *ui;

            QStandardItemModel *m_itemModel;
            bool m_modelDirty;
    };
}

#endif //NEDRYSOFT_FAVOURITESMANAGERDIALOG_H
