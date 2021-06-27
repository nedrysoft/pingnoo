/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 18/06/2021.
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

#include "ClipboardRibbonGroup.h"

#include "Command.h"
#include "CoreConstants.h"
#include "ICommandManager.h"
#include "RibbonBarManager.h"
#include "RibbonDropButtonAction.h"

#include "ui_ClipboardRibbonGroup.h"

#include <QMenu>

#include <QDebug>

Nedrysoft::Core::ClipboardRibbonGroup::ClipboardRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ClipboardRibbonGroup) {

    ui->setupUi(this);

    ui->copyButton->setVertical(false);

    auto icon = QIcon(":/Core/icons/2x/baseline_photo_camera_white_24dp.png");

    ui->copyButton->setText("Copy");

    ui->copyButton->setIcon(icon);
    ui->copyButton->setIconSize(QSize(16, 16));

    auto ribbonBarManager = Nedrysoft::Core::RibbonBarManager::getInstance();

    if (ribbonBarManager) {
        ribbonBarManager->registerWidget(
            ui->copyButton,
            Nedrysoft::Core::Constants::RibbonCommands::ClipboardCopy
        );

        auto clipboardCopyAction = new RibbonDropButtonAction;

        clipboardCopyAction->setEnabled(true);
        clipboardCopyAction->setMenuRole(QAction::ApplicationSpecificRole);

        /*auto commandAction = */ribbonBarManager->registerAction(
            clipboardCopyAction,
            Nedrysoft::Core::Constants::RibbonCommands::ClipboardCopy
        );

        connect(ui->copyButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, [=](bool dropdown) {

            //auto command = qobject_cast<Nedrysoft::Core::RibbonDropButtonAction *>(commandAction->action());


            qDebug() << "Triggered!";

            Q_EMIT clipboardCopyAction->triggered(dropdown);
        });
    }

#if 0
    m_copyCommand = new QAction(
        Nedrysoft::Core::Constants::commandText(tr("Copy"))
    );

    m_copyCommand->setEnabled(true);
    m_copyCommand->setMenuRole(QAction::ApplicationSpecificRole);

    ICommandManager *commandManager = ICommandManager::getInstance();

    if (commandManager) {
        auto command = commandManager->registerAction(m_copyCommand, Nedrysoft::Core::Constants::Commands::Copy);

        connect(ui->copyButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, [=](bool dropdown) {
            qDebug() << "triggered" << dropdown << "context" << IContextManager::getInstance()->context();

            command->action()->trigger();
        });
        /*auto commandManager->findCommand(Nedrysoft::Core::Constants::Commands::Copy);

        auto copyCommand = commandManager->registerAction(m_copyCommand, Nedrysoft::Core::Constants::Commands::Copy);

        m_copyCommand->trigger();
        /*
        connect(ui->copyButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, [=](bool dropdown) {
            if (!dropdown) {
                return;
            }

            QMenu menu;
            QPoint menuPosition = ui->copyButton->rect().bottomLeft();

            menuPosition = mapToGlobal(menuPosition);

            auto copyTableAsText = menu.addAction(tr("Copy Table as Text"));
            auto copyTableAsPDG = menu.addAction(tr("Copy Table as PDF"));
            auto copyTableAsImage = menu.addAction(tr("Copy Table as Image"));
            auto copyTableAsCSV = menu.addAction(tr("Copy Table as CSV"));
            auto copyGraphsAsImage = menu.addAction(tr("Copy Graphs as Image"));
            auto copyGraphsAsPDF = menu.addAction(tr("Copy Graphs as PDF"));
            auto CopyTableAndGraphsAsImage = menu.addAction(tr("Copy Table and Graphs as Image"));
            auto CopyTableAndGraphsAsPDF = menu.addAction(tr("Copy Table and Graphs as PDF"));

            menu.addAction(copyTableAsText);
            menu.addAction(copyTableAsPDG);
            menu.addAction(copyTableAsImage);
            menu.addAction(copyTableAsCSV);
            menu.addAction(copyGraphsAsImage);
            menu.addAction(copyGraphsAsPDF);
            menu.addAction(CopyTableAndGraphsAsImage);
            menu.addAction(CopyTableAndGraphsAsPDF);

            menu.exec(menuPosition);
        });*/
    }
#endif
}

Nedrysoft::Core::ClipboardRibbonGroup::~ClipboardRibbonGroup() {
    delete ui;
}
