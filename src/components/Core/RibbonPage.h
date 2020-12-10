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

#ifndef NEDRYSOFT_RIBBONPAGE_H
#define NEDRYSOFT_RIBBONPAGE_H

#include "IRibbonPage.h"
#include "IRibbonGroup.h"

#include <QHBoxLayout>
#include <QWidget>

namespace Nedrysoft::Core {
    /**
     * @brief       The RibbonPage describes a ribbon bar page.
     *
     * @details     The RibbonPage is a container for groups.  Pages are added to the ribbon bar and each page
     *              appears as a separate tab in the ribbon bar.
     */
    class RibbonPage :
        public Nedrysoft::Core::IRibbonPage {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IRibbonPage);

        public:
            /**
             * @brief       Adds a group to the page.
             *
             * @param[in]   title the title of the group.
             * @param[in]   id the identifier of the group.
             *
             * @returns     the IRibbonGroup instance of the group.
             */
            Nedrysoft::Core::IRibbonGroup *addGroup(QString title, QString id, QWidget *widget) override;

        public:
            /**
             * @brief       Constructs a RibbonPage instance.
             */
            RibbonPage();

            /**
             * @brief       Returns the pages widget.
             *
             * @returns     the page widget.
             */
            QWidget *widget();

        private:
            QWidget *m_pageWidget;
            QHBoxLayout *m_pageLayout;
    };
};

#endif //NEDRYSOFT_RIBBONPAGE_H
