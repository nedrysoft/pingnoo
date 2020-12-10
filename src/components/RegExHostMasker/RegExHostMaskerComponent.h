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

#ifndef NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKERCOMPONENT_H
#define NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKERCOMPONENT_H

#include "RegExHostMaskerSpec.h"
#include "ComponentSystem/IComponent.h"

namespace Nedrysoft::RegExHostMasker {
    class RegExHostMasker;
}

/**
 * @brief       RegExHostMaskerComponent class
 *
 * @details     This component provides a host masker implementation which
 *              uses user supplied regular expressions to mask out IP addresses
 *              or host names.  This allows outputs to hide sensitive information.
 */
class NEDRYSOFT_REGEXHOSTMASKER_DLLSPEC RegExHostMaskerComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT

        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")

        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        /**
         * @brief       Constructor
         */
        RegExHostMaskerComponent();

        /**
         * @brief       Destructor
         */
        ~RegExHostMaskerComponent();

        /**
         * @sa          Nedrysoft::ComponentSystem::IComponent
         */
        virtual void initialiseEvent();

    private:
        QList<Nedrysoft::RegExHostMasker::RegExHostMasker *> m_maskerList;

};

#endif // NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKERCOMPONENT_H
