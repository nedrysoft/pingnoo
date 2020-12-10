/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo
 *
 * Created by Adrian Carpenter on 11/12/2020.
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

#include "NewTargetRibbonGroup.h"
#include "ui_NewTargetRibbonGroup.h"

#include "Ribbon/RibbonDropButton.h"

#include <QMenu>

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::NewTargetRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::NewTargetRibbonGroup) {

    ui->setupUi(this);

    connect(ui->favouriteDropButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, [=](bool clicked) {
        QMenu menu;
        QPoint menuPosition = ui->favouriteDropButton->rect().bottomLeft();

        menu.addAction(tr("Recent Targets"));
        menu.addSeparator();
        menu.addAction(tr("Favourites"));
        menu.addSeparator();
        menu.addAction(tr("Load Favourite..."));
        menu.addAction(tr("New Favourite..."));
        menu.addAction(tr("Edit Favrouite..."));
        menu.addAction(tr("Save Favourite..."));

        menuPosition = mapToGlobal(menuPosition);

        menu.exec(menuPosition);
    });
}

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::~NewTargetRibbonGroup() {
    delete ui;
}
