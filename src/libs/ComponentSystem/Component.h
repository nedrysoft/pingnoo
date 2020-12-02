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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_COMPONENT_H
#define NEDRYSOFT_COMPONENTSYSTEM_COMPONENT_H

#include "ComponentSystemSpec.h"
#include <QString>
#include <QJsonObject>
#include <QVersionNumber>
#include <QList>
#include <QMap>
#include <QDataStream>

namespace Nedrysoft::ComponentSystem {
    /**
     * @brief           Component
     *
     * @details         Class to hold the information about a component
     *
     */
    class COMPONENT_SYSTEM_DLLSPEC Component {
        public:
            /**
             * @brief       Constructor
             *
             */
            Component();

            /**
             * @brief       Constructor
             *
             * @details     Overriden constructor, creates a component with the given
             *              information.
             *
             * @param[in]   name            The name of the component
             * @param[in]   filename        The filename of the component
             * @param[in]   metadata        The metadata from the component file
             *
             */
            Component(QString name, QString filename, QJsonObject metadata);

            /**
             * @brief       addDependency
             *
             * @details     Adds a component as a dependency of this component
             *
             * @param[in]   dependency      The required dependency
             * @param[in]   versionNumber   The required dependency version
             *
             */
            void addDependency(Component *dependency, QVersionNumber versionNumber);

            /**
             * @brief       name
             *
             * @details     Returns the name of the component
             *
             * @return      The component name
             *
             */
            QString name();

            /**
             * @brief       filename
             *
             * @details     Returns the filename of the component
             *
             * @return      The component filename
             *
             */
            QString filename();

            /**
             * @brief       metadata
             *
             * @details     Returns the metadata of the component
             *
             * @return      The component metadata
             *
             */
            QJsonObject metadata();

            /**
             * @brief       name
             *
             * @details     Returns the loaded state of the component
             *
             * @return      True if loaded, false otherwise
             *
             */
            bool isLoaded();

            /**
             * @brief       loadStatus
             *
             * @details     Returns a bit flag of (ComponentLoader::LoadStatus values) the load status
             *
             * @return      The reason the component was not loaded
             *
             */
            int loadStatus();

            /**
             * @brief       missingDependencies
             *
             * @details     The list of any missing dependencies, as these will
             *              not be available from the ComponentLoader, this can be
             *              used to show the names of any missing dependencies.
             *
             * @return      The list of missing dependencies
             *
             */
            QStringList missingDependencies();

            /**
             * @brief       version
             *
             * @details     Returns the version number of the component
             *
             * @return      The version
             *
             */
            QVersionNumber version();

            /**
             * @brief       versionString
             *
             * @details     Returns the formatted version string of the component
             *
             * @return      formatted version string
             *
             */
            QString versionString();

            /**
             * @brief       identifier
             *
             * @details     Returns the reverse dns identifier of the component
             *
             * @return      The identifer
             *
             */
            QString identifier();

            /**
             * @brief       category
             *
             * @details     Returns category of the component
             *
             * @return      The section
             *
             */
            QString category();

            /**
             * @brief       vendor
             *
             * @details     Returns the vendor of the component
             *
             * @return      The vendor
             *
             */
            QString vendor();

            /**
             * @brief       license
             *
             * @details     Returns the license text of the component
             *
             * @return      The license text
             *
             */
            QString license();

            /**
             * @brief       copyright
             *
             * @details     Returns the copyright text of the component
             *
             * @return      The copyright text
             *
             */
            QString copyright();

            /**
             * @brief       description
             *
             * @details     Returns the description text of the component
             *
             * @return      The description text
             *
             */
            QString description();

            /**
             * @brief       url
             *
             * @details     Returns the URL of the component
             *
             * @return      The URL
             *
             */
            QString url();

            /**
             * @brief       dependencies
             *
             * @details     Returns the dependencies of the component
             *
             * @return      The dependencies
             *
             */
            QString dependencies();

            /**
             * @brief       canBeDisabled
             *
             * @details     Returns whether the component can be disabled by the user
             *
             * @return      State
             *
             */
            bool canBeDisabled();

            /**
             * @brief       validateDependencies
             *
             * @details     Validates all dependencies to ensure they are loaded
             *              and ensures that the loaded version meets our minimum
             *              requirement.
             *
             */
            void validateDependencies();

            friend class ComponentLoader;

        private:
            QString m_name;
            QString m_filename;
            QList<Nedrysoft::ComponentSystem::Component *> m_dependencies;
            QJsonObject m_metadata;
            bool m_isLoaded;
            int m_loadStatus;
            QList<QString> m_missingDependencies;
            QMap<Nedrysoft::ComponentSystem::Component *, QVersionNumber> m_dependencyVersions;
    };
}

Q_DECLARE_METATYPE(Nedrysoft::ComponentSystem::Component *)

#endif // NEDRYSOFT_COMPONENTSYSTEM_COMPONENT_H
