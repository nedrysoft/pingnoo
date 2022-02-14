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

#include "CoreConstants.h"
#include "ICommandManager.h"
#include "RibbonBarManager.h"
#include "RibbonAction"

#include "ui_ClipboardRibbonGroup.h"

#include <QMenu>

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
        auto clipboardCopyAction = new Nedrysoft::Ribbon::RibbonAction;

        auto proxyAction = ribbonBarManager->registerAction(
            clipboardCopyAction,
            Nedrysoft::Core::Constants::RibbonCommands::ClipboardCopy
        );

        connect(
            clipboardCopyAction,
            &Nedrysoft::Ribbon::RibbonAction::ribbonEvent,
            [=](Nedrysoft::Ribbon::Event *event) {
                // do nothing for now in the global context.
            }
        );

        ui->copyButton->setAction(proxyAction);
    }
}

Nedrysoft::Core::ClipboardRibbonGroup::~ClipboardRibbonGroup() {
    delete ui;
}
