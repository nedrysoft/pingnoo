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

#include "ComponentLoader.h"
#include "Component.h"
#include "ComponentSystem/IComponentManager.h"
#include "ComponentSystem/IComponentInterface.h"
#include <QDirIterator>
#include <QLibraryInfo>
#include <QLibrary>
#include <QPluginLoader>
#include <QVariant>
#include <QDebug>
#include <QDataStream>
#include <QMap>
#include <QJsonArray>

constexpr unsigned int QtMajorBitMask = 0xFFFF0000;
constexpr unsigned int QtMajorBitShift = 16;
constexpr unsigned int QtMinorBitMask = 0x0000FF00;
constexpr unsigned int QtMinorBitShift = 8;
constexpr unsigned int QtPatchBitMask = 0x000000FF;
constexpr unsigned int QtPatchBitShift = 0;

FizzyAde::ComponentSystem::ComponentLoader::ComponentLoader(QObject *parent)
    : QObject(parent)
{
}

FizzyAde::ComponentSystem::ComponentLoader::~ComponentLoader()
{
    while(m_loadOrder.count()) {
        auto loadPair = m_loadOrder.last();

        delete loadPair.second;

        // there are issues with unloading shared libraries with QSqlDatabase, the safest solution is not to unload the plugins, we
        // leak and then let Qt/OS reclaim the memory when it exits.  No real issue.

        m_loadOrder.removeLast();
    }
}

void FizzyAde::ComponentSystem::ComponentLoader::addComponents(const QString &componentFolder)
{
    auto applicationDebugBuild = QLibraryInfo::isDebugBuild();
    auto applicationQtVersion = QLibraryInfo::version();

#if defined(Q_OS_UNIX) || ( (defined(Q_OS_WIN) && defined(__MINGW32__)) )
#if defined(QT_DEBUG)
    auto forceDebugQtLibraries = true;
#else
    auto forceDebugQtLibraries = false;
#endif
    if (forceDebugQtLibraries != applicationDebugBuild) {
        if (forceDebugQtLibraries) {
            qWarning() << tr("WARNING: Application was built with QT_DEBUG but has loaded RELEASE qt libraries, component system will load DEBUG components instead.");
        } else {
            qWarning() << tr("WARNING: Application was built with QT_NO_DEBUG but has loaded DEBUG qt libraries, component system will load RELEASE components instead.");
        }

        applicationDebugBuild = forceDebugQtLibraries;
    }
#endif

    auto dir = QDirIterator(componentFolder);

    // find compatible components, and create a list of components to consider for loading

    while(dir.hasNext()) {
        dir.next();

        auto componentFilename = dir.fileInfo().absoluteFilePath();

        if (!QLibrary::isLibrary(componentFilename)) {
            continue;
        }

        auto pluginloader = new QPluginLoader(componentFilename);

        auto metaDataObject = pluginloader->metaData();

        if (metaDataObject.isEmpty()) {
            delete pluginloader;

            continue;
        }

        auto componentMetadata = metaDataObject.value("MetaData");
        auto debugBuild = metaDataObject.value("debug");
        auto qtVersion = metaDataObject.value("version");

        if (debugBuild.isNull() || qtVersion.isNull() || componentMetadata.isNull()) {
            delete pluginloader;

            continue;
        }

        if (debugBuild!=applicationDebugBuild) {
            delete pluginloader;

            continue;
        }

        auto componentName = componentMetadata.toObject().value("Name");

        if (componentName.isNull()) {
            delete pluginloader;

            continue;
        }

        auto componentQtMajor = static_cast<int>((qtVersion.toVariant().toUInt() & QtMajorBitMask) >> QtMajorBitShift);
        auto componentQtMinor = static_cast<int>((qtVersion.toVariant().toUInt() & QtMinorBitMask) >> QtMinorBitShift);
        auto componentQtPatch = static_cast<int>((qtVersion.toVariant().toUInt() & QtPatchBitMask) >> QtPatchBitShift);

        auto componentQtVersion = QVersionNumber(componentQtMajor, componentQtMinor, componentQtPatch);

        auto component = new FizzyAde::ComponentSystem::Component(componentName.toString(), componentFilename, metaDataObject);

        if (componentQtVersion.majorVersion()!=applicationQtVersion.majorVersion()) {
            component->m_loadError |= IncompatibleQtVersion;
        }

        if (m_componentSearchList.contains(componentName.toString())) {
            component->m_loadError |= NameClash;
        }

        m_componentSearchList[componentName.toString()] = component;

        delete pluginloader;
    }
}

void FizzyAde::ComponentSystem::ComponentLoader::loadComponents()
{
    QList<FizzyAde::ComponentSystem::Component *> componentLoadList;
    QList<FizzyAde::ComponentSystem::Component *> resolvedLoadList;

    // find and add dependencies from the search

    auto componentIterator = QMapIterator<QString, FizzyAde::ComponentSystem::Component *>(m_componentSearchList);

    while(componentIterator.hasNext()) {
        componentIterator.next();

        auto component = componentIterator.value();

        if (component->m_loadError) {
            continue;
        }

        auto metadata = component->metadata().value("MetaData").toObject();

        auto dependencies = metadata.value("Dependencies").toArray();

        for (auto dependency : dependencies) {
            auto dependencyName = dependency.toObject().value("Name").toString();
            auto dependencyVersion = dependency.toObject().value("Version").toString();

            if (m_componentSearchList.contains(dependencyName))
                component->addDependency(m_componentSearchList[dependencyName], QVersionNumber::fromString(dependencyVersion));
            else {
                component->m_missingDependencies.append(dependencyName);
                component->m_loadError |= MissingDependency;
            }
        }

        if (!component->m_loadError) {
            componentLoadList.append(component);
        }
    }

    // resolve the dependencies to create a load order

    for (auto current : componentLoadList) {
        QList<FizzyAde::ComponentSystem::Component *> dependencyResolveList;

        if (!resolvedLoadList.contains(current)) {
            dependencyResolveList.clear();

            resolve(current, dependencyResolveList);

            for(auto dependency : dependencyResolveList) {
                if (!resolvedLoadList.contains(dependency)) {
                    resolvedLoadList.append(dependency);
                }
            }
        }
    }

    // load the components that we have satisfied dependencies for

    for(auto component : resolvedLoadList) {
        if (component->m_loadError) {
            continue;
        }

        component->validateDependencies();

        if (component->m_loadError) {
            continue;
        }

        // check if dependencies are loaded, if not then this component cannot be loaded

        auto pluginLoader = new QPluginLoader(component->filename());

        if (!pluginLoader->load()) {
            component->m_loadError |= FizzyAde::ComponentSystem::ComponentLoader::LoadError;

            delete pluginLoader;

            continue;
        }

        auto componentInterface = qobject_cast<FizzyAde::ComponentSystem::IComponentInterface *>(pluginLoader->instance());

        if (!componentInterface) {
            component->m_loadError |= FizzyAde::ComponentSystem::ComponentLoader::MissingInterface;

            delete pluginLoader;

            continue;
        }

        m_loadOrder.append(QPair<QPluginLoader *, Component *>(pluginLoader, component));

        component->m_isLoaded = true;
    }

    // call initialiseEvent for each component (in load order)

    for(auto componentPair : m_loadOrder) {
         auto componentInterface = qobject_cast<FizzyAde::ComponentSystem::IComponentInterface *>(componentPair.first->instance());

         componentInterface->initialiseEvent();
    }

    // call initialisationFinishedEvent for each component (in reverse load order)

    for(auto loadedComponentIterator=m_loadOrder.rbegin(); loadedComponentIterator<m_loadOrder.rend(); loadedComponentIterator++) {
        auto componentInterface = qobject_cast<FizzyAde::ComponentSystem::IComponentInterface *>(loadedComponentIterator->first->instance());

        componentInterface->initialisationFinishedEvent();
    }
}

QList<FizzyAde::ComponentSystem::Component *> FizzyAde::ComponentSystem::ComponentLoader::components()
{
    return(m_componentSearchList.values());
}

void FizzyAde::ComponentSystem::ComponentLoader::resolve(FizzyAde::ComponentSystem::Component *component, QList<FizzyAde::ComponentSystem::Component *> &resolvedList)
{
    QList<FizzyAde::ComponentSystem::Component *> processedList;

    resolve(component, resolvedList, processedList);
}

void FizzyAde::ComponentSystem::ComponentLoader::resolve(FizzyAde::ComponentSystem::Component *component, QList<FizzyAde::ComponentSystem::Component *> &resolvedList, QList<FizzyAde::ComponentSystem::Component *> &processedList)
{
    processedList.append(component);

    for(auto dependency : component->m_dependencies)
    {
        if (!resolvedList.contains(dependency)) {
            if (processedList.contains(dependency)) {
                continue;
            }

            resolve(dependency, resolvedList, processedList);
        }
    }

    resolvedList.append(component);
}
