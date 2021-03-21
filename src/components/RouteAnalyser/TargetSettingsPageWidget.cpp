/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 20/12/2020.
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

#include "TargetSettingsPageWidget.h"

#include "ComponentSystem/IComponentManager.h"
#include "Core/IPingEngineFactory.h"
#include "TargetSettings.h"
#include "Utils.h"

#include <cassert>
#include "ui_TargetSettingsPageWidget.h"

Nedrysoft::RouteAnalyser::TargetSettingsPageWidget::TargetSettingsPageWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::TargetSettingsPageWidget) {

    auto targetSettings = Nedrysoft::ComponentSystem::getObject<TargetSettings>();

    ui->setupUi(this);

    auto engineFactories = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IPingEngineFactory>();

    QMultiMap<double, Nedrysoft::Core::IPingEngineFactory *> sortedPingEngines;

    for (auto factory : engineFactories) {
        ui->defaultEngineComboBox->addItem(factory->description(), factory->metaObject()->className());

        sortedPingEngines.insert(1-factory->priority(), factory);
    }

    if (targetSettings) {
        ui->defaultTargetLineEdit->setText(targetSettings->defaultHost());
        ui->defaultIntervalLineEdit->setText(Nedrysoft::Utils::intervalToString(targetSettings->defaultPingInterval()));

        if (targetSettings->defaultIPVersion()==Nedrysoft::Core::IPVersion::V6) {
            ui->ipV6RadioButton->setChecked(true);
        } else {
            ui->ipV4RadioButton->setChecked(true);
        }

        auto selectionIndex = ui->defaultEngineComboBox->findData(targetSettings->defaultPingEngine());

        if (selectionIndex==-1) {
            if (sortedPingEngines.count()) {
                selectionIndex = ui->defaultEngineComboBox->findData(
                        sortedPingEngines.first()->metaObject()->className(),
                        Qt::UserRole + 1 );
            }
        }

        ui->defaultEngineComboBox->setCurrentIndex(selectionIndex);
    }
}

Nedrysoft::RouteAnalyser::TargetSettingsPageWidget::~TargetSettingsPageWidget() {
    delete ui;
}

auto Nedrysoft::RouteAnalyser::TargetSettingsPageWidget::canAcceptSettings() -> bool {
    return true;
};

auto Nedrysoft::RouteAnalyser::TargetSettingsPageWidget::acceptSettings() -> void {
    auto targetSettings = Nedrysoft::ComponentSystem::getObject<TargetSettings>();

    assert(targetSettings!=nullptr);

    double pingInterval = targetSettings->defaultPingInterval();

    Nedrysoft::Utils::parseIntervalString (ui->defaultIntervalLineEdit->text(), pingInterval);

    targetSettings->setDefaultHost(ui->defaultTargetLineEdit->text());
    targetSettings->setDefaultPingInterval(pingInterval);
    targetSettings->setDefaultPingEngine(ui->defaultEngineComboBox->currentData().toString());
    targetSettings->setDefaultIPVersion(
            ui->ipV4RadioButton->isChecked() ? Nedrysoft::Core::IPVersion::V4 : Nedrysoft::Core::IPVersion::V6);

    targetSettings->saveToFile();
}
