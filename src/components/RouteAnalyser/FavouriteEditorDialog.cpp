/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 11/03/2021.
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

#include "FavouriteEditorDialog.h"

#include "Core/ICore.h"
#include "Utils.h"

#include <QTimer>

#include "ui_FavouriteEditorDialog.h"

Nedrysoft::RouteAnalyser::FavouriteEditorDialog::FavouriteEditorDialog(
        QString title,
        QVariantMap parameters,
        QWidget *parent) :

            QDialog(parent), ui(new Ui::FavouriteEditorDialog) {

    ui->setupUi(this);

    m_parametersMap = parameters;

    auto ipVersion = parameters["ipversion"].value<Nedrysoft::Core::IPVersion>();

    if (parameters.contains("name") && parameters["name"].isValid()) {
        ui->nameLineEdit->setText(parameters["name"].toString());
    } else {
        ui->nameLineEdit->setPlaceholderText(tr("Name of the favourite"));
    }

    if (parameters.contains("description") && parameters["description"].isValid()) {
        ui->descriptionLineEdit->setText(parameters["description"].toString());
    } else {
        ui->descriptionLineEdit->setPlaceholderText(tr("Description of the favourite"));
    }

    if (parameters.contains("host") && parameters["host"].isValid()) {
        ui->hostLineEdit->setText(parameters["host"].toString());
    } else {
        ui->hostLineEdit->setPlaceholderText(tr("Host name or IP address"));
    }

    if ( (ipVersion==Nedrysoft::Core::IPVersion::V4) || (ipVersion==Nedrysoft::Core::IPVersion::V6)) {
        ui->ipV4radioButton->setChecked(ipVersion==Nedrysoft::Core::IPVersion::V4);
    } else {
        ui->ipV4radioButton->setChecked(true);
    }

    if (parameters.contains("interval") &&
        parameters["interval"].isValid() &&
        Nedrysoft::Utils::parseIntervalString(parameters["interval"].toString()) ) {
        ui->intervalLineEdit->setText(Nedrysoft::Utils::intervalToString(parameters["interval"].toDouble() / 1000.0));
    } else {
        ui->intervalLineEdit->setText("2.5s");
    }

    connect(ui->okButton, &QPushButton::clicked, [=](bool checked) {
        double interval = 1;

        m_parametersMap["description"] = ui->descriptionLineEdit->text();
        m_parametersMap["name"] = ui->nameLineEdit->text();
        m_parametersMap["host"] = ui->hostLineEdit->text();
        Nedrysoft::Utils::parseIntervalString(ui->intervalLineEdit->text(), interval);
        m_parametersMap["interval"] = interval*1000.0;

        if (ui->ipV4radioButton->isChecked()) {
            m_parametersMap["ipversion"] =
                    QVariant::fromValue<Nedrysoft::Core::IPVersion>(Nedrysoft::Core::IPVersion::V4);
        } else {
            m_parametersMap["ipversion"] =
                    QVariant::fromValue<Nedrysoft::Core::IPVersion>(Nedrysoft::Core::IPVersion::V6);
        }

        accept();
    });

    connect(ui->cancelButton, &QPushButton::clicked, [=](bool checked) {
        reject();
    });

    setWindowTitle(title);

    // resize the dialog vertically to minimum size.

    QTimer::singleShot(0, [=]() {
        resize(width(), 0);
    });
}

Nedrysoft::RouteAnalyser::FavouriteEditorDialog::~FavouriteEditorDialog() {
    delete ui;
}

auto Nedrysoft::RouteAnalyser::FavouriteEditorDialog::map() -> QVariantMap {
    return m_parametersMap;
}
