/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/02/2021.
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

#include "PublicIPHostMaskerSettingsPageWidget.h"

#include "ComponentSystem/IComponentManager.h"
#include "PublicIPHostMasker.h"

#include <cassert>

#include "ui_PublicIPHostMaskerSettingsPageWidget.h"

Nedrysoft::PublicIPHostMasker::PublicIPHostMaskerSettingsPageWidget::PublicIPHostMaskerSettingsPageWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PublicIPHostMaskerSettingsPageWidget) {

    ui->setupUi(this);

    auto hostMasker = Nedrysoft::ComponentSystem::getObject<PublicIPHostMasker>();

    assert(hostMasker!=nullptr);

    connect(ui->enabledCheckBox, &QCheckBox::toggled, [=](bool checked) {
        hostMasker->m_enabled = checked;

        updateSettings();
    });

    ui->detectedPublicIPLineEdit->setText(hostMasker->getPublicIP());

    m_loadingConfiguration = true;

    ui->enabledCheckBox->setChecked(hostMasker->m_enabled);

    m_loadingConfiguration = false;

}

Nedrysoft::PublicIPHostMasker::PublicIPHostMaskerSettingsPageWidget::~PublicIPHostMaskerSettingsPageWidget() {
    delete ui;
}

auto Nedrysoft::PublicIPHostMasker::PublicIPHostMaskerSettingsPageWidget::updateSettings() -> void {
#if !defined(Q_OS_MACOS)
    return;
#endif
    if (!m_loadingConfiguration) {
        acceptSettings();
    }
}

auto Nedrysoft::PublicIPHostMasker::PublicIPHostMaskerSettingsPageWidget::acceptSettings() -> void {
    auto hostMasker = Nedrysoft::ComponentSystem::getObject<PublicIPHostMasker>();

    assert(hostMasker!=nullptr);

    hostMasker->saveToFile();
}

auto Nedrysoft::PublicIPHostMasker::PublicIPHostMaskerSettingsPageWidget::canAcceptSettings() -> bool {
    return true;
}