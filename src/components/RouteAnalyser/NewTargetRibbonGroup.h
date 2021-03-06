/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 10/12/2020.
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

namespace Nedrysoft::Utils {
    class ThemeSupport;
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
             * @brief       Constructs a new NewTargetRibbonGroup instance which is a child of the parent.
             *
             * @param[in]   parent the parent for this widget.
             */
            explicit NewTargetRibbonGroup(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the NewTargetRibbonGroup.
             */
            ~NewTargetRibbonGroup() override;

            /**
             * @brief       Called when light/dark mode changes
             *
             * @param[in]   isDarkMode whether dark mode is active.
             */
            Q_SLOT void updateIcons(bool isDarkMode);

        private:
            /**
             * @brief       Validate the fields of the dialog.
             *
             * @param[out]  string the error string if validation failed.
             *
             * @returns     nullptr on no error; otherwise the first widget that failed validation.
             */
            auto checkFieldsValid(QString &string) -> QWidget *;

            /**
             * @brief       Validates the fields of the dialog and enables/disables the ok/apply buttons.
             */
            auto validateFields() -> void ;

        private:
            Ui::NewTargetRibbonGroup *ui;

            LineSyntaxHighlighter *m_targetHighlighter;
            LineSyntaxHighlighter *m_intervalHighlighter;

            Nedrysoft::Utils::ThemeSupport *m_themeSupport;
    };
}

#endif //NEDRYSOFT_NEWTARGETRIBBONGROUP_H
