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
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QString>
#include <QSettings>
#include <QTimer>
#include <QJsonDocument>
#include "SplashScreen.h"

#if defined(Q_OS_MAC)

#include <CoreFoundation/CoreFoundation.h>

#endif

int main(int argc, char **argv) {
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    qApp->setApplicationName("Pingnoo");
    qApp->setOrganizationName("Nedrysoft");

    auto componentLoader = new Nedrysoft::ComponentSystem::ComponentLoader;
    auto applicationInstance = new QApplication(argc, argv);
    auto appNap = Nedrysoft::AppNap::AppNap::getInstance();

    Nedrysoft::SplashScreen *splashScreen = Nedrysoft::SplashScreen::getInstance();;

    splashScreen->show();

    appNap->prevent("App Nap has been disabled as it interferes with thread timing.");

    auto componentManager = Nedrysoft::ComponentSystem::IComponentManager::getInstance();

    componentManager->addObject(componentLoader);

#ifdef Q_OS_MAC
    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding());
    QString componentPath = QString(pathPtr) + "/Contents/PlugIns";

    CFRelease(appUrlRef);
    CFRelease(macPath);

    if (componentPath.isNull()) {
        QDir dir = applicationInstance->applicationDirPath();

        dir.cdUp();

        componentPath = dir.absolutePath() + "/PlugIns";
    }

    componentLoader->addComponents(componentPath);

    auto extraLibrarySearchPaths = QStringList() << "Frameworks" << "PlugIns";

    auto dataPaths = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);

    for (auto searchPath : dataPaths) {
        for (auto folderPath : extraLibrarySearchPaths) {
            auto folderName =
                    searchPath + "/" + qApp->organizationName() + "/" + qApp->applicationName() + "/" + folderPath;

            if (QDir(folderName).exists())
                qApp->addLibraryPath(folderName);
        }
    }

    for (auto searchPath : dataPaths) {
        auto folderName = searchPath + "/" + qApp->organizationName() + "/" + qApp->applicationName() + "/PlugIns";

        if (QDir(folderName).exists()) {
            componentLoader->addComponents(folderName);
        }
    }
#else
    if (QProcessEnvironment::systemEnvironment().contains("APPDIR")) {
        componentLoader->addComponents(QProcessEnvironment::systemEnvironment().value("APPDIR")+"/Components");
    } else {
        componentLoader->addComponents("Components");
    }

#endif

    QString appSettingsFilename = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0) + "/" +
                                  qApp->organizationName() + "/" + qApp->applicationName() + "/appSettings.json";
    QFile settingsFile(appSettingsFilename);
    QVariantList disabledComponents;

    settingsFile.open(QFile::ReadOnly);

    if (settingsFile.isOpen()) {
        auto settings = QJsonDocument::fromJson(settingsFile.readAll()).toVariant();

        if (settings.isValid()) {
            auto settingsMap = settings.toMap();

            if (settingsMap.contains("disabledComponents")) {
                disabledComponents = settingsMap["disabledComponents"].toList();
            }
        }
    }

    componentLoader->loadComponents([disabledComponents](Nedrysoft::ComponentSystem::Component *component) -> bool {
        if (component->canBeDisabled() == false) {
            return true;
        }

        if (disabledComponents.contains(( component->name() + "." + component->vendor()).toLower())) {
            return false;
        }
        return true;
    });

    QTimer::singleShot(3000, [=]() {
        splashScreen->hide();
    });

    auto exitCode = QApplication::exec();

    delete componentLoader;
    delete applicationInstance;

    return exitCode;
}
