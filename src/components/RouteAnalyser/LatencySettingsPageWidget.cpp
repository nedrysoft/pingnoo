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
#include "Utils.h"
#include "ui_LatencySettingsPageWidget.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QStandardPaths>
#include <cassert>

Nedrysoft::RouteAnalyser::LatencySettingsPageWidget::LatencySettingsPageWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LatencySettingsPageWidget) {

    LatencySettings *latencySettings = Nedrysoft::ComponentSystem::getObject<LatencySettings>();

    ui->setupUi(this);

    ui->idealWidget->setText(tr("ideal"));
    ui->warningWidget->setText(tr("warning"));
    ui->criticalWidget->setText(tr("critical"));

    m_connections.append(connect(ui->idealWidget, &LatencyWidget::colourChanged, [=](QColor colour) {
        ui->idealWidget->setColour(colour);
    }));

    m_connections.append(connect(ui->warningWidget, &LatencyWidget::colourChanged, [=](QColor colour) {
        ui->warningWidget->setColour(colour);
    }));

    m_connections.append(connect(ui->criticalWidget, &LatencyWidget::colourChanged, [=](QColor colour) {
        ui->criticalWidget->setColour(colour);
    }));

    m_connections.append(connect(latencySettings, &LatencySettings::coloursChanged, [=]() {
        ui->idealWidget->setColour(latencySettings->idealColour());
        ui->warningWidget->setColour(latencySettings->warningColour());
        ui->criticalWidget->setColour(latencySettings->criticalColour());
    }));

    m_connections.append(connect(ui->resetPushButton, &QPushButton::clicked, [=](bool) {
        // TODO: create a generic MessageBox which uses the native macOS message box or QMessageBox on other
        //       platforms.  Example can be found in libs/SettingsDialog/src/MacHalper.mm

        latencySettings->resetColours();
        latencySettings->resetThresholds();

        ui->idealWidget->setColour(latencySettings->idealColour());
        ui->warningWidget->setColour(latencySettings->warningColour());
        ui->criticalWidget->setColour(latencySettings->criticalColour());

        update();
    }));

    ui->warningLineEdit->setText(Nedrysoft::Utils::intervalToString(latencySettings->warningValue()));
    ui->criticalLineEdit->setText(Nedrysoft::Utils::intervalToString(latencySettings->criticalValue()));

    ui->idealWidget->setColour(latencySettings->idealColour());
    ui->warningWidget->setColour(latencySettings->warningColour());
    ui->criticalWidget->setColour(latencySettings->criticalColour());

    ui->gradientFillcheckBox->setChecked(latencySettings->gradientFill() ? Qt::Checked : Qt::Unchecked);
}

Nedrysoft::RouteAnalyser::LatencySettingsPageWidget::~LatencySettingsPageWidget() {
    for(auto connection: m_connections) {
        disconnect(connection);
    }

    ui->resetPushButton->disconnect(this);
    ui->criticalWidget->disconnect(this);
    ui->warningWidget->disconnect(this);
    ui->idealWidget->disconnect(this);

    delete ui;
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPageWidget::canAcceptSettings() -> bool {
    return true;
};

auto Nedrysoft::RouteAnalyser::LatencySettingsPageWidget::acceptSettings() -> void {
    auto latencySettings = Nedrysoft::ComponentSystem::getObject<LatencySettings>();

    assert(latencySettings!=nullptr);

    //latencySettings->blockSignals(true);

    latencySettings->setWarningValue(ui->warningLineEdit->text());
    latencySettings->setCriticalValue(ui->criticalLineEdit->text());

    latencySettings->setIdealColour(ui->idealWidget->colour().rgb());
    latencySettings->setWarningColour(ui->warningWidget->colour().rgb());
    latencySettings->setCriticalColour(ui->criticalWidget->colour().rgb());

    latencySettings->setGradientFill(ui->gradientFillcheckBox->isChecked());

    //latencySettings->blockSignals(false);

    latencySettings->saveToFile();
}
