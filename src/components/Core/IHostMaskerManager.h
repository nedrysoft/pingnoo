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

#ifndef PINGNOO_COMPONENTS_CORE_IHOSTMASKERMANAGER_H
#define PINGNOO_COMPONENTS_CORE_IHOSTMASKERMANAGER_H

#include "CoreSpec.h"
#include "IComponentManager.h"

#include <IInterface>
#include <QObject>

namespace Nedrysoft { namespace Core {
    NEDRYSOFT_CORE_DLLSPEC Q_NAMESPACE

    class IHostMasker;

    enum class HostMaskType {
        Screen,
        Output,
        Clipboard
    };

    Q_ENUM_NS(HostMaskType)

    /**
     * @brief       The IHostMaskerManager class represents an object that manages host maskers that are exposed
     *              to the application.  This is purely is simple thing wrapper that provices a covenience function
     *              which returns all the host masker engines and also a method of checking if host masking is
     *              enabled for a given type of output.
     *
     * @class       Nedrysoft::Core::IHostMaskerManager IHostMaskerManager.h <IHostMaskerManager>
     */
    class NEDRYSOFT_CORE_DLLSPEC IHostMaskerManager :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       Returns the Nedrysoft::Core::IHostMaskerManager instance.
             */
            static auto getInstance() -> IHostMaskerManager * {
                return ComponentSystem::getObject<IHostMaskerManager>();
            }

            /**
             * @brief       Check if the host masker is enabled for a given type.
             *
             * @param[in]   type the type of output that we are requesting the enabled state for.
             *
             * @returns     zero if no error occurred; otherwise non-zero.
             */
            virtual auto enabled(Nedrysoft::Core::HostMaskType type) -> bool = 0;

            /**
             * @brief       Set whether the host masker should be used for a given type.
             *
             * @param[in]   type the type of output that we are requesting the enabled state for.
             * @param[in]   enabled true if the masker for the type is enabled; otherwise false.
             *
             * @returns     zero if no error occurred; otherwise non-zero.
             */
            virtual auto setEnabled(HostMaskType type, bool enabled) -> void = 0;

            /**
             * @brief       Registers a host masker with this manager.
             *
             * @param[in]   hostMasker the host masker to register.
             */
            virtual auto add(Nedrysoft::Core::IHostMasker *hostMasker) -> void = 0;

            /**
             * @brief       Removes a host masker that was previously registered with this manager.
             *
             * @param[in]   hostMasker the host masker to remove.
             */
            virtual auto remove(Nedrysoft::Core::IHostMasker *hostMasker) -> void = 0;

            /**
             * @brief       Returns a list of the available host maskers.
             *
             * @returns     A list of IHostMaskers.
             */
             virtual auto maskers() -> QList<Nedrysoft::Core::IHostMasker *> = 0;

            // Classes with virtual functions should not have a public non-virtual destructor:
            virtual ~IHostMaskerManager() = default;

        public:
            /**
             * @brief       This signal is emitted when the state of masking is changed for the given type.
             * @param[in]   type the type of masking being changed.
             * @parampin]   state true to mask the type; otherwise false.
             */
            Q_SIGNAL void maskStateChanged(Nedrysoft::Core::HostMaskType type, bool state);
    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::Core::IHostMaskerManager, "com.nedrysoft.core.IHostMaskerManager/1.0.0")

#endif // PINGNOO_COMPONENTS_CORE_IHOSTMASKERMANAGER_H
