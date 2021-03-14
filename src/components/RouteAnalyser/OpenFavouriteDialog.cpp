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

#include "OpenFavouriteDialog.h"

#include "FavouritesSortProxyFilterModel.h"
#include "TargetManager.h"
#include "Utils.h"

#include "ui_OpenFavouriteDialog.h"

constexpr auto MillisecondsInSecond = 1000.0;

Nedrysoft::RouteAnalyser::OpenFavouriteDialog::OpenFavouriteDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::OpenFavouriteDialog) {

    ui->setupUi(this);

    connect(ui->closePushButton, &QPushButton::clicked, this, &Nedrysoft::RouteAnalyser::OpenFavouriteDialog::onCloseClicked);

    auto favourites = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->favourites();

    for (auto favourite : favourites) {
        m_itemModel.appendRow(createFavourite(favourite));
    }

    m_filterModel.setSourceModel(&m_itemModel);

    m_itemModel.setHorizontalHeaderLabels(
            QStringList() <<
            tr("Name") <<
            tr("Description") <<
            tr("Host") <<
            tr("IP Version") <<
            tr("Interval") );

    ui->favouritesTreeView->setModel(&m_filterModel);

    ui->favouritesTreeView->setCurrentIndex(m_filterModel.index(0, 0));

    connect(ui->searchLineEdit, &QLineEdit::textChanged, [=](QString text) {
        m_filterModel.setFilterText(text);
    });

    connect(ui->favouritesTreeView,
            &QTreeView::doubleClicked,
            this,
            &Nedrysoft::RouteAnalyser::OpenFavouriteDialog::onTreeviewDoubleClicked);

    for (int columnIndex=0;columnIndex<m_itemModel.columnCount();columnIndex++) {
        ui->favouritesTreeView->resizeColumnToContents(columnIndex);
    }
}

Nedrysoft::RouteAnalyser::OpenFavouriteDialog::~OpenFavouriteDialog() {
    delete ui;
}

auto Nedrysoft::RouteAnalyser::OpenFavouriteDialog::selectedItem() -> QVariantMap {
    return m_selectedItem;
}

auto Nedrysoft::RouteAnalyser::OpenFavouriteDialog::createFavourite(QVariantMap favourite) -> QList<QStandardItem *> {
    QString versionString;

    switch(favourite["ipversion"].value<Nedrysoft::Core::IPVersion>()) {
        case Nedrysoft::Core::IPVersion::V4: {
            versionString = "4";
            break;
        }

        case Nedrysoft::Core::IPVersion::V6: {
            versionString = "6";
            break;
        }

        default:
            return QList<QStandardItem *>();
    }

    auto intervalString = Nedrysoft::Utils::intervalToString(favourite["interval"].toDouble()/MillisecondsInSecond);

    auto nameItem = new QStandardItem(favourite["name"].toString());
    auto descriptionItem = new QStandardItem(favourite["description"].toString());
    auto hostItem = new QStandardItem(favourite["host"].toString());
    auto versionItem = new QStandardItem(versionString);
    auto intervalItem = new QStandardItem(intervalString);

    nameItem->setData(favourite);

    return QList<QStandardItem *>() << nameItem << descriptionItem << hostItem << versionItem << intervalItem;
}

void Nedrysoft::RouteAnalyser::OpenFavouriteDialog::onCloseClicked(bool checked) {
    Q_UNUSED(checked)

    reject();
}

void Nedrysoft::RouteAnalyser::OpenFavouriteDialog::onTreeviewDoubleClicked(const QModelIndex &index) {
    m_selectedItem = m_filterModel.data(index, Qt::UserRole+1).toMap();

    accept();
}