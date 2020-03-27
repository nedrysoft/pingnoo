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

#include "ComponentSystem/ComponentLoader.h"
#include "ComponentSystem/Component.h"
#include "ComponentSystem/IComponentManager.h"
#include "AppNap/AppNap.h"
#include <QApplication>
#include <QDebug>
#include <QDir>

#if defined(Q_OS_MAC)
#include <CoreFoundation/CoreFoundation.h>
#endif

int main(int argc, char **argv)
{
    auto componentLoader = new FizzyAde::ComponentSystem::ComponentLoader;
    auto applicationInstance = new QApplication(argc, argv);
    auto appNap = FizzyAde::AppNap::AppNap::getInstance();

    appNap->prevent("App Nap has been disabled as it interferes with thread timing.");

#ifdef Q_OS_MAC
    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding());
    QString componentPath = QString(pathPtr)+"/Contents/PlugIns";

    CFRelease(appUrlRef);
    CFRelease(macPath);

    if (componentPath.isNull()) {
        QDir dir = applicationInstance->applicationDirPath();

        dir.cdUp();

        componentPath = dir.absolutePath()+"/PlugIns";
    }

    componentLoader->addComponents(componentPath);
#else
    componentLoader->addComponents("Components");
#endif

    componentLoader->loadComponents();

    for(FizzyAde::ComponentSystem::Component *component : componentLoader->components()) {
        qDebug() << component->name() << component->version() << component->isLoaded() << component->loadError() << component->missingDependencies();
    }

    auto exitCode = QApplication::exec();

    delete componentLoader;
    delete applicationInstance;

    return(exitCode);
}
