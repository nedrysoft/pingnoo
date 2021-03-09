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

#include "Core/ICore.h"
#include "MacHelper/MacHelper.h"
#include "TargetManager.h"
#include "Utils.h"

#include "ui_FavouritesManagerDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>

Nedrysoft::RouteAnalyser::FavouritesManagerDialog::FavouritesManagerDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::FavouritesManagerDialog),
        m_modelDirty(false) {

    ui->setupUi(this);

    m_itemModel = new QStandardItemModel;

    ui->treeView->setModel(m_itemModel);

    connect(m_itemModel, &QStandardItemModel::itemChanged, [=](QStandardItem *) {
        m_modelDirty = true;
        updateButtons();
    });

    connect(ui->cancelPushButton, &QPushButton::clicked, [=](bool checked) {
        reject();
    });

    connect(ui->deletePushButton, &QPushButton::clicked, [=](bool checked) {
        m_itemModel->removeRow(ui->treeView->currentIndex().row());
    });

    connect(ui->duplicatePushButton, &QPushButton::clicked, [=](bool checked) {
        if (ui->treeView->currentIndex().isValid()) {
            auto index = ui->treeView->currentIndex();

            index = m_itemModel->index(index.row(), 0);

            QVariant data = m_itemModel->data(index, Qt::UserRole+1);

            auto items = createFavourite(data.toMap());

            m_itemModel->insertRow(ui->treeView->currentIndex().row(), items);
        }
    });

    connect(ui->exportPushButton, &QPushButton::clicked, [=](bool checked) {
        auto filename = QFileDialog::getSaveFileName(Nedrysoft::Core::mainWindow());

        if (!filename.isNull()) {
            auto targetManager = Nedrysoft::RouteAnalyser::TargetManager::getInstance();

            targetManager->saveFavourites(filename);
        }
    });

    connect(ui->importPushButton, &QPushButton::clicked, [=](bool checked) {
        auto targetManager = Nedrysoft::RouteAnalyser::TargetManager::getInstance();

        targetManager->importFavourites(this);
    });

    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->treeView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            [=](QModelIndex current, QModelIndex previous) {
                updateButtons();
    });

    m_itemModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Description") << tr("Host") << tr("IP Version") << tr("Interval"));

    ui->treeView->setCurrentIndex(m_itemModel->index(0, 0));

    auto favourites = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->favourites();

    for (auto favourite : favourites) {
        auto items = createFavourite(favourite);

        m_itemModel->appendRow(items);
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

auto Nedrysoft::RouteAnalyser::FavouritesManagerDialog::createFavourite(QVariantMap favourite) -> QList<QStandardItem *> {
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

    auto intervalString = Nedrysoft::Utils::intervalToString(favourite["interval"].toDouble()/1000.0);

    auto nameItem = new QStandardItem(favourite["name"].toString());
    auto descriptionItem = new QStandardItem(favourite["description"].toString());
    auto hostItem = new QStandardItem(favourite["host"].toString());
    auto versionItem = new QStandardItem(versionString);
    auto intervalItem = new QStandardItem(intervalString);

    nameItem->setData(favourite);

    auto items = QList<QStandardItem *>() << nameItem << descriptionItem << hostItem << versionItem << intervalItem;

    return items;
}
