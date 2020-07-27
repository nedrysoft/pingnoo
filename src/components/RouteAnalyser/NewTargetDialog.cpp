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

FizzyAde::RouteAnalyser::NewTargetDialog::NewTargetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new FizzyAde::RouteAnalyser::Ui::NewTargetDialog)
{
    ui->setupUi(this);

    auto pingEngines = FizzyAde::ComponentSystem::getObjects<FizzyAde::Core::IPingEngineFactory>();

    for (auto pingEngine : pingEngines) {
        ui->engineComboBox->addItem(pingEngine->description(), QVariant::fromValue<FizzyAde::Core::IPingEngineFactory *>(pingEngine));
    }
}

FizzyAde::RouteAnalyser::NewTargetDialog::~NewTargetDialog()
{
    delete ui;
}

FizzyAde::Core::IPingEngineFactory *FizzyAde::RouteAnalyser::NewTargetDialog::pingEngineFactory()
{
    return ui->engineComboBox->currentData().value<FizzyAde::Core::IPingEngineFactory *>();
}

QString FizzyAde::RouteAnalyser::NewTargetDialog::pingTarget()
{
    return(ui->hostLineEdit->text());
}

FizzyAde::Core::IPVersion FizzyAde::RouteAnalyser::NewTargetDialog::ipVersion()
{
    if (ui->ipv4RadioButton->isChecked()) {
        return(FizzyAde::Core::IPVersion::V4);
    } else {
        return(FizzyAde::Core::IPVersion::V6);
    }
}

double FizzyAde::RouteAnalyser::NewTargetDialog::interval()
{
    return(ui->intervalDoubleSpinBox->value());
}

