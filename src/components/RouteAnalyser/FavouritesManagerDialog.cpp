/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 23/02/2021.
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

#include "FavouritesManagerDialog.h"

#include "TargetManager.h"

#include "ui_FavouritesManagerDialog.h"

Nedrysoft::RouteAnalyser::FavouritesManagerDialog::FavouritesManagerDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::FavouritesManagerDialog),
        m_modelDirty(false) {

    ui->setupUi(this);

    m_itemModel = new QStandardItemModel;

    connect(m_itemModel, &QStandardItemModel::itemChanged, [=](QStandardItem *) {
        m_modelDirty = true;
        updateButtons();
    });

    m_itemModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Description") << tr("Host") << tr("IP Version") << tr("Interval"));

    ui->treeView->setModel(m_itemModel);

    auto favourites = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->favourites();

    m_itemModel->setRowCount(favourites.count());

    auto currentRow = 0;

    for (auto favourite : favourites) {
        m_itemModel->setItem(currentRow, 0, new QStandardItem(favourite["Name"].toString()));
        m_itemModel->setItem(currentRow, 1, new QStandardItem(favourite["Description"].toString()));
        m_itemModel->setItem(currentRow, 2, new QStandardItem(favourite["Host"].toString()));
        m_itemModel->setItem(currentRow, 3, new QStandardItem(favourite["IP Version"].toString()));
        m_itemModel->setItem(currentRow, 4, new QStandardItem(favourite["Interval"].toString()));

        currentRow++;
    }

    m_modelDirty = true;

    updateButtons();
}

Nedrysoft::RouteAnalyser::FavouritesManagerDialog::~FavouritesManagerDialog() {
    delete ui;

    delete m_itemModel;
}

auto Nedrysoft::RouteAnalyser::FavouritesManagerDialog::updateButtons() -> void {
    ui->deletePushButton->setEnabled((m_itemModel->rowCount()) && (ui->treeView->currentIndex().isValid()));
    ui->exportPushButton->setEnabled(m_itemModel->rowCount());
    ui->editPushButton->setEnabled((m_itemModel->rowCount()) && (ui->treeView->currentIndex().isValid()));
    ui->duplicatePushButton->setEnabled((m_itemModel->rowCount()) && (ui->treeView->currentIndex().isValid()));
    ui->applyPushButton->setEnabled(m_modelDirty);
}