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

#include "Component.h"
#include "ComponentLoader.h"
#include <QDebug>

FizzyAde::ComponentSystem::Component::Component()
{
    m_isLoaded = false;
    m_loadError = 0;
}

FizzyAde::ComponentSystem::Component::Component(QString name, QString filename, QJsonObject metadata)
{
    m_name = std::move(name);
    m_filename = std::move(filename);
    m_metadata = std::move(metadata);
    m_isLoaded = false;
    m_loadError = 0;
}

void FizzyAde::ComponentSystem::Component::addDependency(Component *dependency, QVersionNumber versionNumber)
{
    m_dependencyVersions[dependency] = std::move(versionNumber);
    m_dependencies.append(dependency);
}

QString FizzyAde::ComponentSystem::Component::name()
{
    return(m_name);
}

QString FizzyAde::ComponentSystem::Component::filename()
{
    return(m_filename);
}

QJsonObject FizzyAde::ComponentSystem::Component::metadata()
{
    return(m_metadata);
}

bool FizzyAde::ComponentSystem::Component::isLoaded()
{
    return(m_isLoaded);
}

int FizzyAde::ComponentSystem::Component::loadError()
{
    return(m_loadError);
}

QStringList FizzyAde::ComponentSystem::Component::missingDependencies()
{
    return(m_missingDependencies);
}

QVersionNumber FizzyAde::ComponentSystem::Component::version()
{
    auto componentMetadata = m_metadata["MetaData"].toObject();
    auto componentVersion = componentMetadata["Version"].toString();

    return(QVersionNumber::fromString(componentVersion));
}

void FizzyAde::ComponentSystem::Component::validateDependencies()
{
    for(auto dependency : m_dependencies) {
        if (!dependency->isLoaded()) {
            m_loadError |= FizzyAde::ComponentSystem::ComponentLoader::MissingDependency;
        } else {
            if (dependency->version()<m_dependencyVersions[dependency]) {
                m_loadError |= FizzyAde::ComponentSystem::ComponentLoader::IncompatibleVersion;
            }
        }
    }
}


