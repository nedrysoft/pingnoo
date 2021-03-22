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
#include "Core/ICore.h"

#include "ui_RegExHostMaskerSettingsPageWidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <cassert>

constexpr auto defaultWidgetWidth = 650;
constexpr auto defaultWidgetHeight = 0;
constexpr auto columnPadding = 8;
constexpr auto toolbarAdjustment = 50;

Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::RegExHostMaskerSettingsPageWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::RegExHostMaskerSettingsPageWidget),
        m_loadingConfiguration(true) {

    ui->setupUi(this);

    auto hostMasker = Nedrysoft::ComponentSystem::getObject<RegExHostMasker>();

    assert(hostMasker!=nullptr);

    connect(ui->importButton, &QPushButton::clicked, [=](bool checked) {
        auto filename = QFileDialog::getOpenFileName(Nedrysoft::Core::mainWindow(), tr("Import Configuration"));

        if (!filename.isEmpty()) {
            auto messageBox = new QMessageBox(Nedrysoft::ComponentSystem::getObject<QMainWindow>());
            bool append;

            messageBox->setText(tr("Do you want to append to or overwrite the existing configuration?"));
            messageBox->setWindowTitle(tr("Import Configuration"));
            messageBox->setIcon(QMessageBox::Question);

            auto yesButton = messageBox->addButton(tr("Append"), QMessageBox::YesRole);
            auto noButton = messageBox->addButton(tr("Overwrite"), QMessageBox::NoRole);
            auto cancelButton = messageBox->addButton(tr("Cancel"), QMessageBox::RejectRole);

            messageBox->exec();

            if (messageBox->clickedButton()==yesButton) {
                append = true;
            } else if (messageBox->clickedButton()==noButton) {
                append = false;
            } else if (messageBox->clickedButton()==cancelButton) {
                delete messageBox;

                return;
            } else {
                delete messageBox;

                return;
            }

            if (hostMasker->loadFromFile(filename, append)) {
                m_loadingConfiguration = true;

                ui->expressionsTreeWidget->clear();

                for (auto masker : hostMasker->m_maskList) {
                    addExpression(masker);
                }

                m_loadingConfiguration = false;
            }

            ui->exportButton->setEnabled(hostMasker->m_maskList.count() ? true : false);

            delete messageBox;
        };
    });

    connect(ui->exportButton, &QPushButton::clicked, [=](bool checked) {
        auto filename = QFileDialog::getSaveFileName(Nedrysoft::Core::mainWindow(), tr("Export Configuration"));

        if (!filename.isEmpty()) {
            hostMasker->saveToFile(filename);
        }
    });

    auto headerLabels = QStringList() <<
        tr("Description") <<
        tr("Enabled") <<
        tr("Match\r\nHop") <<
        tr("Match\r\nAddress") <<
        tr("Match\r\nHost") <<
        tr("Mask\r\nAddress") <<
        tr("Mask\r\nHost");

    ui->expressionsTreeWidget->header()->setDefaultAlignment(Qt::AlignHCenter);
    ui->expressionsTreeWidget->setHeaderLabels(headerLabels);

    QFontMetrics fontMetrics(ui->expressionsTreeWidget->header()->font());

    for (auto masker : hostMasker->m_maskList) {
        addExpression(masker);
    }

    ui->exportButton->setEnabled(hostMasker->m_maskList.count() ? true : false);

    // resize the checkbox columns and resize the first column to be the remaining space

    ui->expressionsTreeWidget->header()->setStretchLastSection(false);

    auto resizedColumnsWidth = 0;

    for (int column = 0; column<headerLabels.count(); column++) {
        auto columnWidth = fontMetrics.boundingRect(headerLabels.at(column)).width();

        ui->expressionsTreeWidget->setColumnWidth(column, columnWidth);

        resizedColumnsWidth += columnWidth-4;
    }

    auto adjustment = rect().width()-defaultWidgetWidth;

    ui->expressionsTreeWidget->setColumnWidth(0, defaultWidgetWidth-resizedColumnsWidth-adjustment);

    connect(ui->expressionsTreeWidget, &QTreeWidget::currentItemChanged, [=](QTreeWidgetItem *current, QTreeWidgetItem *previous) {
        updateWidgets(current);
    });

    ui->expressionsTreeWidget->setCurrentItem(ui->expressionsTreeWidget->topLevelItem(0));

    // connect the signals for editing

    connect(ui->enabledCheckBox, &QCheckBox::stateChanged, [=](int state) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        maskerItem.m_enabled = state;

        updateCheckBox(ui->expressionsTreeWidget->currentItem(), 1, state);

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->matchHopsCheckbox, &QCheckBox::stateChanged, [=](int state) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        if (state) {
            maskerItem.m_matchFlags |= static_cast<int>(RegExHostMasker::MatchFlags::MatchHop);
        } else {
            maskerItem.m_matchFlags &= ~static_cast<int>(RegExHostMasker::MatchFlags::MatchHop);
        }

        updateCheckBox(ui->expressionsTreeWidget->currentItem(), 2, state);

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->matchAddressCheckbox, &QCheckBox::stateChanged, [=](int state) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        if (state) {
            maskerItem.m_matchFlags |= static_cast<int>(RegExHostMasker::MatchFlags::MatchAddress);
        } else {
            maskerItem.m_matchFlags &= ~static_cast<int>(RegExHostMasker::MatchFlags::MatchAddress);
        }

        updateCheckBox(ui->expressionsTreeWidget->currentItem(), 3, state);

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->matchHostCheckbox, &QCheckBox::stateChanged, [=](int state) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        if (state) {
            maskerItem.m_matchFlags |= static_cast<int>(RegExHostMasker::MatchFlags::MatchHost);
        } else {
            maskerItem.m_matchFlags &= ~static_cast<int>(RegExHostMasker::MatchFlags::MatchHost);
        }

        updateCheckBox(ui->expressionsTreeWidget->currentItem(), 4, state);

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->maskAddressCheckbox, &QCheckBox::stateChanged, [=](int state) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        if (state) {
            maskerItem.m_matchFlags |= static_cast<int>(RegExHostMasker::MatchFlags::MaskAddress);
        } else {
            maskerItem.m_matchFlags &= ~static_cast<int>(RegExHostMasker::MatchFlags::MaskAddress);
        }

        updateCheckBox(ui->expressionsTreeWidget->currentItem(), 5, state);

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->maskHostCheckbox, &QCheckBox::stateChanged, [=](int state) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        if (state) {
            maskerItem.m_matchFlags |= static_cast<int>(RegExHostMasker::MatchFlags::MaskHost);
        } else {
            maskerItem.m_matchFlags &= ~static_cast<int>(RegExHostMasker::MatchFlags::MaskHost);
        }

        updateCheckBox(ui->expressionsTreeWidget->currentItem(), 6, state);

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->regularExpressionLineEdit, &QLineEdit::textChanged, [=](const QString &newString) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        maskerItem.m_matchExpression = newString;

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->substitutionLineEdit, &QLineEdit::textChanged, [=](const QString &newString) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        maskerItem.m_replacementString = newString;

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->addressSubstitutionLineEdit, &QLineEdit::textChanged, [=](const QString &newString) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        maskerItem.m_addressReplacementString = newString;

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->descriptionLineEdit, &QLineEdit::textChanged, [=](const QString &newString) {
        auto maskerItem = getMaskerItem(ui->expressionsTreeWidget->currentItem());

        maskerItem.m_description = newString;

        if (ui->expressionsTreeWidget->currentItem()) {
            ui->expressionsTreeWidget->currentItem()->setText(0, newString);
        }

        setMaskerItem(ui->expressionsTreeWidget->currentItem(), maskerItem);

        updateSettings();
    });

    connect(ui->deleteButton, &QPushButton::clicked, [=](bool checked) {
        delete ui->expressionsTreeWidget->currentItem();

        updateSettings();
    });

    connect(ui->newButton, &QPushButton::clicked, [=](bool checked) {
        Nedrysoft::RegExHostMasker::RegExHostMaskerItem masker;

        masker.m_enabled = true;
        masker.m_matchFlags = 0;
        masker.m_replacementString = "";
        masker.m_matchExpression = "";
        masker.m_description = tr("New Masker");
        masker.m_hopString = "";
        masker.m_addressReplacementString = "";
        masker.m_hops = QSet<int>();

        addExpression(masker);

        updateSettings();
    });

    m_loadingConfiguration = false;
}

Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::~RegExHostMaskerSettingsPageWidget() {
    delete ui;
}

QSize Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::sizeHint() const {
    QSize size;

    size = QWidget::sizeHint()+QSize(0, toolbarAdjustment);

    size.setWidth(qMax(size.width(), defaultWidgetWidth));

    return size;
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::updateWidgets(QTreeWidgetItem *item) -> void {
    ui->descriptionLineEdit->setEnabled(item!=nullptr);
    ui->regularExpressionLineEdit->setEnabled(item!=nullptr);
    ui->substitutionLineEdit->setEnabled(item!=nullptr);
    ui->addressSubstitutionLineEdit->setEnabled(item!=nullptr);
    ui->matchHopsCheckbox->setEnabled(item!=nullptr);
    ui->maskHostCheckbox->setEnabled(item!=nullptr);
    ui->maskAddressCheckbox->setEnabled(item!=nullptr);
    ui->matchAddressCheckbox->setEnabled(item!=nullptr);
    ui->matchHostCheckbox->setEnabled(item!=nullptr);
    ui->enabledCheckBox->setEnabled(item!=nullptr);
    ui->deleteButton->setEnabled(item!=nullptr);

    if (!item) {
        ui->descriptionLineEdit->setText("");
        ui->regularExpressionLineEdit->setText("");
        ui->addressSubstitutionLineEdit->setText("");
        ui->substitutionLineEdit->setText("");
        ui->matchHopsCheckbox->setChecked(false);
        ui->maskHostCheckbox->setChecked(false);
        ui->maskAddressCheckbox->setChecked(false);
        ui->matchAddressCheckbox->setChecked(false);
        ui->matchHostCheckbox->setChecked(false);

        ui->enabledCheckBox->setChecked(false);

        return;
    }

    Nedrysoft::RegExHostMasker::RegExHostMaskerItem regexItem =
            item->data(0, Qt::UserRole).value<Nedrysoft::RegExHostMasker::RegExHostMaskerItem>();

    ui->descriptionLineEdit->setText(regexItem.m_description);
    ui->regularExpressionLineEdit->setText(regexItem.m_matchExpression);
    ui->substitutionLineEdit->setText(regexItem.m_replacementString);
    ui->addressSubstitutionLineEdit->setText(regexItem.m_addressReplacementString);

    int flags = static_cast<int>(regexItem.m_matchFlags);

    ui->matchHopsCheckbox->setChecked((flags & static_cast<int>(RegExHostMasker::MatchFlags::MatchHop)) != 0);
    ui->maskHostCheckbox->setChecked((flags & static_cast<int>(RegExHostMasker::MatchFlags::MaskHost)) != 0);
    ui->maskAddressCheckbox->setChecked((flags & static_cast<int>(RegExHostMasker::MatchFlags::MaskAddress)) != 0);
    ui->matchAddressCheckbox->setChecked((flags & static_cast<int>(RegExHostMasker::MatchFlags::MatchAddress)) != 0);
    ui->matchHostCheckbox->setChecked((flags & static_cast<int>(RegExHostMasker::MatchFlags::MatchHost)) != 0);

    ui->enabledCheckBox->setChecked(regexItem.m_enabled);
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::updateSettings() -> void {
    auto hostMasker = Nedrysoft::ComponentSystem::getObject<RegExHostMasker>();

    assert(hostMasker!=nullptr);

    ui->exportButton->setEnabled(hostMasker->m_maskList.count() ? true : false);

#if !defined(Q_OS_MACOS)
    return;
#endif
    if (!m_loadingConfiguration) {
        acceptSettings();
    }
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::canAcceptSettings() -> bool {
#if defined(Q_OS_MACOS)
    return true;
#endif
    //TODO: this needs to check the settings configured in the dialog to ensure they are valid
    return true;
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::acceptSettings() -> void {
    auto hostMasker = Nedrysoft::ComponentSystem::getObject<RegExHostMasker>();

    assert(hostMasker!=nullptr);

    QFontMetrics fontMetrics(ui->expressionsTreeWidget->header()->font());

    QList<Nedrysoft::RegExHostMasker::RegExHostMaskerItem> itemList;

    for(auto currentIndex=0; currentIndex<ui->expressionsTreeWidget->topLevelItemCount(); currentIndex++) {
        auto currentItem = ui->expressionsTreeWidget->topLevelItem(currentIndex);

        itemList.append(currentItem->data(0, Qt::UserRole).value<Nedrysoft::RegExHostMasker::RegExHostMaskerItem>());
    }

    hostMasker->m_maskList = itemList;

    hostMasker->saveToFile();
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::addExpression(
        Nedrysoft::RegExHostMasker::RegExHostMaskerItem masker) -> void {

    auto treeItem = new QTreeWidgetItem;

    RegExHostMasker::MatchFlags matchFlags[] = {
            RegExHostMasker::MatchFlags::MatchHop,
            RegExHostMasker::MatchFlags::MatchAddress,
            RegExHostMasker::MatchFlags::MatchHost,
            RegExHostMasker::MatchFlags::MaskAddress,
            RegExHostMasker::MatchFlags::MaskHost
    };

    QCheckBox *checkBoxes[] = {
            ui->matchHopsCheckbox,
            ui->matchAddressCheckbox,
            ui->matchHostCheckbox,
            ui->maskAddressCheckbox,
            ui->maskHostCheckbox
    };

    treeItem->setText(0, masker.m_description);
    treeItem->setData(0, Qt::UserRole, QVariant::fromValue(masker));

    ui->expressionsTreeWidget->addTopLevelItem(treeItem);

    // create enabled column

    auto enabledCheckBox = addCheckBox(masker.m_enabled);

    ui->expressionsTreeWidget->setItemWidget(treeItem, 1, enabledCheckBox->parentWidget());

    connect(enabledCheckBox, &QCheckBox::stateChanged, [=](int state) {
        auto itemData = treeItem->data(0, Qt::UserRole).value<Nedrysoft::RegExHostMasker::RegExHostMaskerItem>();

        itemData.m_enabled = state;

        treeItem->setData(0, Qt::UserRole, QVariant::fromValue(itemData));

        // block signals, we don't want to end up in a recursive loop.

        ui->enabledCheckBox->blockSignals(true);
        ui->enabledCheckBox->setChecked(state);
        ui->enabledCheckBox->blockSignals(false);

        updateSettings();
    });

    // bit of a hack, can't centre the checkbox directly, so stick it in a widget with a centered layout.

    for (int column = 2; column<ui->expressionsTreeWidget->header()->count(); column++) {
        auto checkBox = addCheckBox(masker.m_matchFlags & static_cast<int>(matchFlags[column-2]));

        ui->expressionsTreeWidget->setItemWidget(treeItem, column, checkBox->parentWidget());

        treeItem->setData(column, Qt::UserRole+1, static_cast<int>(matchFlags[column-2]));

        connect(checkBox, &QCheckBox::stateChanged, [=](int state) {
            auto itemData = treeItem->data(0, Qt::UserRole).value<Nedrysoft::RegExHostMasker::RegExHostMaskerItem>();
            auto field = treeItem->data(column, Qt::UserRole+1).toInt();

            if (state) {
                itemData.m_matchFlags |= field;
            } else {
                itemData.m_matchFlags &= ~field;
            }

            treeItem->setData(0, Qt::UserRole, QVariant::fromValue(itemData));

            // block signals, we don't want to end up in a recursive loop.

            checkBoxes[column-2]->blockSignals(true);
            checkBoxes[column-2]->setChecked(state);
            checkBoxes[column-2]->blockSignals(false);

            updateSettings();
        });
    }

    ui->expressionsTreeWidget->setCurrentItem(treeItem);
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::addCheckBox(
        bool isChecked ) -> QCheckBox * {

    auto containerWidget = new QWidget;
    auto checkBox = new QCheckBox(containerWidget);
    auto containerLayout = new QHBoxLayout;

    checkBox->setCheckState(isChecked ? Qt::Checked : Qt::Unchecked);

    containerLayout->setAlignment(Qt::AlignCenter);
    containerLayout->setContentsMargins(1, 1, 1, 1);

    containerWidget->setLayout(containerLayout);

    containerLayout->addWidget(checkBox);

    return checkBox;
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::getCheckBox(QWidget *container) -> QCheckBox * {
    auto layout = qobject_cast<QHBoxLayout *>(container->layout());

    if (!layout) {
        return nullptr;
    }

    auto layoutItem = layout->itemAt(0);

    if (!layoutItem) {
        return nullptr;
    }

    return qobject_cast<QCheckBox *>(layoutItem->widget());
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::updateCheckBox(
        QTreeWidgetItem *item,
        int column,
        bool isChecked) -> void {

    if (!item) {
        return;
    }

    auto containerWidget = ui->expressionsTreeWidget->itemWidget(item, column);

    if (!containerWidget) {
        return;
    }

    auto checkBox = getCheckBox(containerWidget);

    if (checkBox) {
        checkBox->setChecked(isChecked ? Qt::Checked : Qt::Unchecked);
    }
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::getMaskerItem(QTreeWidgetItem * /*item*/) ->
        Nedrysoft::RegExHostMasker::RegExHostMaskerItem {

    if (!ui->expressionsTreeWidget->currentItem()) {
        RegExHostMaskerItem item;

        return item;
    }

    auto maskerItem = ui->expressionsTreeWidget->currentItem()->data(
            0, Qt::UserRole).value<Nedrysoft::RegExHostMasker::RegExHostMaskerItem>();

    return maskerItem;
}

auto Nedrysoft::RegExHostMasker::RegExHostMaskerSettingsPageWidget::setMaskerItem(
        QTreeWidgetItem *item,
        Nedrysoft::RegExHostMasker::RegExHostMaskerItem maskerItem) -> void {

    if (!item) {
        return;
    }

    item->setData(
            0,
            Qt::UserRole,
            QVariant::fromValue<Nedrysoft::RegExHostMasker::RegExHostMaskerItem>(maskerItem) );
}

