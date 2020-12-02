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
#include <QJsonArray>

Nedrysoft::ComponentSystem::Component::Component() {
    m_isLoaded = false;
    m_loadStatus = 0;
}

Nedrysoft::ComponentSystem::Component::Component(QString name, QString filename, QJsonObject metadata) {
    m_name = std::move(name);
    m_filename = std::move(filename);
    m_metadata = std::move(metadata);
    m_isLoaded = false;
    m_loadStatus = 0;
}

void Nedrysoft::ComponentSystem::Component::addDependency(Component *dependency, QVersionNumber versionNumber) {
    m_dependencyVersions[dependency] = std::move(versionNumber);
    m_dependencies.append(dependency);
}

QString Nedrysoft::ComponentSystem::Component::name() {
    return m_name;
}

QString Nedrysoft::ComponentSystem::Component::filename() {
    return m_filename;
}

QJsonObject Nedrysoft::ComponentSystem::Component::metadata() {
    return m_metadata;
}

bool Nedrysoft::ComponentSystem::Component::isLoaded() {
    return m_isLoaded;
}

int Nedrysoft::ComponentSystem::Component::loadStatus() {
    return m_loadStatus;
}

QStringList Nedrysoft::ComponentSystem::Component::missingDependencies() {
    return m_missingDependencies;
}

QVersionNumber Nedrysoft::ComponentSystem::Component::version() {
    auto componentMetadata = m_metadata["MetaData"].toObject();
    auto componentVersion = componentMetadata["Version"].toString();

    return QVersionNumber::fromString(componentVersion);
}

QString Nedrysoft::ComponentSystem::Component::versionString() {
    auto componentMetadata = m_metadata["MetaData"].toObject();
    auto componentVersion = componentMetadata["Version"].toString();
    auto componentBranch = componentMetadata["Branch"].toString();
    auto componentRevision = componentMetadata["Revision"].toString();

    return QString("%1-%2 (%3)").arg(componentVersion).arg(componentBranch).arg(componentRevision);
}

QString Nedrysoft::ComponentSystem::Component::identifier() {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return ( componentMetadata["Name"].toString() + "." + componentMetadata["Vendor"].toString()).toLower();
}

QString Nedrysoft::ComponentSystem::Component::category() {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return componentMetadata["Category"].toString();
}

QString Nedrysoft::ComponentSystem::Component::vendor() {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return componentMetadata["Vendor"].toString();
}

QString Nedrysoft::ComponentSystem::Component::license() {
    QString licenseText;
    auto componentMetadata = m_metadata["MetaData"].toObject();

    auto licenseContent = componentMetadata["License"].toArray();

    for (auto object : licenseContent) {
        licenseText += object.toString() + "\r\n";
    }

    return licenseText;
}

QString Nedrysoft::ComponentSystem::Component::copyright() {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return componentMetadata["Copyright"].toString();
}

QString Nedrysoft::ComponentSystem::Component::description() {
    QString descriptionText;
    auto componentMetadata = m_metadata["MetaData"].toObject();

    auto licenseContent = componentMetadata["Description"].toArray();

    for (auto object : licenseContent) {
        descriptionText += object.toString() + "\r\n";
    }

    return descriptionText;
}

QString Nedrysoft::ComponentSystem::Component::url() {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return componentMetadata["Url"].toString();
}

QString Nedrysoft::ComponentSystem::Component::dependencies() {
    QString dependencyText;
    auto componentMetadata = m_metadata["MetaData"].toObject();

    auto licenseContent = componentMetadata["Dependencies"].toArray();

    for (auto object : licenseContent) {
        auto dependency = object.toObject();

        dependencyText += QString("%1 (%2)\r\n").arg(dependency["Name"].toString()).arg(
                dependency["Version"].toString());
    }

    return dependencyText;
}

bool Nedrysoft::ComponentSystem::Component::canBeDisabled() {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    if (componentMetadata.contains("CanBeDisabled")) {
        return componentMetadata["CanBeDisabled"].toBool();
    }

    return true;
}

void Nedrysoft::ComponentSystem::Component::validateDependencies() {
    for (auto dependency : m_dependencies) {
        if (!dependency->isLoaded()) {
            m_loadStatus |= Nedrysoft::ComponentSystem::ComponentLoader::MissingDependency;
        } else {
            if (dependency->version() < m_dependencyVersions[dependency]) {
                m_loadStatus |= Nedrysoft::ComponentSystem::ComponentLoader::IncompatibleVersion;
            }
        }
    }
}


