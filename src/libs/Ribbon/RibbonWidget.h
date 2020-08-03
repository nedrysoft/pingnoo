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

#ifndef FIZZYADE_RIBBON_RIBBONWIDGET_H
#define FIZZYADE_RIBBON_RIBBONWIDGET_H

#include "RibbonSpec.h"
#include <QTabWidget>
#include <QHBoxLayout>

namespace FizzyAde::Ribbon
{
    class RibbonTabBar;

    enum ThemeMode
    {
        Light = 0,
        Dark = 1
    };

    static constexpr QRgb TextColor[] = {                                       //! Colour of text on the control
        qRgb(73, 69, 66),
        qRgb(0xDC, 0xDC, 0xDC)
    };


    static constexpr QRgb TabBarBackgroundColor[] = {                           //! Background colour of the tab bar
        qRgb(246, 243, 247),
        qRgb(0x2d, 0x2d, 0x2d)
    };

    static constexpr QRgb TabHoverColor[] = {                                   //! Highlight colour of a tab when hovered over
        qRgb(254, 251, 255),
        TabBarBackgroundColor[Dark]
    };

    static constexpr QRgb TabSelectedColor[] = {                                //! Colour of a tab when the mouse is being held down
        qRgb(51, 84, 151),
        qRgb(0xdf, 0xdf, 0xdf)
    };

    static constexpr QRgb RibbonBorderColor[] = {                               //! Colour of the ribbon divider at the bottom of the control
        qRgb(214, 211, 207),
        qRgb(0, 0, 0)
    };

    static constexpr QRgb GroupDividerColor[] = {                               //! Colour of the group divider
        qRgb(180, 178, 173),
        qRgb(0x57, 0x57, 0x57)
    };

    static constexpr int TabBarHeight = 28;                                     //! Height of the tab bar portion of the control
    static constexpr int RibbonBarHeight = 100;                                 //! Height of the entire control
    static constexpr int TabHighlightHeight = 3;                                //! Height of the highlight mark on the selected page
    static constexpr int GroupDividerMargin = 2;                                //! Width of the group divider

    /**
     * @brief       Ribbon Bar Widget
     *
     * @details     Subclass of QTabWidget to create a lookalike of the Ribbon
     *              bar control.
     *
     */
    class RIBBON_WIDGET_EXPORT RibbonWidget :
        public QTabWidget
    {
        Q_OBJECT

    public:
        /**
         * @brief       Constructor
         *
         * @param[in]   parent          parent widget
         *
         */
        RibbonWidget(QWidget *parent = nullptr);

        friend class RibbonTabBar;
        friend class RibbonGroup;

    protected:
        /**
         * @brief       paintEvent
         *
         * @details     Overridden paintEvent for drawing widget
         *
         * @param[in]   event           The event information
         *
         */
        virtual void paintEvent(QPaintEvent *event);

        FizzyAde::Ribbon::RibbonTabBar *m_tabBar;                                  //! The tab bar associated with this ribbon
    };
}

#endif // FIZZYADE_RIBBON_RIBBONWIDGET_H
