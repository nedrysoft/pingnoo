/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
 * An open source ping path analyser
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

#include "NewTargetDialog.h"

#include "ComponentSystem/IComponentManager.h"
#include "Core/IPingEngineFactory.h"
#include "LineSyntaxHighlighter.h"
#include "Utils.h"
#include "ui_NewTargetDialog.h"

#include <QLineEdit>
#include <QPushButton>

constexpr auto lineEditHeightAdjustment = 2;

Nedrysoft::RouteAnalyser::NewTargetDialog::NewTargetDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Nedrysoft::RouteAnalyser::Ui::NewTargetDialog),
        m_targetHighlighter(nullptr),
        m_intervalHighlighter(nullptr) {

    static int minimumLineHeight = 0;

    if (!minimumLineHeight) {
        QLineEdit lineEdit;

        minimumLineHeight = lineEdit.minimumSizeHint().height()+lineEditHeightAdjustment;
    }

    ui->setupUi(this);

    auto pingEngines = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IPingEngineFactory>();

    for (auto pingEngine : pingEngines) {
        ui->engineComboBox->addItem(
                pingEngine->description(),
                QVariant::fromValue<Nedrysoft::Core::IPingEngineFactory *>(pingEngine) );
    }

    m_intervalHighlighter = new LineSyntaxHighlighter(ui->intervalLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    m_targetHighlighter = new LineSyntaxHighlighter(ui->targetLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::checkHostValid(text);
    });

    ui->targetLineEdit->setLineWrapMode(QTextEdit::NoWrap);
    ui->targetLineEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->targetLineEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->targetLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    ui->targetLineEdit->setMaximumHeight(minimumLineHeight);

    connect(ui->targetLineEdit, &QTextEdit::textChanged, [=]() {
        validateFields();
    });

    ui->intervalLineEdit->setLineWrapMode(QTextEdit::NoWrap);
    ui->intervalLineEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->intervalLineEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->intervalLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    ui->intervalLineEdit->setMaximumHeight(minimumLineHeight);

    connect(ui->intervalLineEdit, &QTextEdit::textChanged, [=]() {
        validateFields();
    });

    validateFields();
}

Nedrysoft::RouteAnalyser::NewTargetDialog::~NewTargetDialog() {
    delete ui;
}

Nedrysoft::Core::IPingEngineFactory *Nedrysoft::RouteAnalyser::NewTargetDialog::pingEngineFactory() {
    return ui->engineComboBox->currentData().value<Nedrysoft::Core::IPingEngineFactory *>();
}

QString Nedrysoft::RouteAnalyser::NewTargetDialog::pingTarget() {
    if (ui->targetLineEdit->toPlainText().isEmpty()) {
        return ui->targetLineEdit->placeholderText();
    }

    return ui->targetLineEdit->toPlainText();
}

Nedrysoft::Core::IPVersion Nedrysoft::RouteAnalyser::NewTargetDialog::ipVersion() {
    if (ui->ipv4RadioButton->isChecked()) {
        return Nedrysoft::Core::IPVersion::V4;
    } else {
        return Nedrysoft::Core::IPVersion::V6;
    }
}

double Nedrysoft::RouteAnalyser::NewTargetDialog::interval() {
    double intervalTime = 1;
    auto intervalString = ui->intervalLineEdit->toPlainText().isEmpty() ?
                  ui->intervalLineEdit->placeholderText() :
                  ui->intervalLineEdit->toPlainText();

    if (!Nedrysoft::Utils::parseIntervalString(intervalString, intervalTime)) {
        return intervalTime;
    }

    return intervalTime;
}

QWidget *Nedrysoft::RouteAnalyser::NewTargetDialog::checkFieldsValid(QString &string) {
    double intervalValue;
    QWidget *returnWidget = nullptr;

    auto target = ui->targetLineEdit->toPlainText().isEmpty() ?
                  ui->targetLineEdit->placeholderText() :
                  ui->targetLineEdit->toPlainText();

    auto interval = ui->intervalLineEdit->toPlainText().isEmpty() ?
                    ui->intervalLineEdit->placeholderText() :
                    ui->intervalLineEdit->toPlainText();

    if (!Nedrysoft::Utils::checkHostValid(target)) {
        returnWidget=ui->targetLineEdit;
    }

    if (!Nedrysoft::Utils::parseIntervalString(interval, intervalValue)) {
        if (!returnWidget) {
            returnWidget=ui->intervalLineEdit;
        }
    }

    m_intervalHighlighter->updateSyntax();
    m_targetHighlighter->updateSyntax();

    return returnWidget;
}

void Nedrysoft::RouteAnalyser::NewTargetDialog::validateFields() {
    QString errorString;

    auto invalidWidget = checkFieldsValid(errorString);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(invalidWidget == nullptr ? true : false);
}
