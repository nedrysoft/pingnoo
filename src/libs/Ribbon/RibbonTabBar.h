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

#ifndef FIZZYADE_RIBBON_RIBBONTABBAR_H
#define FIZZYADE_RIBBON_RIBBONTABBAR_H

#include "RibbonSpec.h"
#include <QTabBar>
#include <QFont>

namespace FizzyAde::Ribbon
{
    /**
     * @brief       Ribbon Tab Bar Widget
     *
     * @details     Subclass of QTabBar to create a lookalike of the Ribbon
     *              bar tab control.
     *
     */
    class RIBBON_WIDGET_EXPORT RibbonTabBar :
        public QTabBar
    {
        Q_OBJECT

    public:
        /**
         * @brief       Constructor
         *
         * @param[in]   parent              parent widget
         *
         */
        RibbonTabBar(QWidget *parent = nullptr);

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

        /**
         * @brief       tabSizeHint
         *
         * @details     Overridden paintEvent for size widget
         *
         * @param[in]   index           The index of the tab
         *
         * @return      the calculated size hint for the tab
         *
         */
        virtual QSize tabSizeHint(int index) const;

        /**
         * @brief       eventFilter
         *
         * @details     handles widget events passed from Qt
         *
         * @param[in]   obj             object that caused the event
         * @param[in]   event           the event information
         *
         * @return      true if event has handled, otherwise false
         */
        bool eventFilter(QObject * obj, QEvent * event);

    private:
        QFont m_selectedFont;
        QFont m_normalFont;
        bool m_mouseInWidget;
    };
}
#endif // FIZZYADE_RIBBON_RIBBONTABBAR_H
