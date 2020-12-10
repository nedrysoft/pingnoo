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

#include "mainwindow.h"

#include "AboutDialog.h"
#include "ComponentSystem/Component.h"
#include "ComponentSystem/ComponentLoader.h"
#include "ComponentSystem/IComponentManager.h"
#include "ComponentSystem/ComponentViewerDialog.h"
#include "SettingsDialog/SettingsDialog.h"
#include "Core/ICommandManager.h"
#include "Core/IContextManager.h"
#include "Core/ICore.h"
#include "Core/IEditor.h"
#include "Core/IMenu.h"
#include "EditorManager.h"
#include "Pingnoo.h"
#include "RibbonBarManager.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

Nedrysoft::Core::MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Nedrysoft::Core::Ui::MainWindow) {

    ui->setupUi(this);

#if defined(Q_OS_MACOS)
    qApp->setWindowIcon(QIcon(":/app/images/appicon-512x512@2x.png"));
#else
    qApp->setWindowIcon(QIcon(":/app/appicon.ico"));
#endif

    auto ribbonBarManager = new Nedrysoft::Core::RibbonBarManager(ui->ribbonBar);

    Nedrysoft::ComponentSystem::addObject(ribbonBarManager);

    ribbonBarManager->addPage(tr("Home"), Pingnoo::Constants::ribbonHomePage);
    ribbonBarManager->addPage(tr("File"), Pingnoo::Constants::ribbonFilePage);

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
}

/*void Nedrysoft::Core::MainWindow::onCutButtonClicked() {
    auto customPlot = m_plotList.at(0);
    auto pdfWriter = QPdfWriter("/Users/adriancarpenter/test.pdf");

    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setResolution(StandardDPI);

    pdfWriter.newPage();

    QRectF pageRect = pdfWriter.pageLayout().paintRect();

    auto plotWidth = static_cast<double>(customPlot->viewport().width());
    auto plotHeight = static_cast<double>(customPlot->viewport().height());

    auto scale = pageRect.width() / plotWidth;
    auto scale2 = pageRect.height() / plotHeight;

    if (scale2 < scale)
        scale = scale2;

    QCPPainter painter(&pdfWriter);

    painter.setMode(QCPPainter::pmVectorized);
    painter.setMode(QCPPainter::pmNoCaching);
    painter.setMode(QCPPainter::pmNonCosmetic);

    painter.scale(scale, scale);

    customPlot->toPainter(&painter, static_cast<int>(plotWidth), static_cast<int>(plotHeight));
}*/

void Nedrysoft::Core::MainWindow::initialise() {
    createDefaultCommands();
    registerDefaultCommands();

    Nedrysoft::ComponentSystem::addObject(new Nedrysoft::Core::EditorManager(ui->editorTabWidget));

    /*auto editors = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IEditor>();

    for (auto editor : editors) {
        ui->editorTabWidget->addTab(editor->widget(), "Route Analyser");
    }*/
}

void Nedrysoft::Core::MainWindow::createDefaultCommands() {
    //createCommand(Pingnoo::Constants::editCut, ui->cutButton);
    //createCommand(Pingnoo::Constants::editCopy, ui->copyButton);
    //createCommand(Pingnoo::Constants::editPaste, ui->pasteButton);

    // create the commands, these are essentially placeholders.  Commands can be added to menus, buttons,
    // shortcut keys etc.

    createCommand(Pingnoo::Constants::fileOpen, nullptr);
    createCommand(Pingnoo::Constants::helpAbout, nullptr, QAction::ApplicationSpecificRole);
    createCommand(Pingnoo::Constants::helpAboutComponents, nullptr, QAction::ApplicationSpecificRole);
    createCommand(Pingnoo::Constants::filePreferences, nullptr, QAction::PreferencesRole);

    // create the menus, we create a main menu bar, then sub menus on that (File, Edit, Help etc).  In each
    // menu we then create groups, this allows us to reserve sections of the menu for specific items, components
    // can use these groups to add their commands at specific locations in a menu.

    createMenu(Pingnoo::Constants::applicationMenuBar);

    auto fileMenu = createMenu(Pingnoo::Constants::menuFile, Pingnoo::Constants::applicationMenuBar);

    fileMenu->addGroupBefore(Pingnoo::Constants::defaultGroupTop, Pingnoo::Constants::fileNewGroup);
    fileMenu->addGroupAfter(Pingnoo::Constants::fileNewGroup, Pingnoo::Constants::fileOpenGroup);
    fileMenu->addGroupAfter(Pingnoo::Constants::fileOpenGroup, Pingnoo::Constants::fileSaveGroup);
    fileMenu->addGroupAfter(Pingnoo::Constants::defaultGroupBottom, Pingnoo::Constants::fileMiscGroup);

    createMenu(Pingnoo::Constants::menuEdit, Pingnoo::Constants::applicationMenuBar);
    createMenu(Pingnoo::Constants::menuHelp, Pingnoo::Constants::applicationMenuBar);

    addMenuCommand(Pingnoo::Constants::fileOpen, Pingnoo::Constants::menuFile);
    addMenuCommand(Pingnoo::Constants::filePreferences, Pingnoo::Constants::menuFile);

    addMenuCommand(Pingnoo::Constants::helpAbout, Pingnoo::Constants::menuHelp);
    addMenuCommand(Pingnoo::Constants::helpAboutComponents, Pingnoo::Constants::menuHelp);

    addMenuCommand(Pingnoo::Constants::editCut, Pingnoo::Constants::menuEdit);
    addMenuCommand(Pingnoo::Constants::editCopy, Pingnoo::Constants::menuEdit);
    addMenuCommand(Pingnoo::Constants::editPaste, Pingnoo::Constants::menuEdit);

    if (Nedrysoft::Core::IContextManager::getInstance()) {
        Nedrysoft::Core::IContextManager::getInstance()->setContext(Nedrysoft::Core::GlobalContext);
    }
}

void Nedrysoft::Core::MainWindow::registerDefaultCommands() {
    auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

    m_aboutComponentsAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::helpAboutComponents));

    m_aboutComponentsAction->setEnabled(true);
    m_aboutComponentsAction->setMenuRole(QAction::ApplicationSpecificRole);

    commandManager->registerAction(m_aboutComponentsAction, Pingnoo::Constants::helpAboutComponents);

    connect(m_aboutComponentsAction, &QAction::triggered, [](bool) {
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

    m_settingsAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::filePreferences));

    m_settingsAction->setEnabled(true);
    m_settingsAction->setMenuRole(QAction::PreferencesRole);

    commandManager->registerAction(m_settingsAction, Pingnoo::Constants::filePreferences);

    connect(m_settingsAction, &QAction::triggered, [this](bool) {
        QList<Nedrysoft::SettingsDialog::ISettingsPage *> pages;

        m_settingsDialog = new Nedrysoft::SettingsDialog::SettingsDialog(pages, this);

        m_settingsDialog->show();

        connect(m_settingsDialog, &Nedrysoft::SettingsDialog::SettingsDialog::closed, [=]() {
            m_settingsDialog->deleteLater();

            m_settingsDialog = nullptr;
        });
    });

    m_aboutAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::helpAbout));

    m_aboutAction->setEnabled(true);
    m_aboutAction->setMenuRole(QAction::ApplicationSpecificRole);

    commandManager->registerAction(m_aboutAction, Pingnoo::Constants::helpAbout);

    connect(m_aboutAction, &QAction::triggered, [](bool) {
        AboutDialog aboutDialog;

        aboutDialog.exec();
    });
}

Nedrysoft::Core::ICommand *Nedrysoft::Core::MainWindow::createCommand(
        QString commandId,
        QAbstractButton *button,
        QAction::MenuRole menuRole) {

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

Nedrysoft::Core::IMenu *Nedrysoft::Core::MainWindow::createMenu(QString menuId, QString parentMenuId) {
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

Nedrysoft::Core::IMenu *Nedrysoft::Core::MainWindow::findMenu(QString menuId) {
    auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

    if (!commandManager) {
        return nullptr;
    }

    return commandManager->findMenu(menuId);
}

void Nedrysoft::Core::MainWindow::addMenuCommand(QString commandId, QString menuId, QString groupId) {
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
