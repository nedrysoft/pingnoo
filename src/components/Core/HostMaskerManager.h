/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 19/06/2021.
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

#ifndef PINGNOO_COMPONENTS_CORE_HOSTMASKERMANAGER_H
#define PINGNOO_COMPONENTS_CORE_HOSTMASKERMANAGER_H

#include "CoreSpec.h"
#include "IComponentManager.h"
#include "IHostMaskerManager.h"

#include <IInterface>
#include <QObject>
#include <QMap>

namespace Nedrysoft { namespace Core {
    class IHostMasker;

    /**
     * @brief       The HostMaskerManager class represents an object that manages host maskers that are exposed
     *              to the application.  This is purely is simple thing wrapper that provices a covenience function
     *              which returns all the host masker engines and also a method of checking if host masking is
     *              enabled for a given type of output.
     */
    class NEDRYSOFT_CORE_DLLSPEC HostMaskerManager :
            public Nedrysoft::Core::IHostMaskerManager {

        private:
            Q_OBJECT
            Q_INTERFACES(Nedrysoft::Core::IHostMaskerManager)

        public:
            /**
             * @brief       Constructs a new HostMaskerManager.
             */
            HostMaskerManager();

             /**
             * @brief       Destroys the HostMaskerManager.
             */
            virtual ~HostMaskerManager();

            /**
             * @brief       Check if the host masker is enabled for a given type.
             *
             * @param[in]   type the type of output that we are requesting the enabled state for.
             *
             * @returns     zero if no error occurred; otherwise non-zero.
             */
            virtual auto enabled(Nedrysoft::Core::HostMaskType type) -> bool;

            /**
             * @brief       Set whether the host masker should be used for a given type.
             *
             * @param[in]   type the type of output that we are requesting the enabled state for.
             * @param[in]   enabled true if the masker for the type is enabled; otherwise false.
             *
             * @returns     zero if no error occurred; otherwise non-zero.
             */
            virtual auto setEnabled(HostMaskType type, bool enabled) -> void;

            /**
             * @brief       Registers a host masker with this manager.
             *
             * @param[in]   hostMasker the host masker to register.
             */
            virtual auto add(Nedrysoft::Core::IHostMasker *hostMasker) -> void;

            /**
             * @brief       Removes a host masker that was previously registered with this manager.
             *
             * @param[in]   hostMasker the host masker to remove.
             */
            virtual auto remove(Nedrysoft::Core::IHostMasker *hostMasker) -> void;

            /**
             * @brief       Returns a list of the available host maskers.
             *
             * @returns     A list of IHostMaskers.
             */
            virtual auto maskers() -> QList<Nedrysoft::Core::IHostMasker *>;

        private:
            QMap<Nedrysoft::Core::HostMaskType, bool> m_maskingState;
            QList<Nedrysoft::Core::IHostMasker *> m_maskers;
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::HostMaskerManager, "com.nedrysoft.core.HostMaskerManager/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_HOSTMASKERMANAGER_H
