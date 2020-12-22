/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 10/12/2020.
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

#include "ComponentSystem/IComponentManager.h"
#include "Core/ICommandManager.h"
#include "Core/IEditorManager.h"
#include "Core/IPingEngineFactory.h"
#include "LineSyntaxHighlighter.h"
#include "Ribbon/RibbonDropButton.h"
#include "RouteAnalyserEditor.h"
#include "Utils.h"

#include <QAbstractItemView>
#include <QMenu>
#include <QRegularExpressionMatch>

constexpr auto comboPadding = 12;
constexpr auto defaultInterval = "2.5s";
constexpr auto defaultTarget = "1.1.1.1";
constexpr auto lineEditHeight = 21;

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
        menu.addAction(tr("Edit Favourite..."));
        menu.addAction(tr("Save Favourite..."));

        menuPosition = mapToGlobal(menuPosition);

        menu.exec(menuPosition);
    });

    m_intervalHighlighter = new LineSyntaxHighlighter(ui->intervalLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    m_targetHighlighter = new LineSyntaxHighlighter(ui->targetLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::checkHostValid(text);
    });

    ui->intervalLineEdit->setPlaceholderText(defaultInterval);
    ui->targetLineEdit->setPlaceholderText(defaultTarget);

    auto pingEngines = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IPingEngineFactory>();

    if (pingEngines.count()) {
        auto minimumWidth = 0;
        for(auto pingEngine : pingEngines) {
            ui->engineComboBox->addItem(
                    pingEngine->description(),
                    QVariant::fromValue<Nedrysoft::Core::IPingEngineFactory *>(pingEngine) );

            QFontMetrics fontMetrics(ui->engineComboBox->font());

            minimumWidth = qMax(minimumWidth, fontMetrics.boundingRect(pingEngine->description()).width()+comboPadding);
        }

        ui->engineComboBox->view()->setMinimumWidth(minimumWidth);
        ui->engineComboBox->setPlaceholderText(pingEngines[0]->description());
    }

    connect(ui->startButton, &Nedrysoft::Ribbon::RibbonButton::clicked, [=](bool checked) {
        auto editorManager = Nedrysoft::Core::IEditorManager::getInstance();

        if (editorManager) {
            auto pingEngineFactory = ui->engineComboBox->currentData().value<Nedrysoft::Core::IPingEngineFactory *>();

            auto target = ui->targetLineEdit->toPlainText().isEmpty() ?
                    ui->targetLineEdit->placeholderText() :
                    ui->targetLineEdit->toPlainText();

            auto interval = ui->intervalLineEdit->toPlainText().isEmpty() ?
                    ui->intervalLineEdit->placeholderText() :
                    ui->intervalLineEdit->toPlainText();

            if (!Nedrysoft::Utils::checkHostValid(target)) {
                return;
            }

            double intervalTime;

            if (!Nedrysoft::Utils::parseIntervalString(interval, intervalTime)) {
                return;
            }

            RouteAnalyserEditor *editor = new RouteAnalyserEditor;

            editor->setPingEngine(pingEngineFactory);
            editor->setTarget(target);

            if (ui->ipV4RadioButton->isChecked()) {
                editor->setIPVersion(Nedrysoft::Core::IPVersion::V4);
            } else {
                editor->setIPVersion(Nedrysoft::Core::IPVersion::V6);
            }

            editor->setInterval(intervalTime);

            Nedrysoft::ComponentSystem::addObject(editor);

            editorManager->openEditor(editor);
        }
    });

    connect(ui->intervalLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [=]() {
        validateFields();
    });

    connect(ui->targetLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [=]() {
        validateFields();
    });

    validateFields();

    ui->targetLineEdit->setMaximumHeight(lineEditHeight);
    ui->intervalLineEdit->setMaximumHeight(lineEditHeight);
}

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::~NewTargetRibbonGroup() {
    delete ui;
}


auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::checkFieldsValid(QString &string) -> QWidget * {
    double intervalValue;
    QWidget *returnWidget = nullptr;

    auto target = ui->targetLineEdit->toPlainText().isEmpty() ?
                  ui->targetLineEdit->placeholderText() :
                  ui->targetLineEdit->toPlainText();

    auto interval = ui->intervalLineEdit->toPlainText().isEmpty() ?
                    ui->intervalLineEdit->placeholderText() :
                    ui->intervalLineEdit->toPlainText();

    if (!Nedrysoft::Utils::checkHostValid(target)) {
        returnWidget = ui->targetLineEdit;
    }

    if (!Nedrysoft::Utils::parseIntervalString(interval, intervalValue)) {
        if (!returnWidget) {
            returnWidget = ui->intervalLineEdit;
        }
    }

    m_intervalHighlighter->updateSyntax();
    m_targetHighlighter->updateSyntax();

    return returnWidget;
}

auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::validateFields() -> void {
    QString errorString;

    auto invalidWidget = checkFieldsValid(errorString);

    ui->startButton->setEnabled(invalidWidget==nullptr);
}
