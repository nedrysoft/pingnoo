/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 14/06/2021.
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

#include "RouteDiscoveryWidget.h"
#include "ui_RouteDiscoveryWidget.h"

Nedrysoft::RouteAnalyser::RouteDiscoveryWidget::RouteDiscoveryWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::RouteDiscoveryWidget) {

    ui->setupUi(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
}

Nedrysoft::RouteAnalyser::RouteDiscoveryWidget::~RouteDiscoveryWidget() {
    delete ui;
}

auto Nedrysoft::RouteAnalyser::RouteDiscoveryWidget::setTarget(QString target) -> void {
    ui->targetLabel->setText(QString("<center>Discovering route to %1...</center>").arg(target));
}

auto Nedrysoft::RouteAnalyser::RouteDiscoveryWidget::setProgress(int currentHop, int totalHops, int maximumTTL) -> void {
    if (totalHops==-1) {
        ui->hopsLabel->setText(QString("<center>(Maximum Hops %1)</center>").arg(maximumTTL));

        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(0);
        ui->progressBar->setValue(0);

        return;
    }

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(totalHops);
    ui->progressBar->setValue(currentHop);

    ui->hopsLabel->setText(QString("<center>(Discovering hop %1 of %2)</center>").arg(currentHop).arg(totalHops));
}
