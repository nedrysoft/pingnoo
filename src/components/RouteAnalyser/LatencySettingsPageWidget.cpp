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

#include "LatencySettingsPageWidget.h"

#include "ColourManager.h"
#include "ComponentSystem/IComponentManager.h"
#include "LatencySettings.h"
#include "ui_LatencySettingsPageWidget.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QStandardPaths>

Nedrysoft::RouteAnalyser::LatencySettingsPageWidget::LatencySettingsPageWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LatencySettingsPageWidget) {

    LatencySettings *latencySettings = Nedrysoft::ComponentSystem::getObject<LatencySettings>();

    ui->setupUi(this);

    ui->idealWidget->setText(tr("ideal"));
    ui->warningWidget->setText(tr("warning"));
    ui->criticalWidget->setText(tr("critical"));

    ui->warningLineEdit->setText(latencySettings->toString(latencySettings->warningValue()));
    ui->criticalLineEdit->setText(latencySettings->toString(latencySettings->criticalValue()));

    ui->idealWidget->setColour(ColourManager::getIdealColour());
    ui->warningWidget->setColour(ColourManager::getWarningColour());
    ui->criticalWidget->setColour(ColourManager::getCriticalColour());
}

Nedrysoft::RouteAnalyser::LatencySettingsPageWidget::~LatencySettingsPageWidget() {
    delete ui;
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPageWidget::canAcceptSettings() -> bool {
    return true;
};

auto Nedrysoft::RouteAnalyser::LatencySettingsPageWidget::acceptSettings() -> void {
    auto latencySettings = Nedrysoft::ComponentSystem::getObject<LatencySettings>();

    assert(latencySettings!=nullptr);

    latencySettings->setWarningValue(ui->warningLineEdit->text());
    latencySettings->setCriticalValue(ui->criticalLineEdit->text());

    latencySettings->saveToFile();
}