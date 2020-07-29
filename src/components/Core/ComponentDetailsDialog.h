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

#ifndef FIZZYADE_CORE_COMPONENTDETAILSDIALOG_H
#define FIZZYADE_CORE_COMPONENTDETAILSDIALOG_H

#include <QDialog>
#include "ComponentSystem/Component.h"

namespace FizzyAde::Core
{
    namespace Ui
    {
        class ComponentDetailsDialog;
    }

    /**
     * ComponentDetailsDialog
     *
     * Dialog that shows the metadata information embedded in
     * a component.
     *
     */
    class ComponentDetailsDialog : public QDialog
    {
        Q_OBJECT

    public:
        /**
         * Constructor
         *
         * @param[in] component Pointer to the component instance to be viewed
         * @param[in] parent Parent widget
         */
        explicit ComponentDetailsDialog(FizzyAde::ComponentSystem::Component *component, QWidget *parent = nullptr);

        /**
         * Destructor
         */
        ~ComponentDetailsDialog();

    private:
        Ui::ComponentDetailsDialog *ui;                         //! The generated ui class for the dialog
    };
}

#endif // FIZZYADE_CORE_COMPONENTDETAILSDIALOG_H
