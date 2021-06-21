/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 17/06/2021.
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

#include "HostMaskingRibbonGroup.h"

#include "IHostMaskerManager.h"

#include "ui_HostMaskingRibbonGroup.h"

Nedrysoft::Core::HostMaskingRibbonGroup::HostMaskingRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::HostMaskingRibbonGroup) {

    ui->setupUi(this);

    connect(ui->maskClipboardCheckBox, &QCheckBox::clicked, [=](bool checked) {
        auto hostMaskerManager = IHostMaskerManager::getInstance();

        if (!hostMaskerManager) {
            return;
        }

        hostMaskerManager->setEnabled(Nedrysoft::Core::HostMaskType::Clipboard, checked);
    });

    connect(ui->maskOutputCheckBox, &QCheckBox::clicked, [=](bool checked) {
        auto hostMaskerManager = IHostMaskerManager::getInstance();

        if (!hostMaskerManager) {
            return;
        }

        hostMaskerManager->setEnabled(Nedrysoft::Core::HostMaskType::Output, checked);
    });

    connect(ui->maskScreenCheckBox, &QCheckBox::clicked, [=](bool checked) {
        auto hostMaskerManager = IHostMaskerManager::getInstance();

        if (!hostMaskerManager) {
            return;
        }

        hostMaskerManager->setEnabled(Nedrysoft::Core::HostMaskType::Screen, checked);
    });

    auto hostMaskManager = Nedrysoft::Core::IHostMaskerManager::getInstance();

    if (hostMaskManager) {
        ui->maskScreenCheckBox->setChecked(hostMaskManager->enabled(Nedrysoft::Core::HostMaskType::Screen));
        ui->maskOutputCheckBox->setChecked(hostMaskManager->enabled(Nedrysoft::Core::HostMaskType::Output));
        ui->maskClipboardCheckBox->setChecked(hostMaskManager->enabled(Nedrysoft::Core::HostMaskType::Clipboard));
    }
}

Nedrysoft::Core::HostMaskingRibbonGroup::~HostMaskingRibbonGroup() {
    delete ui;
}
