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

#ifndef NEDRYSOFT_RIBBONBARMANAGER_H
#define NEDRYSOFT_RIBBONBARMANAGER_H

#include "IRibbonBarManager.h"
#include "Ribbon/RibbonWidget.h"

#include <QMap>

namespace Nedrysoft::Core {
    class RibbonPage;
    /**
     * @brief       Implementation of IRibbonBarManager using Nedrysoft::Ribbon
     */
    class RibbonBarManager :
            public Nedrysoft::Core::IRibbonBarManager {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IRibbonBarManager)

        public:
            /**
             * @brief       Constructs a RibbonBarManager
             */
            RibbonBarManager();

            /**
             * @brief       Constructs a RibbonBarManager with the supplied Ribbon Widget
             *
             * @param[in]   ribbonWidget the ribbon widget
             */
            RibbonBarManager(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget);

            /**
             * @brief       Adds a page to the ribbon bar
             *
             * @see         Nedrysoft::Core::IRibbonBarManager::addPage
             *
             * @param[in]   title the title of the page.
             * @param[in]   id the identifier of the page.
             *
             * @returns     the IRibbonPage instance of the page.
             */
            Nedrysoft::Core::IRibbonPage *addPage(QString title, QString id) override;

            /**
             * @brief       Returns the page given by the id
             *
             * @see         Nedrysoft::Core::IRibbonBarManager::addPage
             *
             * @param[in]   id the identifier of the page.
             *
             * @returns     if it exists, the IRibbonPage instance of the page; otherwise nullptr.
             */
            Nedrysoft::Core::IRibbonPage *page(QString id) override;

        public:
            /**
             * @brief       Attaches the ribbon to this instance.
             *
             * @param[in]   ribbonWidget the ribbon widget.
             */
            void setRibbonWidget(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget);

        private:
            Nedrysoft::Ribbon::RibbonWidget *m_ribbonWidget;
            QMap<QString, Nedrysoft::Core::RibbonPage *> m_pages;
    };
}

#endif //NEDRYSOFT_RIBBONBARMANAGER_H
