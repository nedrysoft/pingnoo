/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo
 *
 * Created by Adrian Carpenter on 11/12/2020.
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

#ifndef NEDRYSOFT_NEWTARGETRIBBONGROUP_H
#define NEDRYSOFT_NEWTARGETRIBBONGROUP_H

#include "LineSyntaxHighlighter.h"

#include <QWidget>

namespace Nedrysoft::Ribbon {
    class RibbonLineEdit;
}

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class NewTargetRibbonGroup;
    }
    /**
     * @brief       The NewTargetRibbonGroup is a Ribbon group that allows a new target to be created.
     */
    class NewTargetRibbonGroup :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new NewTargetRibbonGroup instance.
             */
            explicit NewTargetRibbonGroup(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the NewTargetRibbonGroup.
             */
            ~NewTargetRibbonGroup() override;

        private:
            /**
             * @brief       Validate the fields of the dialog.
             *
             * @param[out]  string the error string if validation failed.
             *
             * @returns     nullptr on no error; otherwise the first widget that failed validation.
             */
            QWidget *checkFieldsValid(QString &string);

            /**
             * @brief       Validates the fields of the dialog and enables/disables the ok/apply buttons.
             */
            void validateFields();

        private:
            Ui::NewTargetRibbonGroup *ui;

            LineSyntaxHighlighter *m_targetHighlighter;
            LineSyntaxHighlighter *m_intervalHighlighter;
    };
};

#endif //NEDRYSOFT_NEWTARGETRIBBONGROUP_H
