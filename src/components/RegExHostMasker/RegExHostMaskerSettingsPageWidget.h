/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/02/2021.
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

#ifndef NEDRYSOFT_REGEXHOSTMASKERSETTINGSPAGEWIDGET_H
#define NEDRYSOFT_REGEXHOSTMASKERSETTINGSPAGEWIDGET_H

#include <QWidget>

#include "RegExHostMasker.h"

class QTreeWidgetItem;
class QCheckBox;

namespace Nedrysoft::RegExHostMasker {
    namespace Ui {
        class RegExHostMaskerSettingsPageWidget;
    }

    /**
     * @brief       The RegExHostMaskerSettingsPageWidget class provides the page to configure the regular expression
     *              host masker.
     */
    class RegExHostMaskerSettingsPageWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new RegExHostMaskerSettingsPageWidget which is a child of parent.
             *
             * @param[in]   parent the parent of this child.
             */
            explicit RegExHostMaskerSettingsPageWidget(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the RegExHostMaskerSettingsPageWidget.
             */
            ~RegExHostMaskerSettingsPageWidget() override;

            /**
             * @brief       Returns whether the current settings can be accepted.
             *
             * @return      true if settings are valid; otherwise false.
             */
            auto canAcceptSettings() -> bool;

            /**
             * @brief       Accepts the current configuration.
             */
            auto acceptSettings() -> void;

        private:
            /**
             * @brief       Updates the widgets for the selected item.
             *
             * @param[in]   item the item to display (nullptr if none)
             */
            auto updateWidgets(QTreeWidgetItem *item) -> void;

            /**
             * @brief       Called when the data is changed by the user.
             *
             * @note        Under Windows and Linux this function does nothing, on macOS it applies the changes
             *              immediately, this is standard macOS behavior.
             */
            auto updateSettings() -> void;

            /**
             * @brief       Adds an expression configuration to the list.
             *
             * @param[in]   item the item to add.
             */
            auto addExpression(Nedrysoft::RegExHostMasker::RegExHostMaskerItem masker) -> void;

            /**
             * @brief       Adds a checkbox cell to a treeview.
             *
             * @param[in]   isChecked the initial state of the checkbox.
             *
             * @returns     the checkbox widget.
             */
            auto addCheckBox(bool isChecked) -> QCheckBox *;

            /**
             * @brief       Returns the checkbox from the container.
             *
             * @param[in]   container the container that has the checkbox.
             *
             * @returns     the checkbox.
             */
            auto getCheckBox(QWidget *container) -> QCheckBox *;

            /**
             * @brief       Updates the checkbox in the table to the given state.
             *
             * @param[in]   item the tree view item.
             * @param[in]   column the checkbox column to modify.
             * @param[in]   isChecked true if item is checked; otherwise false.
             */
            auto updateCheckBox(QTreeWidgetItem *item, int column, bool isChecked) -> void;

            /**
             * @brief       Returns the masker item for the tree item.
             *
             * @param[in]   item the tree widget item.
             *
             * @returns     the masker item.
             */
            auto getMaskerItem(QTreeWidgetItem *item) -> RegExHostMaskerItem;

            /**
             * @brief       Sets the masker item for the tree item
             *
             * @param[in]   item the tree widget item to set.
             *
             * @param[in]   maskerItem the masker item.
             */
            auto setMaskerItem(QTreeWidgetItem *item, RegExHostMaskerItem maskerItem) -> void;

        public:
            /**
             * @brief       Reimplements: QWidget::sizeHint().
             *
             * @returns     the preferred size for the widdget.
             */
            QSize sizeHint() const override;

        private:
            Ui::RegExHostMaskerSettingsPageWidget *ui;

            bool m_loadingConfiguration;
    };
}

#endif //NEDRYSOFT_REGEXHOSTMASKERSETTINGSPAGEWIDGET_H
