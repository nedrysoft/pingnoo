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

#ifndef NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKERCOMPONENT_H
#define NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKERCOMPONENT_H

#include "PublicIPHostMaskerSpec.h"
#include "ComponentSystem/IComponentInterface.h"
#include "PublicIPHostMasker.h"

/**
 * @brief       PublicIPHostMaskerComponent class
 *
 * @details     This component provides a host masker implementation which
 *              determines the public IP of the host and then masks this appropriately
 *              where necessary.  This allows outputs to hide sensitive information.
 *
 */
class NEDRYSOFT_PUBLICIPHOSTMASKER_DLLSPEC PublicIPHostMaskerComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponentInterface {
    Q_OBJECT
        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")
        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponentInterface)

    public:
        /**
         * @brief       Constructor
         *
         */
        PublicIPHostMaskerComponent();

        /**
         * @brief       Destructor
         *
         */
        ~PublicIPHostMaskerComponent();

        /**
         * @sa          Nedrysoft::ComponentSystem::IComponentInterface
         *
         */
        virtual void initialiseEvent();

    private:

        Nedrysoft::PublicIPHostMasker::PublicIPHostMasker *m_hostMasker;
};

#endif // NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKERCOMPONENT_H
