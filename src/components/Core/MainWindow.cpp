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

#include "MainWindow.h"

#include "AboutDialog.h"
#include "ComponentSystem/Component.h"
#include "ComponentSystem/ComponentViewerDialog.h"
#include "Core/ICommandManager.h"
#include "Core/IEditor.h"
#include "EditorManager.h"
#include "Pingnoo.h"
#include "RibbonBarManager.h"
#include "SettingsDialog/ISettingsPage.h"
#include "SettingsDialog/SettingsDialog.h"
#include "ui_MainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <spdlog/spdlog.h>

Nedrysoft::Core::MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Nedrysoft::Core::Ui::MainWindow),
        m_ribbonBarManager(nullptr),
        m_settingsDialog(nullptr) {

    // TODO: m_ribbonBarManager should be created in the component initialisation

    spdlog::set_level(spdlog::level::trace);

    ui->setupUi(this);

#if defined(Q_OS_MACOS)
    qApp->setWindowIcon(QIcon(":/app/images/appicon-512x512@2x.png"));
#else
    qApp->setWindowIcon(QIcon(":/app/AppIcon.ico"));
#endif

    m_ribbonBarManager = new Nedrysoft::Core::RibbonBarManager(ui->ribbonBar);

    Nedrysoft::ComponentSystem::addObject(m_ribbonBarManager);

    m_ribbonBarManager->addPage(tr("Home"), Pingnoo::Constants::ribbonHomePage);

    ui->editorTabWidget->setText(tr("Select New Target from the Menu or Ribbon bar to begin."));

    showMaximized();

    this->setWindowTitle(QString(tr("Pingnoo %1.%2.%3-%4"))
            .arg(PINGNOO_GIT_YEAR)
            .arg(PINGNOO_GIT_MONTH)
            .arg(PINGNOO_GIT_DAY)
            .arg(PINGNOO_GIT_BRANCH) );

    // QStatusBar *statusBar = new QStatusBar;

    /*
    m_pointInfoLabel = new QLabel();
    m_hopInfoLabel = new QLabel();
    m_hostInfoLabel = new QLabel();
    m_timeInfoLabel = new QLabel();

    m_pointInfoLabel->setIndent(StatusBarPointIndent);

    m_timeInfoLabel->setMinimumWidth(ui->tableWidget->fontMetrics().horizontalAdvance(QDateTime::currentDateTime().toString()+"XX.XX"));//StatusBarTimeWidth);
    m_pointInfoLabel->setMinimumWidth(ui->tableWidget->fontMetrics().horizontalAdvance(tr("XX XXXX.XXXX")));//StatusBarPointWidth);
    m_hopInfoLabel->setMinimumWidth(ui->tableWidget->fontMetrics().horizontalAdvance(tr("Hop XXXX")));
    m_hostInfoLabel->setMinimumWidth( ui->tableWidget->fontMetrics().horizontalAdvance(tr("XXXXXXXXXXXXXXX.XXXXXXXXXXXXXXX.XXXXXXXXXXXXXXX.XXX")));

    m_pointInfoLabel->setTextFormat(Qt::RichText);
    m_hopInfoLabel->setTextFormat(Qt::RichText);
    m_hostInfoLabel->setTextFormat(Qt::RichText);
    m_timeInfoLabel->setTextFormat(Qt::RichText);

    ui->statusbar->addWidget(m_pointInfoLabel);
    ui->statusbar->addWidget(m_timeInfoLabel);
    ui->statusbar->addWidget(m_hopInfoLabel);
    ui->statusbar->addWidget(m_hostInfoLabel);
    */
}

Nedrysoft::Core::MainWindow::~MainWindow() {
    /* delete m_pointInfoLabel;
     delete m_hopInfoLabel;
     delete m_hostInfoLabel;
     delete m_tableModel;*/

    delete ui;

    if (m_ribbonBarManager) {
        delete m_ribbonBarManager;
    }

    if (m_editorManager) {
        delete m_editorManager;
    }

    if (m_settingsDialog) {
        delete m_settingsDialog;
    }
}

auto Nedrysoft::Core::MainWindow::initialise() -> void {
    createDefaultCommands();
    registerDefaultCommands();

    m_editorManager = new Nedrysoft::Core::EditorManager(ui->editorTabWidget);

    Nedrysoft::ComponentSystem::addObject(m_editorManager);
}

auto Nedrysoft::Core::MainWindow::createDefaultCommands() -> void {
    // create the commands, these are essentially placeholders.  Commands can be added to menus, buttons,
    // shortcut keys etc.

    createCommand(Pingnoo::Constants::fileOpen, nullptr);
    createCommand(Pingnoo::Constants::helpAbout, nullptr, QAction::ApplicationSpecificRole);
    createCommand(Pingnoo::Constants::helpAboutComponents, nullptr, QAction::ApplicationSpecificRole);
    createCommand(Pingnoo::Constants::filePreferences, nullptr, QAction::PreferencesRole);
    createCommand(Pingnoo::Constants::fileExit, nullptr, QAction::QuitRole);

    // create the menus, we create a main menu bar, then sub menus on that (File, Edit, Help etc).  In each
    // menu we then create groups, this allows us to reserve sections of the menu for specific items, components
    // can use these groups to add their commands at specific locations in a menu.

    createMenu(Pingnoo::Constants::applicationMenuBar);

    auto fileMenu = createMenu(Pingnoo::Constants::menuFile, Pingnoo::Constants::applicationMenuBar);

    fileMenu->addGroupBefore(Pingnoo::Constants::defaultGroupTop, Pingnoo::Constants::fileNewGroup);
    fileMenu->addGroupAfter(Pingnoo::Constants::fileNewGroup, Pingnoo::Constants::fileOpenGroup);
    fileMenu->addGroupAfter(Pingnoo::Constants::fileOpenGroup, Pingnoo::Constants::fileSaveGroup);
    fileMenu->addGroupAfter(Pingnoo::Constants::defaultGroupBottom, Pingnoo::Constants::defaultGroupBottom);
    fileMenu->addGroupAfter(Pingnoo::Constants::defaultGroupBottom, Pingnoo::Constants::fileExitGroup);

    createMenu(Pingnoo::Constants::menuEdit, Pingnoo::Constants::applicationMenuBar);
    createMenu(Pingnoo::Constants::menuHelp, Pingnoo::Constants::applicationMenuBar);

    addMenuCommand(Pingnoo::Constants::fileOpen, Pingnoo::Constants::menuFile);
    addMenuCommand(Pingnoo::Constants::filePreferences, Pingnoo::Constants::menuFile);
    addMenuCommand(Pingnoo::Constants::fileExit, Pingnoo::Constants::menuFile);

    addMenuCommand(Pingnoo::Constants::helpAbout, Pingnoo::Constants::menuHelp);
    addMenuCommand(Pingnoo::Constants::helpAboutComponents, Pingnoo::Constants::menuHelp);

    addMenuCommand(Pingnoo::Constants::editCut, Pingnoo::Constants::menuEdit);
    addMenuCommand(Pingnoo::Constants::editCopy, Pingnoo::Constants::menuEdit);
    addMenuCommand(Pingnoo::Constants::editPaste, Pingnoo::Constants::menuEdit);

    if (Nedrysoft::Core::IContextManager::getInstance()) {
        Nedrysoft::Core::IContextManager::getInstance()->setContext(Nedrysoft::Core::GlobalContext);
    }
}

auto Nedrysoft::Core::MainWindow::registerDefaultCommands() -> void {
    auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

    auto aboutComponentsAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::helpAboutComponents));

    aboutComponentsAction->setEnabled(true);
    aboutComponentsAction->setMenuRole(QAction::ApplicationSpecificRole);

    commandManager->registerAction(aboutComponentsAction, Pingnoo::Constants::helpAboutComponents);

    connect(aboutComponentsAction, &QAction::triggered, [](bool) {
        Nedrysoft::ComponentSystem::ComponentViewerDialog componentViewerDialog(
                Nedrysoft::ComponentSystem::getObject<QMainWindow>() );

        if (componentViewerDialog.exec()) {
            QString appSettingsFilename =
                    QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0) + "/" +
                    qApp->organizationName() + "/" + qApp->applicationName() + "/appSettings.json";

            QFile settingsFile(appSettingsFilename);
            QVariantList disabledPlugins;

            settingsFile.open(QFile::ReadOnly);

            auto loadedSettings = QJsonDocument::fromJson(settingsFile.readAll());

            QJsonArray disabledComponents = QJsonArray::fromStringList(componentViewerDialog.disabledComponents());

            auto rootObject = loadedSettings.object();

            rootObject["disabledComponents"] = disabledComponents;

            settingsFile.close();

            settingsFile.open(QFile::WriteOnly | QFile::Truncate);

            QJsonDocument saveDocument(rootObject);

            settingsFile.write(saveDocument.toJson());

            settingsFile.close();
        }
    });

    auto settingsAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::filePreferences));

    settingsAction->setEnabled(true);
    settingsAction->setMenuRole(QAction::PreferencesRole);

    commandManager->registerAction(settingsAction, Pingnoo::Constants::filePreferences);

    connect(settingsAction, &QAction::triggered, [this](bool) {
        if (m_settingsDialog) {
            m_settingsDialog->raise();
            m_settingsDialog->activateWindow();

            return;
        }

        QList<Nedrysoft::SettingsDialog::ISettingsPage *> pages;

        pages = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::SettingsDialog::ISettingsPage>();

        m_settingsDialog = new Nedrysoft::SettingsDialog::SettingsDialog(pages, this);

        m_settingsDialog->setWindowTitle(tr("Pingnoo configuration"));

#if !defined(Q_OS_MACOS)
        m_settingsDialog->setWindowModality(Qt::ApplicationModal);
#endif
        m_settingsDialog->show();

        connect(m_settingsDialog, &Nedrysoft::SettingsDialog::SettingsDialog::closed, [=]() {
            m_settingsDialog->deleteLater();

            m_settingsDialog = nullptr;
        });
    });

    auto exitAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::fileExit));

    exitAction->setEnabled(true);
    exitAction->setMenuRole(QAction::QuitRole);

    commandManager->registerAction(exitAction, Pingnoo::Constants::fileExit);

    connect(exitAction, &QAction::triggered, [this](bool) {
        close();
    });

    auto aboutAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::helpAbout));

    aboutAction->setEnabled(true);
    aboutAction->setMenuRole(QAction::ApplicationSpecificRole);

    commandManager->registerAction(aboutAction, Pingnoo::Constants::helpAbout);

    connect(aboutAction, &QAction::triggered, [](bool) {
        AboutDialog aboutDialog;

        aboutDialog.exec();
    });
}

auto Nedrysoft::Core::MainWindow::createCommand(
        QString commandId,
        QAbstractButton *button,
        QAction::MenuRole menuRole ) -> Nedrysoft::Core::ICommand * {

    auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

    if (!commandManager) {
        return nullptr;
    }

    auto action = new QAction(Pingnoo::Constants::commandText(commandId));

    action->setMenuRole(menuRole);

    auto command = commandManager->registerAction(action, commandId);

    if (button) {
        command->attachToWidget(button);
    }

    action->setEnabled(false);

    return command;
}

auto Nedrysoft::Core::MainWindow::createMenu(QString menuId, QString parentMenuId) -> Nedrysoft::Core::IMenu * {
    auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

    if (!commandManager) {
        return nullptr;
    }

    Nedrysoft::Core::IMenu *parentMenu = nullptr;

    if (!parentMenuId.isNull()) {
        parentMenu = commandManager->findMenu(parentMenuId);
    }

    return commandManager->createMenu(menuId, parentMenu);
}

auto Nedrysoft::Core::MainWindow::findMenu(QString menuId) -> Nedrysoft::Core::IMenu * {
    auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

    if (!commandManager) {
        return nullptr;
    }

    return commandManager->findMenu(menuId);
}

auto Nedrysoft::Core::MainWindow::addMenuCommand(QString commandId, QString menuId, QString groupId) -> void {
    auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

    if (!commandManager) {
        return;
    }

    auto menu = commandManager->findMenu(menuId);

    if (!menu) {
        return;
    }

    auto command = commandManager->findCommand(commandId);

    if (groupId.isNull()) {
        groupId = Pingnoo::Constants::defaultGroupTop;
    }

    menu->appendCommand(command, groupId);
}

void Nedrysoft::Core::MainWindow::closeEvent(QCloseEvent *closeEvent) {
    if (m_settingsDialog) {
        if (!m_settingsDialog->close()) {
            closeEvent->ignore();

            return;
        }

        delete m_settingsDialog;

        m_settingsDialog = nullptr;
    }
}