/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo
 *
 * Created by Adrian Carpenter on 13/12/2020.
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

// You may need to build the project (run Qt uic code generator) to get "ui_RouteAnalyserRibbonGroup.h" resolved

#include "LatencyRibbonGroup.h"

#include "ui_LatencyRibbonGroup.h"
#include "ColourManager.h"

#include <QMenu>

Nedrysoft::RouteAnalyser::LatencyRibbonGroup::LatencyRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LatencyRibbonGroup) {

    ui->setupUi(this);

    ui->lowWidget->setColour(ColourManager::getMinColour());
    ui->mediumWidget->setColour(ColourManager::getMidColour());
    ui->highWidget->setColour(ColourManager::getMaxColour());

    ui->lowLineEdit->setPlaceholderText("100ms");
    ui->mediumLineEdit->setPlaceholderText("200ms");
    ui->highLineEdit->setPlaceholderText("300ms");
}

Nedrysoft::RouteAnalyser::LatencyRibbonGroup::~LatencyRibbonGroup() {
    delete ui;
}
