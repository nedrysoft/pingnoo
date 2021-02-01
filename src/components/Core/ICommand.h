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

#ifndef NEDRYSOFT_CORE_ICOMMAND_H
#define NEDRYSOFT_CORE_ICOMMAND_H

#include "ComponentSystem/IInterface.h"
#include "CoreSpec.h"

#include <QAbstractButton>
#include <QAction>
#include <QObject>

namespace Nedrysoft::Core {
    /**
     * @brief       ICommand interface
     *
     * @details     ICommand represents an actionable command in the system, commands
     *              are bound to QActions for given contexts, this allows the target of
     *              the command to change depending on the current context that the application
     *              is in.
     */
    class NEDRYSOFT_CORE_DLLSPEC ICommand :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Returns the proxy action.
             *
             * @returns     the proxy action
             */
            virtual auto action() -> QAction * = 0;

            /**
             * @brief       Sets the active state of the command.
             *
             * @param[in]   state true if active; otherwise false.
             */
            virtual auto setActive(bool state) -> void = 0;

            /**
             * @brief       Returns the active state of the command.
             *
             * @returns     true if enabled; otherwise false.
             */
            virtual auto active() -> bool = 0;

        public:
            /**
             * @brief       Attaches a command to an abstract push button
             *
             * @details     Binds to the buttons signals and then emits the appropriate
             *              signals from the push button
             *
             * @param[in]   widget the abstract button subclassed widget
             */
            virtual auto attachToWidget(QAbstractButton *widget) -> void {
                connect(widget, &QAbstractButton::clicked, [this](bool) {
                    this->action()->trigger();
                });

                connect(this->action(), &QAction::changed, [this, widget] {
                    widget->setEnabled(this->active());
                });
            }
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::Core::ICommand, "com.nedrysoft.core.ICommand/1.0.0")

#endif // NEDRYSOFT_CORE_ICOMMAND_H
