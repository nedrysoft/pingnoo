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

#include "NewTargetDialog.h"
#include "ui_NewTargetDialog.h"
#include "ComponentSystem/IComponentManager.h"
#include "Core/IPingEngineFactory.h"

Nedrysoft::RouteAnalyser::NewTargetDialog::NewTargetDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Nedrysoft::RouteAnalyser::Ui::NewTargetDialog) {
    ui->setupUi(this);

    auto pingEngines = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IPingEngineFactory>();

    for (auto pingEngine : pingEngines) {
        ui->engineComboBox->addItem(pingEngine->description(),
                                    QVariant::fromValue<Nedrysoft::Core::IPingEngineFactory *>(pingEngine));
    }
}

Nedrysoft::RouteAnalyser::NewTargetDialog::~NewTargetDialog() {
    delete ui;
}

Nedrysoft::Core::IPingEngineFactory *Nedrysoft::RouteAnalyser::NewTargetDialog::pingEngineFactory() {
    return ui->engineComboBox->currentData().value<Nedrysoft::Core::IPingEngineFactory *>();
}

QString Nedrysoft::RouteAnalyser::NewTargetDialog::pingTarget() {
    return ui->hostLineEdit->text();
}

Nedrysoft::Core::IPVersion Nedrysoft::RouteAnalyser::NewTargetDialog::ipVersion() {
    if (ui->ipv4RadioButton->isChecked()) {
        return Nedrysoft::Core::IPVersion::V4;
    } else {
        return Nedrysoft::Core::IPVersion::V6;
    }
}

double Nedrysoft::RouteAnalyser::NewTargetDialog::interval() {
    return ui->intervalDoubleSpinBox->value();
}

