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

#ifndef NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKERCOMPONENT_H
#define NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKERCOMPONENT_H

#include "ComponentSystem/IComponent.h"
#include "PublicIPHostMasker.h"
#include "PublicIPHostMaskerSpec.h"

namespace Nedrysoft::PublicIPHostMasker {
    class PublicIPHostMasker;
    class PublicIPHostMaskerSettingsPage;
}


/**
 * @brief       The PublicIPHostMaskerComponent class provides a host masker for the clients public ip.
 */
class NEDRYSOFT_PUBLICIPHOSTMASKER_DLLSPEC PublicIPHostMaskerComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT
        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")

        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        /**
         * @brief       Constructs a PublicIPHostMaskerComponent.
         */
        PublicIPHostMaskerComponent();

        /**
         * @brief       Destroys the PublicIPHostMaskerComponent.
         */
        ~PublicIPHostMaskerComponent();

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

    private:

        Nedrysoft::PublicIPHostMasker::PublicIPHostMasker *m_hostMasker;
        Nedrysoft::PublicIPHostMasker::PublicIPHostMaskerSettingsPage *m_settingsPage;
};

#endif // NEDRYSOFT_PUBLICIPHOSTMASKER_PUBLICIPHOSTMASKERCOMPONENT_H
