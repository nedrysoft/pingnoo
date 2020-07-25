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

#include "ComponentViewerDialog.h"
#include "ui_ComponentViewerDialog.h"
#include "ComponentSystem/ComponentLoader.h"
#include "ComponentSystem/IComponentManager.h"
#include "ComponentSystem/Component.h"
#include <QTreeWidgetItem>
#include "ComponentDetailsDialog.h"
#include "FontAwesome/FontAwesome.h"

ComponentViewerDialog::ComponentViewerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentViewerDialog)
{
    ui->setupUi(this);

    auto minusIcon = FizzyAde::FontAwesome::FontAwesome::icon("fas fa-minus", 16, Qt::darkRed);
    auto crossIcon = FizzyAde::FontAwesome::FontAwesome::icon("fas fa-times", 16, Qt::darkRed);
    auto tickIcon = FizzyAde::FontAwesome::FontAwesome::icon("fas fa-check", 16, Qt::darkGreen);

    ui->componentsTreeWidget->setHeaderLabels(QStringList() << "Name" << "Load" << "Version" << "Vendor");

    ui->componentsTreeWidget->setColumnWidth(0, 300);
    ui->componentsTreeWidget->setColumnWidth(1, 50);
    ui->componentsTreeWidget->setColumnWidth(2, 300);
    ui->componentsTreeWidget->setColumnWidth(3, 200);

    QMap<QString, QMap<QString, FizzyAde::ComponentSystem::Component *> > categoryMap;

    auto componentLoader = FizzyAde::ComponentSystem::getObject<FizzyAde::ComponentSystem::ComponentLoader>();

    auto components = componentLoader->components();

    for(auto component : components) {
        categoryMap[component->category()][component->name()] = component;
    }

    QMapIterator<QString, QMap<QString, FizzyAde::ComponentSystem::Component *> > categoryIterator(categoryMap);

    while(categoryIterator.hasNext()) {
        categoryIterator.next();

        auto categoryItem = new QTreeWidgetItem;

        categoryItem->setText(0, categoryIterator.key());

        QMapIterator<QString, FizzyAde::ComponentSystem::Component *> componentIterator(categoryIterator.value());

        while(componentIterator.hasNext()) {
            componentIterator.next();

            auto component = componentIterator.value();

            auto componentItem =  new QTreeWidgetItem;

            if (component->loadStatus() == FizzyAde::ComponentSystem::ComponentLoader::Loaded) {
                componentItem->setIcon(0, tickIcon);
            } else {
                if (component->loadStatus() == FizzyAde::ComponentSystem::ComponentLoader::Disabled) {
                    componentItem->setIcon(0, minusIcon);
                } else {
                    componentItem->setIcon(0, crossIcon);
                }
            }

            componentItem->setText(0, component->name());

            if (component->loadStatus() == FizzyAde::ComponentSystem::ComponentLoader::Disabled) {
                componentItem->setCheckState(1, Qt::Unchecked);
            } else {
                componentItem->setCheckState(1, Qt::Checked);
            }

            componentItem->setText(2, component->versionString());
            componentItem->setText(3, component->vendor());

            componentItem->setData(0, Qt::UserRole, QVariant::fromValue<FizzyAde::ComponentSystem::Component *>(component));

            categoryItem->addChild(componentItem);
        }

        ui->componentsTreeWidget->addTopLevelItem(categoryItem);
    }

    ui->componentsTreeWidget->expandAll();
}

ComponentViewerDialog::~ComponentViewerDialog()
{
    delete ui;
}

void ComponentViewerDialog::on_componentsTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)

    auto component = item->data(0, Qt::UserRole).value<FizzyAde::ComponentSystem::Component *>();

    if (component) {
        ComponentDetailsDialog detailsDialog(component);

        detailsDialog.exec();
    }
}

QStringList ComponentViewerDialog::disabledComponents()
{
    QStringList disabledComponentList;

    for (auto categoryIndex=0;categoryIndex<ui->componentsTreeWidget->topLevelItemCount();categoryIndex++) {
        auto categoryItem = ui->componentsTreeWidget->topLevelItem(categoryIndex);

        for (auto itemIndex=0;itemIndex<categoryItem->childCount();itemIndex++) {
            auto componentItem = categoryItem->child(itemIndex);

            if (componentItem->checkState(1)==Qt::Unchecked) {
                FizzyAde::ComponentSystem::Component *component = componentItem->data(0, Qt::UserRole).value<FizzyAde::ComponentSystem::Component *>();

                if (component) {
                    disabledComponentList.append((component->name()+"."+component->vendor()).toLower());
                }
            }
        }
    }

    return disabledComponentList;
}
