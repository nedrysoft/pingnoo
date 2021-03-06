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

#ifndef NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKERCOMPONENT_H
#define NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKERCOMPONENT_H

#include "RegExHostMaskerSpec.h"
#include "ComponentSystem/IComponent.h"

namespace Nedrysoft::RegExHostMasker {
    class RegExHostMasker;
    class RegExHostMaskerSettingsPage;
}

/**
 * @brief       The RegExHostMaskerComponent class provides a host masker using a regular expression.
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
         * @brief       Constructs a RegExHostMaskerComponent.
         */
        RegExHostMaskerComponent();

        /**
         * @brief       Destroys the RegExHostMaskerComponent.
         */
        ~RegExHostMaskerComponent();

        /**
         * @brief       The initialiseEvent function is called by the component system during loading.
         *
         * @details     Called by the component loader after all components have been loaded, called in load order.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::initialiseEvent
         */
        auto initialiseEvent() -> void override;

        /**
         * @brief       The finaliseEvent function is called by the component system during shutdown.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::finaliseEvent
         */
        auto finaliseEvent() -> void override;

        /**
         * @brief       The initialisationFinishedEvent function is called by the component loader after all
         *              components have been initialised.
         *
         * @details     Called by the component loader after all components have been
         *              initialised, called in reverse load order.
         */
        auto initialisationFinishedEvent() -> void override;

    private:
        Nedrysoft::RegExHostMasker::RegExHostMasker *m_hostMasker;
        Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPage *m_settingsPage;
};

#endif // NEDRYSOFT_REGEXHOSTMASKER_REGEXHOSTMASKERCOMPONENT_H
