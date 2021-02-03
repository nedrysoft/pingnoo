/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 02/02/2021.
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

#include "RegExHostMaskerSettingsPageWidget.h"

#include "RegExHostMasker.h"
#include "ComponentSystem/IComponentManager.h"

#include "ui_RegExHostMaskerSettingsPageWidget.h"

#include <QStandardPaths>

Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::RegExHostMaskerSettingsPageWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::RegExHostMaskerSettingsPageWidget) {

    ui->setupUi(this);

    auto headerLabels = QStringList() <<
        tr("Description") <<
        tr("Match\r\nHop") <<
        tr("Match\r\nAddress") <<
        tr("Match\r\nHost") <<
        tr("Mask\r\nAddress") <<
        tr("Mask\r\nHost");

    RegExHostMasker::MatchFlags matchFlags[] = {
            RegExHostMasker::MatchFlags::MatchHop,
            RegExHostMasker::MatchFlags::MatchAddress,
            RegExHostMasker::MatchFlags::MatchHost,
            RegExHostMasker::MatchFlags::MaskAddress,
            RegExHostMasker::MatchFlags::MaskHost
    };

    ui->expressionsTreeWidget->header()->setDefaultAlignment(Qt::AlignHCenter);
    ui->expressionsTreeWidget->setHeaderLabels(headerLabels);

    auto hostMasker = Nedrysoft::ComponentSystem::getObject<RegExHostMasker>();

    if (!hostMasker) {
        return;
    }

    QFontMetrics fontMetrics(ui->expressionsTreeWidget->header()->font());

    for (auto masker : hostMasker->m_maskList) {
        auto treeItem = new QTreeWidgetItem;

        treeItem->setText(0, masker.m_description);

        ui->expressionsTreeWidget->addTopLevelItem(treeItem);

        // bit of a hack, can't centre the checkbox directly, so stick it in a widget with a centered layout.

        for (int column = 1; column<headerLabels.count(); column++) {
            auto checkBox = new QCheckBox;
            auto containerWidget = new QWidget;
            auto containerLayout = new QHBoxLayout;

            if (masker.m_matchFlags & static_cast<int>(matchFlags[column-1])) {
                checkBox->setCheckState(Qt::Checked);
            } else {
                checkBox->setCheckState(Qt::Unchecked);
            }

            containerLayout->setAlignment(Qt::AlignCenter);
            containerLayout->setContentsMargins(1, 1, 1, 1);

            containerWidget->setLayout(containerLayout);

            containerLayout->addWidget(checkBox);

            ui->expressionsTreeWidget->setItemWidget(treeItem, column, containerWidget);
        }
    }

    // resize the checkbox columns and resize the first column to be the remaining space

    ui->expressionsTreeWidget->header()->setStretchLastSection(false);

    auto resizedColumnsWidth = 0;

    for (int column = 1; column<headerLabels.count(); column++) {
        auto columnWidth = fontMetrics.boundingRect(headerLabels.at(column)).width();

        ui->expressionsTreeWidget->setColumnWidth(column, columnWidth);

        resizedColumnsWidth += columnWidth-8;
    }

    ui->expressionsTreeWidget->setColumnWidth(0, contentsRect().width()-resizedColumnsWidth);
}

Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::~RegExHostMaskerSettingsPageWidget() {
    delete ui;
}
