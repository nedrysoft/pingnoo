/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 19/12/2020.
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

#ifndef NEDRYSOFT_COLOURDIALOG_H
#define NEDRYSOFT_COLOURDIALOG_H

#include <QApplication>
#include <QColor>
#include <QString>

#if defined(__OBJC__)
#import <AppKit/NSColorPanel.h>
#else
#include <QColorDialog>
#endif

namespace Nedrysoft::RouteAnalyser {
    using ColourFunction = std::function<void(QColor colour)>;

    /**
     * @brief       The ColourDialog class is a wrapper around the QColorDialog
     *
     * @details     The ColourDialog tries to display an operating system native colour picker, however, under
     *              macOS the QColourDialog does not allow the title to be changed, so this wrapper uses objective c
     *              to create the colour picker dialog natively which does allow the title to be set.
     */
    class ColourDialog :
            public QObject {
        private:
            Q_OBJECT

        private:
            /**
             * @brief       Constructs a ColourDialog.
             *
             * @notes       This is private as it is a singleton, use getInstance to get the instance.
             */
            ColourDialog();

            /**
             * @brief       Destroys the ColourDialog.
             */
            ~ColourDialog();

        public:
            /**
             * @brief       Opens a non modal colour picker dialog.
             * @param[in]   initialColour the initial colour that is selected.
             * @param[in]   title the title of the dialog, this is suffixed with "Colour", ie. "Ideal Colour"
             */
            auto open(QColor initialColour, QString title, ColourFunction colourFunction) -> void;

            /**
             * @brief       Closes the ColorDialog
             */
            auto close() -> void;

            /**
             * @brief       Gets the instance of the colour dialog.
             *
             * @returns     a pointer to the singleton.
             */
            static auto getInstance() -> Nedrysoft::RouteAnalyser::ColourDialog *;

        private:
            ColourFunction *m_colourFunction;
#if defined(__OBJC__)
            id m_observer;
            NSColorPanel *m_colourPanel;
#else
            QColorDialog *m_colourDialog;
#endif
    };
}

#endif //NEDRYSOFT_COLOURDIALOG_H
