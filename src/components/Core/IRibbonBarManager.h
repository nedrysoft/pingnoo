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

#ifndef PINGNOO_COMPONENTS_CORE_IRIBBONBARMANAGER_H
#define PINGNOO_COMPONENTS_CORE_IRIBBONBARMANAGER_H

#include "CoreSpec.h"
#include "ICommand.h"
#include "IContextManager.h"

#include <IInterface>

namespace Nedrysoft { namespace Ribbon {
    class RibbonDropButton;
    class RibbonWidget;
}}

namespace Nedrysoft { namespace Core {
    class IRibbonPage;
    class RibbonAction;

    /**
     * @brief       The IRibbonBarManager interface describes an object that handles the creation of ribbon bars.
     *
     * @details     The IRibbonBarManager handles the management of the ribbon bar, it allows pages to be added
     *              to the ribbon.
     *
     * @class       Nedrysoft::Core::IRibbonBarManager IRibbonBarManager.h <IRibbonBarManager>
     */
    class NEDRYSOFT_CORE_DLLSPEC IRibbonBarManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the IRibbonBarManager instance.
             *
             * @returns     the IRibbonBarManager singleton instance.
             */
            static auto getInstance() -> IRibbonBarManager * {
                return ComponentSystem::getObject<IRibbonBarManager>();
            }

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
            virtual auto addPage(QString title, QString id, float order=1) -> Nedrysoft::Core::IRibbonPage * = 0;

            /**
             * @brief       Returns the page given by the id.
             *
             * @param[in]   id the identifier of the page.
             *
             * @returns     if it exists, the IRibbonPage instance of the page; otherwise nullptr.
             */
            virtual auto page(QString id) -> Nedrysoft::Core::IRibbonPage * = 0;

            /**
             * @brief       Switches the current page to the one with the given identifier.
             *
             * @param[in]   id the identifier of the page.
             *
             * @returns     true if page was switched; otherwise false.
             */
            virtual auto selectPage(QString id) -> bool = 0;

            /**
             * @brief       Registers a Drop Button with the manager.
             *
             * @note        This is an overloaded function, for each type of ribbon widget there should be
             *              a corresponding function to register the widget.
             *
             * @param[in]   widget the drop button widget.
             * @param[in]   commandId the command identifier for this button.
             */
            /*virtual auto registerWidget(
                Nedrysoft::Ribbon::RibbonDropButton *widget,
                QString commandId
            ) -> void = 0;*/

            virtual auto registerAction(
                RibbonAction *action,
                QString commandId,
                int contextId = Nedrysoft::Core::GlobalContext
            ) -> Nedrysoft::Core::ICommand * = 0;

            virtual auto setRibbonBar(Nedrysoft::Ribbon::RibbonWidget *widget) -> void = 0;

            // Classes with virtual functions should not have a public non-virtual destructor:
            virtual ~IRibbonBarManager() = default;
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IRibbonBarManager, "com.nedrysoft.core.IRibbonBarManager/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_IRIBBONBARMANAGER_H
