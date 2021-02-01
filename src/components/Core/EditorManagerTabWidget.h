/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 31/12/2020.
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

#ifndef NEDRYSOFT_EDITORMANAGERTABWIDGET_H
#define NEDRYSOFT_EDITORMANAGERTABWIDGET_H

#include <QTabWidget>

namespace Nedrysoft::Core {
    /**
     * @brief       The EditorManagerTabWidget provides a tab editor which has text on the background
     *              when no tabs are open.
     */
    class EditorManagerTabWidget :
            public QTabWidget {

        public:
            /**
             * @brief       Constructs a new EditorManagerTabWidget that is a child of the parent.
             *
             * @param[in]   parent the parent widget.
             */
            EditorManagerTabWidget(QWidget *parent=nullptr);

            /**
             * @brief       Sets the text to draw on the background.
             *
             * @param[in]   text the text to display.
             */
            auto setText(QString text) -> void;

        protected:
            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @param[in]   event the event information.
             */
            void paintEvent(QPaintEvent *event) override;

        private:
            QString m_text;
            int m_openSansRegular;
    };
}

#endif //NEDRYSOFT_EDITORMANAGERTABWIDGET_H
