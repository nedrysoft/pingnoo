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

#ifndef NEDRYSOFT_RIBBONBARMANAGER_H
#define NEDRYSOFT_RIBBONBARMANAGER_H

#include "IRibbonBarManager.h"
#include "Ribbon/RibbonWidget.h"

#include <QMap>

namespace Nedrysoft::Core {
    class RibbonPage;
    /**
     * @brief       The RibbonBarManager implements IRibbonBarManager using Nedrysoft::Ribbon as the implementation.
     */
    class RibbonBarManager :
            public Nedrysoft::Core::IRibbonBarManager {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IRibbonBarManager)

        public:
            /**
             * @brief       Constructs a RibbonBarManager with the supplied Ribbon Widget.
             *
             * @param[in]   ribbonWidget the ribbon widget.
             */
            RibbonBarManager(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget=nullptr);

            /**
             * @brief       Destroys the RibbonBarManager.
             */
            ~RibbonBarManager();

            /**
             * @brief       Adds a page to the ribbon bar.
             *
             * @param[in]   title the title of the page.
             * @param[in]   id the identifier of the page.
             * @param[in]   order the order of the page, a unit value between 0-1.
             *
             * @note        Pages are inserted according to the order parameter, where 0 would be the start,
             *              0.5 the middle and 1 the end.  Where items have the same order value, position
             *              is decided alphabetically.
             *
             * @returns     the IRibbonPage instance of the page.
             */
            auto addPage(QString title, QString id, float order=1) -> Nedrysoft::Core::IRibbonPage * override;

            /**
             * @brief       Returns the page given by the id.
             *
             * @see         Nedrysoft::Core::IRibbonBarManager::addPage.
             *
             * @param[in]   id the identifier of the page.
             *
             * @returns     if it exists, the IRibbonPage instance of the page; otherwise nullptr.
             */
            auto page(QString id) -> Nedrysoft::Core::IRibbonPage * override;

            /**
             * @brief       Switches the current page to the one with the given identifier.
             *
             * @param[in]   id the identifier of the page.
             *
             * @returns     true if page was switched; otherwise false.
             */
            auto selectPage(QString id) -> bool override;

        public:
            /**
             * @brief       Attaches the ribbon to this instance.
             *
             * @param[in]   ribbonWidget the ribbon widget.
             */
            auto setRibbonWidget(Nedrysoft::Ribbon::RibbonWidget *ribbonWidget) -> void;

            /**
             * @brief       Called when a group is added to a page.
             *
             * @notes       Pages are by default hidden and are only visible when groups are added.
             *
             * @param[in]   page the page that has had a group added.
             */
            auto groupAdded(Nedrysoft::Core::RibbonPage *page) -> void;

        private:
            Nedrysoft::Ribbon::RibbonWidget *m_ribbonWidget;
            QMap<QString, Nedrysoft::Core::RibbonPage *> m_pages;
    };
}

#endif //NEDRYSOFT_RIBBONBARMANAGER_H
