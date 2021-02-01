/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/03/2020.
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

#ifndef NEDRYSOFT_MAPWIDGET_MAPWIDGET_H
#define NEDRYSOFT_MAPWIDGET_MAPWIDGET_H

#include <QGridLayout>
#include <QWidget>

#if ( defined(NEDRYSOFT_LIBRARY_MAPWIDGET_EXPORT))
#define NEDRYSOFT_MAPWIDGET_DLLSPEC Q_DECL_EXPORT
#else
#define NEDRYSOFT_MAPWIDGET_DLLSPEC Q_DECL_IMPORT
#endif

class QQuickWidget;

namespace Nedrysoft::MapWidget {
    /**
     * @brief       THe MapWidget is a bridge from the QML map widget to a QWidget.
     */
    class NEDRYSOFT_MAPWIDGET_DLLSPEC MapWidget :
            public QWidget {

        public:
            /**
             * @brief       Constructs a MapWidget that is a child of the parent.
             *
             * @param       parent the parent widget.
             */
            MapWidget(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the MapWidget.
             */
            ~MapWidget();

        private:
            QQuickWidget *m_mapWidget;
            QGridLayout *m_mapLayout;
    };
}

#endif // NEDRYSOFT_MAPWIDGET_MAPWIDGET_H
