/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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
#include "Utils.h"
#include "ui_ViewportRibbonGroup.h"

#include <QDateTime>

constexpr auto defaultViewportSize = 10*60;

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
        Q_EMIT viewportChanged(start, end);
        // TODO: add logic.
    });

    connect(ui->durationComboBox, &QComboBox::currentTextChanged, [=](QString text) {
        double value;

        if (Nedrysoft::Utils::parseIntervalString(text, value)) {
            Q_EMIT viewportWindowChanged(value);
        }
    });

    ui->trimmerWidget->setViewport(0, 1);
    ui->trimmerWidget->setEnabled(false);
}

Nedrysoft::RouteAnalyser::ViewportRibbonGroup::~ViewportRibbonGroup() {
    delete ui;
}

auto Nedrysoft::RouteAnalyser::ViewportRibbonGroup::setViewport(double start, double end) -> void {
    ui->trimmerWidget->setViewport(start, end);
}

auto Nedrysoft::RouteAnalyser::ViewportRibbonGroup::setViewportEnabled(bool enabled) -> void {
    ui->trimmerWidget->setEnabled(enabled);
}

auto Nedrysoft::RouteAnalyser::ViewportRibbonGroup::isViewportEnabled() -> bool {
    return ui->trimmerWidget->isEnabled();
}

auto Nedrysoft::RouteAnalyser::ViewportRibbonGroup::setStartAndEnd(double start, double end) -> void {
    ui->startLineEdit->setText(QDateTime::fromTime_t(start).toString());
    ui->endLineEdit->setText(QDateTime::fromTime_t(end).toString());
}

auto Nedrysoft::RouteAnalyser::ViewportRibbonGroup::viewportSize() -> double {
    double value;

    if (Nedrysoft::Utils::parseIntervalString(ui->durationComboBox->currentText(), value)) {
        return value;
    }

    return defaultViewportSize;
}
