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
#include "ui_mainwindow.h"
#include "ComponentSystem/IComponentManager.h"
#include "ComponentSystem/ComponentLoader.h"
#include "ComponentSystem/Component.h"
#include "EditorManager.h"
#include "Core/ICore.h"
#include "Core/IEditor.h"
#include "Core/ICommandManager.h"
#include "Core/IContextManager.h"
#include "Core/IMenu.h"
#include "Pingnoo.h"
#include <QDebug>
#include <QApplication>
#include "ComponentViewerDialog.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include "AboutDialog.h"

FizzyAde::Core::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new FizzyAde::Core::Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->setWindowIcon(QIcon(":/Pingnoo/appicon.ico"));

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

FizzyAde::Core::MainWindow::~MainWindow()
{
   /* delete m_pointInfoLabel;
    delete m_hopInfoLabel;
    delete m_hostInfoLabel;
    delete m_tableModel;*/

    delete ui;
}

void FizzyAde::Core::MainWindow::onCutButtonClicked()
{
/*    auto customPlot = m_plotList.at(0);
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

    customPlot->toPainter(&painter, static_cast<int>(plotWidth), static_cast<int>(plotHeight));*/
}

void FizzyAde::Core::MainWindow::initialise()
{
    createDefaultCommands();
    registerDefaultCommands();

    FizzyAde::ComponentSystem::addObject(new FizzyAde::Core::EditorManager(ui->editorTabWidget));

    /*auto editors = FizzyAde::ComponentSystem::getObjects<FizzyAde::Core::IEditor>();

    for (auto editor : editors) {
        ui->editorTabWidget->addTab(editor->widget(), "Route Analyser");
    }*/
}

void FizzyAde::Core::MainWindow::createDefaultCommands()
{
    createCommand(Pingnoo::Constants::editCut, ui->cutButton);
    createCommand(Pingnoo::Constants::editCopy, ui->copyButton);
    createCommand(Pingnoo::Constants::editPaste, ui->pasteButton);
    createCommand(Pingnoo::Constants::fileOpen, nullptr);
    createCommand(Pingnoo::Constants::helpAbout, nullptr, QAction::ApplicationSpecificRole);
    createCommand(Pingnoo::Constants::helpAboutComponents, nullptr, QAction::ApplicationSpecificRole);

    createMenu(Pingnoo::Constants::applicationMenuBar);

    auto fileMenu = createMenu(Pingnoo::Constants::menuFile, Pingnoo::Constants::applicationMenuBar);

    fileMenu->addGroupBefore(Pingnoo::Constants::defaultGroupTop, Pingnoo::Constants::fileNewGroup);
    fileMenu->addGroupAfter(Pingnoo::Constants::fileOpenGroup, Pingnoo::Constants::fileNewGroup);
    fileMenu->addGroupAfter(Pingnoo::Constants::fileNewGroup, Pingnoo::Constants::fileSaveGroup);

    createMenu(Pingnoo::Constants::menuEdit, Pingnoo::Constants::applicationMenuBar);
    createMenu(Pingnoo::Constants::menuHelp, Pingnoo::Constants::applicationMenuBar);

    addMenuCommand(Pingnoo::Constants::menuFile, Pingnoo::Constants::fileOpen);

    addMenuCommand(Pingnoo::Constants::menuHelp, Pingnoo::Constants::helpAbout);
    addMenuCommand(Pingnoo::Constants::menuHelp, Pingnoo::Constants::helpAboutComponents);

    addMenuCommand(Pingnoo::Constants::menuEdit, Pingnoo::Constants::editCut);
    addMenuCommand(Pingnoo::Constants::menuEdit, Pingnoo::Constants::editCopy);
    addMenuCommand(Pingnoo::Constants::menuEdit, Pingnoo::Constants::editPaste);

    if (FizzyAde::Core::IContextManager::getInstance()) {
        FizzyAde::Core::IContextManager::getInstance()->setContext(FizzyAde::Core::GlobalContext);
    }
}

void FizzyAde::Core::MainWindow::registerDefaultCommands()
{
    auto commandManager = FizzyAde::Core::ICommandManager::getInstance();

    m_aboutComponentsAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::helpAboutComponents));

    m_aboutComponentsAction->setEnabled(true);
    m_aboutComponentsAction->setMenuRole(QAction::ApplicationSpecificRole);

    commandManager->registerAction(m_aboutComponentsAction, Pingnoo::Constants::helpAboutComponents);

    connect(m_aboutComponentsAction, &QAction::triggered, [](bool) {
        ComponentViewerDialog componentViewerDialog(FizzyAde::ComponentSystem::getObject<QMainWindow>());

        if (componentViewerDialog.exec()) {
            QString appSettingsFilename = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/"+qApp->organizationName()+"/"+qApp->applicationName()+"/appSettings.json";
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

    m_aboutAction = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::helpAbout));

    m_aboutAction->setEnabled(true);
    m_aboutAction->setMenuRole(QAction::ApplicationSpecificRole);

    commandManager->registerAction(m_aboutAction, Pingnoo::Constants::helpAbout);

    connect(m_aboutAction, &QAction::triggered, [](bool) {
        AboutDialog aboutDialog;

        aboutDialog.exec();
    });

}

FizzyAde::Core::ICommand *FizzyAde::Core::MainWindow::createCommand(QString commandId, QAbstractButton *button, QAction::MenuRole menuRole)
{
    auto commandManager = FizzyAde::Core::ICommandManager::getInstance();

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

FizzyAde::Core::IMenu *FizzyAde::Core::MainWindow::createMenu(QString menuId, QString parentMenuId)
{
    auto commandManager = FizzyAde::Core::ICommandManager::getInstance();

    if (!commandManager) {
        return nullptr;
    }

    FizzyAde::Core::IMenu *parentMenu = nullptr;

    if (!parentMenuId.isNull()) {
        parentMenu = commandManager->findMenu(parentMenuId);
    }

    return commandManager->createMenu(menuId, parentMenu);
}

FizzyAde::Core::IMenu *FizzyAde::Core::MainWindow::findMenu(QString menuId)
{
    auto commandManager = FizzyAde::Core::ICommandManager::getInstance();

    if (!commandManager) {
        return nullptr;
    }

    return commandManager->findMenu(menuId);
}

void FizzyAde::Core::MainWindow::addMenuCommand(QString menuId, QString commandId)
{
    auto commandManager = FizzyAde::Core::ICommandManager::getInstance();

    if (!commandManager) {
        return;
    }

    auto menu = commandManager->findMenu(menuId);

    auto command = commandManager->findCommand(commandId);

    menu->addCommand(command, Pingnoo::Constants::defaultGroupTop);
}

void FizzyAde::Core::MainWindow::on_toolButton_clicked()
{

}
