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

#include "ComponentDetailsDialog.h"
#include "ui_ComponentDetailsDialog.h"

ComponentDetailsDialog::ComponentDetailsDialog(FizzyAde::ComponentSystem::Component *component, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentDetailsDialog)
{
    ui->setupUi(this);

    ui->nameLineEdit->setText(component->name());
    ui->versionLineEdit->setText(component->versionString());
    ui->categoryLineEdit->setText(component->category());
    ui->vendorLineEdit->setText(component->vendor());
    ui->copyrightLineEdit->setText(component->copyright());
    ui->URLLineEdit->setText(component->url());
    ui->descriptionTextEdit->setText(component->description());
    ui->licenseTextEdit->setText(component->license());
    ui->dependenciesTextEdit->setText(component->dependencies());
    ui->locationLineEdit->setText(component->filename());
}

ComponentDetailsDialog::~ComponentDetailsDialog()
{
    delete ui;
}
