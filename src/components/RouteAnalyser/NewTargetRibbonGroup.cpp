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
#include "Core/IPingEngineFactory.h"

#include <QAbstractItemView>
#include <QMenu>
#include <ComponentSystem/IComponentManager.h>

constexpr auto comboPadding = 12;

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::NewTargetRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::NewTargetRibbonGroup) {

    ui->setupUi(this);

    connect(ui->favouriteDropButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, [=](bool clicked) {
        QMenu menu;
        QPoint menuPosition = ui->favouriteDropButton->rect().bottomLeft();

        menu.addAction(tr("Recent Targets"))->setDisabled(true);
        menu.addSeparator();
        menu.addAction(tr("Favourites"))->setDisabled(true);
        menu.addSeparator();
        menu.addAction(tr("Load Favourite..."));
        menu.addAction(tr("New Favourite..."));
        menu.addAction(tr("Edit Favrouite..."));
        menu.addAction(tr("Save Favourite..."));

        menuPosition = mapToGlobal(menuPosition);

        menu.exec(menuPosition);
    });

    ui->intervalLineEdit->setPlaceholderText("2.5s");
    ui->targetLineEdit->setPlaceholderText("1.1.1.1");

    auto pingEngines = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IPingEngineFactory>();

    if (pingEngines.count()) {
        auto minimumWidth = 0;
        for(auto pingEngine : pingEngines) {
            ui->engineComboBox->addItem(pingEngine->description());

            QFontMetrics fontMetrics(ui->engineComboBox->font());

            minimumWidth = qMax(minimumWidth, fontMetrics.boundingRect(pingEngine->description()).width()+comboPadding);
        }

        ui->engineComboBox->view()->setMinimumWidth(minimumWidth);
        ui->engineComboBox->setPlaceholderText(pingEngines[0]->description());
    }
}

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::~NewTargetRibbonGroup() {
    delete ui;
}
