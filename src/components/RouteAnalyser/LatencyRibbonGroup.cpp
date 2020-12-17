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

#include "LatencyRibbonGroup.h"

#include "ui_LatencyRibbonGroup.h"
#include "ColourManager.h"
#include "ComponentSystem/IComponentManager.h"
#include "LineSyntaxHighlighter.h"
#include "Ribbon/RibbonLineEdit.h"
#include "RouteAnalyser.h"
#include "RouteAnalyserEditor.h"
#include "Utils.h"

#include <QMenu>

auto constexpr idealDefaultValue = "100ms";
auto constexpr warningDefaultValue = "200ms";
auto constexpr criticalDefaultValue = "300ms";

constexpr auto lineEditHeight = 21;

Nedrysoft::RouteAnalyser::LatencyRibbonGroup::LatencyRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LatencyRibbonGroup) {

    ui->setupUi(this);

    m_idealHighlighter = new LineSyntaxHighlighter(ui->idealLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    m_warningHighlighter = new LineSyntaxHighlighter(ui->warningLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    m_criticalHighlighter = new LineSyntaxHighlighter(ui->criticalLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    ui->idealWidget->setColour(ColourManager::getIdealColour());
    ui->warningWidget->setColour(ColourManager::getWarningColour());
    ui->criticalWidget->setColour(ColourManager::getCriticalColour());

    ui->idealLineEdit->setPlaceholderText(idealDefaultValue);
    ui->warningLineEdit->setPlaceholderText(warningDefaultValue);
    ui->criticalLineEdit->setPlaceholderText(criticalDefaultValue);

    ui->idealLineEdit->setMaximumHeight(lineEditHeight);
    ui->warningLineEdit->setMaximumHeight(lineEditHeight);
    ui->criticalLineEdit->setMaximumHeight(lineEditHeight);

    connect(ui->idealLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [=]() {
        updatePeriod(ui->idealLineEdit);

        m_idealHighlighter->updateSyntax();
    });

    connect(ui->warningLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [=]() {
        updatePeriod(ui->warningLineEdit);

        m_warningHighlighter->updateSyntax();
    });

    connect(ui->criticalLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [=]() {
        updatePeriod(ui->criticalLineEdit);

        m_criticalHighlighter->updateSyntax();
    });
}

Nedrysoft::RouteAnalyser::LatencyRibbonGroup::~LatencyRibbonGroup() {
    delete ui;
}

void Nedrysoft::RouteAnalyser::LatencyRibbonGroup::updatePeriod(Nedrysoft::Ribbon::RibbonLineEdit *lineEdit) {
    double intervalValue;

    if (!Nedrysoft::Utils::parseIntervalString(lineEdit->toPlainText(), intervalValue)) {
        return;
    }

    for(auto editor: Nedrysoft::ComponentSystem::getObjects<Nedrysoft::RouteAnalyser::RouteAnalyserEditor>()) {
        // TODO: update editors
    }
}