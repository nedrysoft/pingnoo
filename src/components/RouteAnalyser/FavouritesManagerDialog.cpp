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
#include "FavouriteEditorDialog.h"
#if defined(Q_OS_MACOS)
#include "MacHelper/MacHelper.h"
#endif
#include "TargetManager.h"
#include "Utils.h"

#include "ui_FavouritesManagerDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>

constexpr auto MillisecondsInSecond = 1000.0;
constexpr auto DataRole = Qt::UserRole+1;
constexpr auto DataColumn = 0;

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

    connect(ui->cancelPushButton,
            &QPushButton::clicked,
            this,
            &Nedrysoft::RouteAnalyser::FavouritesManagerDialog::onCancelClicked);

    connect(ui->deletePushButton, &QPushButton::clicked, [=](bool checked) {
        m_itemModel->removeRow(ui->treeView->currentIndex().row());

        m_modelDirty = true;

        updateButtons();
    });

    connect(ui->duplicatePushButton, &QPushButton::clicked, [=](bool checked) {
        if (ui->treeView->currentIndex().isValid()) {
            auto index = ui->treeView->currentIndex();

            index = m_itemModel->index(index.row(), DataColumn);

            QVariant data = m_itemModel->data(index, DataRole);

            auto items = createFavourite(data.toMap());

            m_itemModel->insertRow(ui->treeView->currentIndex().row(), items);

            m_modelDirty = true;

            updateButtons();
        }
    });

    connect(ui->applyPushButton, &QPushButton::clicked, [=](bool checked) {
        applyChanges();

        m_modelDirty = false;

        updateButtons();
    });

    connect(ui->okPushButton,
            &QPushButton::clicked,
            this,
            &Nedrysoft::RouteAnalyser::FavouritesManagerDialog::onOkClicked);

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

        m_modelDirty = true;

        updateButtons();
    });

    connect(ui->newPushButton, &QPushButton::clicked, [=](bool checked) {
        QVariantMap newItemMap;

        FavouriteEditorDialog favouriteEditorDialog(tr("New Favourite"), newItemMap, this);

        if (favouriteEditorDialog.exec()) {
            auto items = createFavourite(favouriteEditorDialog.map());

            m_itemModel->appendRow(items);

            m_modelDirty = true;

            updateButtons();
        }
    });

    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->treeView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            [=](QModelIndex current, QModelIndex previous) {
                updateButtons();
    });

    connect(ui->treeView, &QTreeView::doubleClicked, this, &FavouritesManagerDialog::onEditFavourite);
    connect(ui->editPushButton, &QPushButton::clicked, [=](bool clicked) {
        onEditFavourite(ui->treeView->currentIndex());
    });

    m_itemModel->setHorizontalHeaderLabels(
            QStringList() <<
            tr("Name") <<
            tr("Description") <<
            tr("Host") <<
            tr("IP Version") <<
            tr("Interval"));

    ui->treeView->setCurrentIndex(m_itemModel->index(0, 0));

    auto favourites = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->favourites();

    for (auto favourite : favourites) {
        auto items = createFavourite(favourite);

        m_itemModel->appendRow(items);
    }

    for (int columnIndex=0;columnIndex<m_itemModel->columnCount();columnIndex++) {
        ui->treeView->resizeColumnToContents(columnIndex);
    }

    m_modelDirty = false;

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

    auto intervalString = Nedrysoft::Utils::intervalToString(favourite["interval"].toDouble()/MillisecondsInSecond);

    auto nameItem = new QStandardItem(favourite["name"].toString());
    auto descriptionItem = new QStandardItem(favourite["description"].toString());
    auto hostItem = new QStandardItem(favourite["host"].toString());
    auto versionItem = new QStandardItem(versionString);
    auto intervalItem = new QStandardItem(intervalString);

    nameItem->setData(favourite);

    auto items = QList<QStandardItem *>() << nameItem << descriptionItem << hostItem << versionItem << intervalItem;

    return items;
}

void Nedrysoft::RouteAnalyser::FavouritesManagerDialog::onEditFavourite(const QModelIndex &index) {
    auto selectedItem = m_itemModel->item(index.row());

    if (selectedItem) {

        auto map = selectedItem->data(DataRole).toMap();

        Nedrysoft::RouteAnalyser::FavouriteEditorDialog dialog(tr("Edit Favourite"), map, this);

        if (dialog.exec()) {
            map = dialog.map();
            QString versionString;

            switch(map["ipversion"].value<Nedrysoft::Core::IPVersion>()) {
                case Nedrysoft::Core::IPVersion::V4: {
                    versionString = "4";
                    break;
                }

                case Nedrysoft::Core::IPVersion::V6: {
                    versionString = "6";
                    break;
                }
            }

            auto intervalString = Nedrysoft::Utils::intervalToString(map["interval"].toDouble()/MillisecondsInSecond);

            m_itemModel->item(index.row(), Name)->setText(map["name"].toString());
            m_itemModel->item(index.row(), Description)->setText(map["description"].toString());
            m_itemModel->item(index.row(), Host)->setText(map["host"].toString());
            m_itemModel->item(index.row(), IPVersion)->setText(versionString);
            m_itemModel->item(index.row(), Interval)->setText(intervalString);

            selectedItem->setData(map);
        }
    }
}

auto Nedrysoft::RouteAnalyser::FavouritesManagerDialog::applyChanges() -> void {
    auto targetManager = Nedrysoft::RouteAnalyser::TargetManager::getInstance();

    QList<QVariantMap> favouritesList;

    for (int itemIndex=0;itemIndex<m_itemModel->rowCount();itemIndex++) {
        favouritesList.append(m_itemModel->item(itemIndex, DataColumn)->data(DataRole).toMap());
    }

    targetManager->setFavourites(favouritesList);
}

void Nedrysoft::RouteAnalyser::FavouritesManagerDialog::onCancelClicked(bool checked) {
    Q_UNUSED(checked)

    reject();
}

void Nedrysoft::RouteAnalyser::FavouritesManagerDialog::onOkClicked(bool checked) {
    Q_UNUSED(checked)

    applyChanges();

    accept();
}
