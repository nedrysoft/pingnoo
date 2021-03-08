/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 10/12/2020.
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

// You may need to build the project (run Qt uic code generator) to get "ui_RouteAnalyserRibbonGroup.h" resolved

#include "NewTargetRibbonGroup.h"
#include "ui_NewTargetRibbonGroup.h"

#include "ComponentSystem/IComponentManager.h"
#include "Core/ICommandManager.h"
#include "Core/IEditorManager.h"
#include "Core/IPingEngineFactory.h"
#include "TargetManager.h"
#include "FavouritesManagerDialog.h"
#include "RouteAnalyserEditor.h"
#include "TargetSettings.h"
#include "ThemeSupport.h"
#include "Utils.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QMenu>
#include <QStandardItemModel>

constexpr auto comboPadding = 12;
constexpr auto defaultInterval = "2.5s";
constexpr auto defaultTarget = "1.1.1.1";
constexpr auto lineEditHeight = 21;

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::NewTargetRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::NewTargetRibbonGroup) {

    ui->setupUi(this);

    m_themeSupport = new Nedrysoft::Utils::ThemeSupport;

    updateIcons(m_themeSupport->isDarkMode());

    connect(m_themeSupport, &Nedrysoft::Utils::ThemeSupport::themeChanged, [=](bool isDarkMode) {
        updateIcons(isDarkMode);
    });

    auto targetSettings = Nedrysoft::ComponentSystem::getObject<Nedrysoft::RouteAnalyser::TargetSettings>();

    assert(targetSettings!=nullptr);

    connect(ui->favouriteDropButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, [=](bool dropdown) {
        if (!dropdown) {
            FavouritesManagerDialog editorManagerDialog(Nedrysoft::Core::mainWindow());

            editorManagerDialog.exec();

            return;
        }

        QMenu menu;
        QPoint menuPosition = ui->favouriteDropButton->rect().bottomLeft();

        auto recents = menu.addAction(tr("Recent Targets"));
        menu.addSeparator();
        auto favourites = menu.addAction(tr("Favourites"));
        menu.addSeparator();
        auto openFavourite = menu.addAction(tr("Open Favourite..."));
        auto saveFavourite = menu.addAction(tr("Save Favourite..."));
        menu.addSeparator();
        auto newFavourite = menu.addAction(tr("New Favourite..."));
        auto editFavourites = menu.addAction(tr("Edit Favourites..."));
        menu.addSeparator();
        auto importFavourites = menu.addAction(tr("Import Favourites..."));
        auto exportFavourites = menu.addAction(tr("Export Favourites..."));

        menuPosition = mapToGlobal(menuPosition);

        auto recentTargets = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->recents();

        QMenu recentsMenu;

        for (auto target : recentTargets) {
            recentsMenu.addAction(target["host"].toString());
        }

        recents->setMenu(&recentsMenu);
        recents->setDisabled(recentTargets.isEmpty());

        // ****
        // the favourites use / to allow structuring of them
        // ****

        auto favouritesList = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->favourites();

        QMap<QString, QMenu *> menuMap;

        menuMap["/"] = new QMenu;

        for (auto favourite : favouritesList) {
            auto menuPath = favourite["name"].toString().split("/");
            QString buildPath;

            if (menuPath.length()) {
                auto menuIndex = 0;

                buildPath = "/";

                while(menuIndex<menuPath.length()-1) {
                    auto thisSegment = menuPath.at(menuIndex).trimmed();

                    if (!thisSegment.isEmpty()) {
                        auto nextBuildPath = buildPath+thisSegment+"/";

                        if (!menuMap.contains(nextBuildPath)) {
                            menuMap[nextBuildPath] = new QMenu;

                            QAction *action = new QAction(menuPath.at(menuIndex));

                            action->setMenu(menuMap[nextBuildPath]);

                            menuMap[buildPath]->addAction(action);
                        }

                        buildPath += menuPath.at(menuIndex)+"/";
                    }

                    menuIndex++;
                }

                menuMap[buildPath]->addAction(new QAction(menuPath.at(menuIndex)));
            } else {
                menuMap["/"]->addAction(favourite["name"].toString());
            }
        }

        favourites->setMenu(menuMap["/"]);

        favourites->setDisabled(favouritesList.isEmpty());
        saveFavourite->setDisabled(true);
        exportFavourites->setDisabled(true);

        auto selectedAction = menu.exec(menuPosition);

        qDeleteAll(menuMap);

        if (selectedAction == openFavourite) {

        } else if (selectedAction == saveFavourite) {

        } else if (selectedAction == newFavourite) {

        } else if (selectedAction == editFavourites) {
            FavouritesManagerDialog favouritesDialog;

            favouritesDialog.exec();
        } else if (selectedAction == importFavourites) {

        } else if (selectedAction == exportFavourites) {

        }
    });

    m_intervalHighlighter = new LineSyntaxHighlighter(ui->intervalLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    m_targetHighlighter = new LineSyntaxHighlighter(ui->targetLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::checkHostValid(text);
    });

    ui->intervalLineEdit->setPlaceholderText(Nedrysoft::Utils::intervalToString(targetSettings->defaultPingInterval()));
    ui->targetLineEdit->setPlaceholderText(targetSettings->defaultHost());

    if (targetSettings->defaultIPVersion()==Nedrysoft::Core::IPVersion::V4) {
        ui->ipV4RadioButton->setChecked(true);
    } else {
        ui->ipV6RadioButton->setChecked(true);
    }

    auto pingEngines = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IPingEngineFactory>();

    if (pingEngines.count()) {
        auto minimumWidth = 0;

        QMultiMap<double, Nedrysoft::Core::IPingEngineFactory *> sortedPingEngines;

        for(auto pingEngine : pingEngines) {
            sortedPingEngines.insert(1-pingEngine->priority(), pingEngine);
        }

        for(auto pingEngine : sortedPingEngines) {
            ui->engineComboBox->addItem(
                    pingEngine->description(),
                    QVariant::fromValue<Nedrysoft::Core::IPingEngineFactory *>(pingEngine) );

            auto model = dynamic_cast<QStandardItemModel *>(ui->engineComboBox->model());

            model->item(model->rowCount()-1, 0)->setEnabled(pingEngine->available());
            model->item(model->rowCount()-1, 0)->setData(pingEngine->metaObject()->className(), Qt::UserRole+1);

            QFontMetrics fontMetrics(ui->engineComboBox->font());

            minimumWidth = qMax(minimumWidth, fontMetrics.boundingRect(pingEngine->description()).width()+comboPadding);
        }

        ui->engineComboBox->view()->setMinimumWidth(minimumWidth);

        auto selectionIndex = ui->engineComboBox->findData(targetSettings->defaultPingEngine(), Qt::UserRole+1);

        if (selectionIndex==-1) {
            if (sortedPingEngines.count()) {
                selectionIndex = ui->engineComboBox->findData(
                        sortedPingEngines.first()->metaObject()->className(),
                        Qt::UserRole + 1 );
            }
        }

        ui->engineComboBox->setCurrentIndex(selectionIndex);
    }

    connect(ui->startButton, &Nedrysoft::Ribbon::RibbonButton::clicked, [=](bool checked) {
        auto editorManager = Nedrysoft::Core::IEditorManager::getInstance();

        if (editorManager) {
            auto pingEngineFactory = ui->engineComboBox->currentData().value<Nedrysoft::Core::IPingEngineFactory *>();

            auto target = ui->targetLineEdit->toPlainText().isEmpty() ?
                    ui->targetLineEdit->placeholderText() :
                    ui->targetLineEdit->toPlainText();

            auto interval = ui->intervalLineEdit->toPlainText().isEmpty() ?
                    ui->intervalLineEdit->placeholderText() :
                    ui->intervalLineEdit->toPlainText();

            if (!Nedrysoft::Utils::checkHostValid(target)) {
                return;
            }

            double intervalTime;

            if (!Nedrysoft::Utils::parseIntervalString(interval, intervalTime)) {
                return;
            }

            RouteAnalyserEditor *editor = new RouteAnalyserEditor;

            editor->setPingEngine(pingEngineFactory);
            editor->setTarget(target);

            Nedrysoft::Core::IPVersion ipVersion;

            if (ui->ipV4RadioButton->isChecked()) {
                ipVersion = Nedrysoft::Core::IPVersion::V4;
            } else {
                ipVersion = Nedrysoft::Core::IPVersion::V6;
            }

            editor->setIPVersion(ipVersion);

            editor->setInterval(intervalTime);

            editorManager->openEditor(editor);

            auto favouritesManager = Nedrysoft::RouteAnalyser::TargetManager::getInstance();

            favouritesManager->addFavourite(target, target, target, ipVersion);
        }
    });

    connect(ui->intervalLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [this]() {
        ui->intervalLineEdit->blockSignals(true);
        validateFields();
        ui->intervalLineEdit->blockSignals(false);
    });

    connect(ui->targetLineEdit, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [this]() {
        ui->targetLineEdit->blockSignals(true);
        validateFields();
        ui->targetLineEdit->blockSignals(false);
    });

    validateFields();

    ui->targetLineEdit->setMaximumHeight(lineEditHeight);
    ui->intervalLineEdit->setMaximumHeight(lineEditHeight);
}

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::~NewTargetRibbonGroup() {
    delete ui;
}

void Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::updateIcons(bool isDarkMode) {
    QIcon icon;

    if (isDarkMode) {
        icon = QIcon(QString(":/RouteAnalyser/icons/2x/twotone_bookmarks_white_24dp.png"));
    } else {
        icon = QIcon(QString(":/RouteAnalyser/icons/2x/twotone_bookmarks_black_24dp.png"));
    }

    ui->favouriteDropButton->setIcon(icon);
}

auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::checkFieldsValid(QString &string) -> QWidget * {
    double intervalValue;
    QWidget *returnWidget = nullptr;

    auto target = ui->targetLineEdit->toPlainText().isEmpty() ?
                  ui->targetLineEdit->placeholderText() :
                  ui->targetLineEdit->toPlainText();

    auto interval = ui->intervalLineEdit->toPlainText().isEmpty() ?
                    ui->intervalLineEdit->placeholderText() :
                    ui->intervalLineEdit->toPlainText();

    if (!Nedrysoft::Utils::checkHostValid(target)) {
        returnWidget = ui->targetLineEdit;
    }

    if (!Nedrysoft::Utils::parseIntervalString(interval, intervalValue)) {
        if (!returnWidget) {
            returnWidget = ui->intervalLineEdit;
        }
    }

    m_intervalHighlighter->updateSyntax();
    m_targetHighlighter->updateSyntax();

    return returnWidget;
}

auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::validateFields() -> void {
    QString errorString;

    auto invalidWidget = checkFieldsValid(errorString);

    ui->startButton->setEnabled(invalidWidget==nullptr);
}
