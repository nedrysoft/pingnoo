/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 01/02/2021.
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

#include "HostMaskerSettingsPageWidget.h"

#include "ComponentSystem/IComponentManager.h"
#include "Core.h"
#include "IHostMaskerSettingsPage.h"

#include "ui_HostMaskerSettingsPageWidget.h"

Nedrysoft::Core::HostMaskerSettingsPageWidget::HostMaskerSettingsPageWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::HostMaskerSettingsPageWidget) {

    ui->setupUi(this);

}

Nedrysoft::Core::HostMaskerSettingsPageWidget::~HostMaskerSettingsPageWidget() {
    delete ui;
}

auto Nedrysoft::Core::HostMaskerSettingsPageWidget::initialise() -> void {
    auto maskers = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IHostMaskerSettingsPage>();

    ui->maskersTreeWidget->clear();

    ui->maskersTreeWidget->setHeaderLabels(QStringList() << tr("Masker Type"));

    for(auto masker : maskers) {
        QTreeWidgetItem *maskerItem = new QTreeWidgetItem;

        maskerItem->setText(0, masker->displayName());

        auto maskerWidget = masker->widget();

        ui->maskersTreeWidget->addTopLevelItem(maskerItem);
        ui->maskersStackedWidget->addWidget(maskerWidget);

        maskerItem->setData(0, Qt::UserRole, QVariant::fromValue(maskerWidget));
    }

    connect(ui->maskersTreeWidget,
            &QTreeWidget::currentItemChanged,
            [=](QTreeWidgetItem *current, QTreeWidgetItem *previous) {

        auto widget = current->data(0, Qt::UserRole).value<QWidget *>();

        if (widget) {
            ui->maskersStackedWidget->setCurrentWidget(widget);
        }
    });
}