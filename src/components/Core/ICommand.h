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

#ifndef FIZZYADE_CORE_ICOMMAND_H
#define FIZZYADE_CORE_ICOMMAND_H

#include "CoreSpec.h"
#include "ComponentSystem/IInterface.h"
#include <QObject>
#include <QAction>
#include <QAbstractButton>
#include <QDebug>

namespace FizzyAde::Core
{
    /**
     * ICommand interface
     *
     * ICommand represents an actionable command in the system, commands
     * are bound to QActions for given contexts, this allows the target of
     * the command to change depending on the current context that the application
     * is in.
     *
     */

    class FIZZYADE_CORE_DLLSPEC ICommand :
        public FizzyAde::ComponentSystem::IInterface
    {
        Q_OBJECT

    public:
        /**
         * Returns the proxy action
         *
         * @return returns the proxy action
         *
         */
        virtual QAction *action() = 0;

        /**
         * Sets the active state of the command
         *
         * @param[in] state true if enabled, else false
         *
         */
        virtual void setActive(bool state) = 0;

        /**
         * Returns the active state of the command
         *
         * @return state if enabled, else false
         *
         */
        virtual bool active() = 0;

    public:
        /**
         * Attaches a command to an abstract push button
         *
         * Binds to the buttons signals and then emits the appropraite
         * signals from the push button
         *
         * @param[in] widget the abstract button subclassed widget
         *
         */
        virtual void attachToWidget(QAbstractButton *widget)
        {
            connect(widget, &QAbstractButton::clicked, [this] (bool) {
                this->action()->trigger();
            });

            connect(this->action(), &QAction::changed, [this, widget] {
                widget->setEnabled(this->active());
                qDebug() << "set widget enabled" << this->active();
            });
        }

    public:

    };
}

Q_DECLARE_INTERFACE(FizzyAde::Core::ICommand, "com.fizzyade.core.ICommand/1.0.0")

#endif // FIZZYADE_CORE_ICOMMAND_H
