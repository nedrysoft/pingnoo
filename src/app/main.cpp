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

#include "ComponentSystem/Component.h"
#include "ComponentSystem/ComponentLoader.h"
#include "ComponentSystem/IComponentManager.h"
#include "Core/ICore.h"
#include "Core/ILogger.h"
#include "SplashScreen.h"

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QGuiApplication>
#include <QIcon>
#include <QJsonDocument>
#include <QMainWindow>
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QTimer>
#include <spdlog/spdlog.h>

#if defined(Q_OS_MAC)
#include <CoreFoundation/CoreFoundation.h>
#endif

auto constexpr splashscreenTimeout = 3000;

int main(int argc, char **argv) {
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    qApp->setApplicationName("Pingnoo");
    qApp->setOrganizationName("Nedrysoft");

    auto componentLoader = new Nedrysoft::ComponentSystem::ComponentLoader;
    auto applicationInstance = new QApplication(argc, argv);

    Nedrysoft::SplashScreen *splashScreen = nullptr;

    splashScreen = Nedrysoft::SplashScreen::getInstance();

    splashScreen->show();

    auto componentManager = Nedrysoft::ComponentSystem::IComponentManager::getInstance();

    componentManager->addObject(componentLoader);

    SPDLOG_DEBUG("Application started.");

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

            if (QDir(folderName).exists()) {
                qApp->addLibraryPath(folderName);
            }
        }
    }

    for (auto searchPath : dataPaths) {
        auto folderName = searchPath + "/" + qApp->organizationName() + "/" + qApp->applicationName() + "/PlugIns";

        if (QDir(folderName).exists()) {
            componentLoader->addComponents(folderName);
        }
    }
#else
    QStringList componentLocations = QStringList() << "APPDIR" << "PINGNOO_COMPONENT_DIR";

    for(auto dirName : componentLocations) {
        if (QProcessEnvironment::systemEnvironment().contains(dirName)) {
            SPDLOG_INFO(QString("%1 = %1")
                                .arg(dirName)
                                .arg(QProcessEnvironment::systemEnvironment().value(dirName)).toStdString());

            componentLoader->addComponents(QProcessEnvironment::systemEnvironment().value(dirName) + "/Components");
        }
    }

    auto applicationDir = QDir(qApp->applicationDirPath());

    if (applicationDir.exists("Components")) {
        auto componentsPath = applicationDir.absoluteFilePath("Components");

        componentLoader->addComponents(componentsPath);
    }
#endif
    QString appSettingsFilename = QStandardPaths::standardLocations(
            QStandardPaths::GenericDataLocation).at(0) + "/" +
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
        if (!component->canBeDisabled()) {
            return true;
        }

        if (disabledComponents.contains(( component->name() + "." + component->vendor()).toLower())) {
            return false;
        }

        return true;
    });

    int exitCode;

    if (Nedrysoft::ComponentSystem::getObject<QMainWindow>()) {
        auto loggerInstances = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::ILogger>();

        for (auto logger : loggerInstances) {
            SPDLOG_INFO(QString("Registering logger %1").arg(logger->logger()->name().c_str()).toStdString());
        }

#if defined(Q_OS_WINDOWS)
        qApp->setWindowIcon(QIcon(":/app/AppIcon.ico"));
#else
        qApp->setWindowIcon(QIcon(":/app/images/appicon-512x512@2x.png"));
#endif

        QTimer::singleShot(splashscreenTimeout, [=]() {
            splashScreen->deleteLater();
        });

        exitCode = QApplication::exec();
    } else {
        qDebug() << QString(QObject::tr("The application does not have a main window, exiting. (please check Components are installed correctly)"));

        exitCode = 1;
    }

    componentLoader->unloadComponents();

    delete componentLoader;
    delete applicationInstance;

    return exitCode;
}
