/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
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

#include "ViewportRibbonGroup.h"

#include "ColourManager.h"
#include "ComponentSystem/IComponentManager.h"
#include "RouteAnalyserEditor.h"
#include "ui_ViewportRibbonGroup.h"

Nedrysoft::RouteAnalyser::ViewportRibbonGroup::ViewportRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ViewportRibbonGroup) {

    ui->setupUi(this);

    ui->durationComboBox->addItems(
            QStringList() <<
                "60 Seconds" <<
                "10 Minutes" <<
                "15 Minutes" <<
                "30 Minutes" <<
                "45 Minutes" <<
                "60 Minutes" <<
                "12 Hours" <<
                "24 Hours" );

    connect(ui->trimmerWidget, &TrimmerWidget::positionChanged, [=](double start, double end) {
        // TODO: add logic.
    });

    connect(ui->durationComboBox, &QComboBox::currentTextChanged, [=](QString text) {
        // TODO: add logic.
    });

    connect(ui->endLockCheckBox, &QCheckBox::stateChanged, [=](int state) {
        // TODO: add logic.

        if (state==Qt::CheckState::Checked) {

        } else if (state==Qt::CheckState::Unchecked) {

        }
    });

    ui->trimmerWidget->setPosition(0.25, 0.5);
}

Nedrysoft::RouteAnalyser::ViewportRibbonGroup::~ViewportRibbonGroup() {
    delete ui;
}
