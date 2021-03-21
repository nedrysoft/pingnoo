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

#include "LatencyRibbonGroup.h"

#include "ColourManager.h"
#include "ComponentSystem/IComponentManager.h"
#include "LineSyntaxHighlighter.h"
#include "LatencySettings.h"
#include "RouteAnalyserEditor.h"
#include "Utils.h"
#include "ui_LatencyRibbonGroup.h"

#include <QMenu>
#include <cassert>

constexpr auto lineEditHeight = 21;

Nedrysoft::RouteAnalyser::LatencyRibbonGroup::LatencyRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LatencyRibbonGroup) {

    ui->setupUi(this);

    auto latencySettings = Nedrysoft::RouteAnalyser::LatencySettings::getInstance();

    assert(latencySettings!=nullptr);

    ui->idealWidget->setText(tr("Ideal"));
    ui->warningWidget->setText(tr("Warning"));
    ui->criticalWidget->setText(tr("Critical"));

    ui->idealWidget->setFixedHeight(ui->criticalWidget->height());
    ui->idealWidget->setFixedWidth(ui->criticalWidget->width());

    m_warningHighlighter = new LineSyntaxHighlighter(ui->warningLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    m_criticalHighlighter = new LineSyntaxHighlighter(ui->criticalLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    ui->idealWidget->setColour(latencySettings->idealColour());
    ui->warningWidget->setColour(latencySettings->warningColour());
    ui->criticalWidget->setColour(latencySettings->criticalColour());

    ui->warningLineEdit->setPlaceholderText(getValueString(LatencyType::Warning));
    ui->criticalLineEdit->setPlaceholderText(getValueString(LatencyType::Critical));

    ui->warningLineEdit->setMaximumHeight(lineEditHeight);
    ui->criticalLineEdit->setMaximumHeight(lineEditHeight);

    connect(ui->warningLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [=]() {
        updatePeriod(LatencyType::Warning, ui->warningLineEdit);

        m_warningHighlighter->updateSyntax();
    });

    connect(ui->criticalLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [=]() {
        updatePeriod(LatencyType::Critical, ui->criticalLineEdit);

        m_criticalHighlighter->updateSyntax();
    });

    connect(latencySettings, &LatencySettings::coloursChanged, [=]() {
        ui->idealWidget->setColour(latencySettings->idealColour());
        ui->warningWidget->setColour(latencySettings->warningColour());
        ui->criticalWidget->setColour(latencySettings->criticalColour());
    });

    connect(ui->idealWidget, &LatencyWidget::colourChanged, [=](QColor colour) {
        latencySettings->setIdealColour(colour.rgb());
    });

    connect(ui->warningWidget, &LatencyWidget::colourChanged, [=](QColor colour) {
        latencySettings->setWarningColour(colour.rgb());
    });

    connect(ui->criticalWidget, &LatencyWidget::colourChanged, [=](QColor colour) {
        latencySettings->setCriticalColour(colour.rgb());
    });
}

Nedrysoft::RouteAnalyser::LatencyRibbonGroup::~LatencyRibbonGroup() {
    delete ui;
}

auto Nedrysoft::RouteAnalyser::LatencyRibbonGroup::updatePeriod(
        LatencyType type,
        Nedrysoft::Ribbon::RibbonLineEdit *lineEdit) -> void {

    double intervalValue;

    if (!Nedrysoft::Utils::parseIntervalString(lineEdit->toPlainText(), intervalValue)) {
        return;
    }

    Q_EMIT valueChanged(type, intervalValue);

    /*for(auto editor: Nedrysoft::ComponentSystem::getObjects<Nedrysoft::RouteAnalyser::RouteAnalyserEditor>()) {
        // TODO: update editors
    }*/
}

auto Nedrysoft::RouteAnalyser::LatencyRibbonGroup::setValue(LatencyType type, double value) -> void {
    Q_UNUSED(value)

    switch(type) {
        case LatencyType::Ideal: {
            break;
        }

        case LatencyType::Warning: {
            break;
        }

        case LatencyType::Critical: {
            break;
        }
    }
}

auto Nedrysoft::RouteAnalyser::LatencyRibbonGroup::getValueString(LatencyType type) -> QString {
    auto latencySettings = Nedrysoft::ComponentSystem::getObject<LatencySettings>();
    double value = 0;

    switch(type) {
        case LatencyType::Warning: {
            value = latencySettings->warningValue();
            break;
        }

        case LatencyType::Critical: {
            value = latencySettings->criticalValue();
            break;
        }

        default: {
            break;
        }
    }

    return Nedrysoft::Utils::intervalToString(value);
}