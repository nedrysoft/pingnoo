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

#include "NewTargetRibbonGroup.h"

#include "FavouriteEditorDialog.h"
#include "FavouritesManagerDialog.h"
#include "IPingEngineFactory.h"
#include "OpenFavouriteDialog.h"
#include "RouteAnalyserEditor.h"
#include "TargetManager.h"
#include "TargetSettings.h" // TODO: what to do about this?  Separate library I guess...
#include "Utils.h"

#include "ui_NewTargetRibbonGroup.h"

#include <ICommandManager>
#include <IEditorManager>
#include <QAbstractItemView>
#include <QMenu>
#include <QStandardItemModel>
#include <ThemeSupport>
#include <cassert>
#include <map>

constexpr auto ComboPadding = 12;
constexpr auto LineEditHeight = 21;
constexpr auto DataRole = Qt::UserRole + 1;
constexpr auto MillisecondsPerSecond = 1000.0;

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::NewTargetRibbonGroup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::NewTargetRibbonGroup),
        m_recentsMenu(nullptr),
        m_recentTargetsAction(nullptr) {

    ui->setupUi(this);

    auto themeSupport = Nedrysoft::ThemeSupport::ThemeSupport::getInstance();

    updateIcons(themeSupport->isDarkMode());

    connect(themeSupport, &Nedrysoft::ThemeSupport::ThemeSupport::themeChanged, [=](bool isDarkMode) {
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

        m_recentTargetsAction = menu.addAction(tr("Recent Targets"));
        menu.addSeparator();
        m_favouritesAction = menu.addAction(tr("Favourites"));
        menu.addSeparator();
        m_openFavouriteAction = menu.addAction(tr("Open Favourite..."));
        m_saveFavouriteAction = menu.addAction(tr("Save Favourite..."));
        menu.addSeparator();
        m_newFavouriteAction = menu.addAction(tr("New Favourite..."));
        m_editFavouritesAction = menu.addAction(tr("Edit Favourites..."));
        menu.addSeparator();
        m_importFavouritesAction = menu.addAction(tr("Import Favourites..."));
        m_exportFavouritesAction = menu.addAction(tr("Export Favourites..."));

        menuPosition = mapToGlobal(menuPosition);

        connect(m_importFavouritesAction,
                &QAction::triggered,
                this,
                &Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onImportFavourites);

        connect(m_exportFavouritesAction,
                &QAction::triggered,
                this,
                &Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onExportFavourites);

        connect(m_editFavouritesAction,
                &QAction::triggered,
                this,
                &Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onEditFavourites);

        connect(m_newFavouriteAction,
                &QAction::triggered,
                this,
                &Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onNewFavourite);

        connect(m_openFavouriteAction,
                &QAction::triggered,
                this,
                &Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onOpenFavourite);

        populateRecentsMenu();

        populateFavouritesMenu();

        auto favouritesList = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->favourites();

        m_saveFavouriteAction->setDisabled(true);

        m_exportFavouritesAction->setDisabled(favouritesList.isEmpty());

        menu.exec(menuPosition);

        qDeleteAll(m_favouritesMenuMap);

        m_favouritesMenuMap.clear();
    });

    m_intervalHighlighter = new LineSyntaxHighlighter(ui->intervalLineEdit->document(), [=](const QString &text) {
        return Nedrysoft::Utils::parseIntervalString(text);
    });

    m_targetHighlighter = new LineSyntaxHighlighter(ui->targetLineEdit->document(), [=](const QString &text) {
        QAbstractSocket::NetworkLayerProtocol protocol;

        auto returnValue = Nedrysoft::Utils::checkHostValid(text, &protocol);

        if (returnValue) {
            switch(protocol) {
                case QAbstractSocket::IPv4Protocol: {
                    return ui->ipV4RadioButton->isChecked();
                }

                case QAbstractSocket::IPv6Protocol: {
                    return ui->ipV6RadioButton->isChecked();
                }

                default: {
                    break;
                }
            }
        }

        return returnValue;
    });

    ui->intervalLineEdit->setPlaceholderText(Nedrysoft::Utils::intervalToString(targetSettings->defaultPingInterval()));
    ui->targetLineEdit->setPlaceholderText(targetSettings->defaultHost());

    if (targetSettings->defaultIPVersion()==Nedrysoft::Core::IPVersion::V4) {
        ui->ipV4RadioButton->setChecked(true);
    } else {
        ui->ipV6RadioButton->setChecked(true);
    }

    connect(ui->ipV4RadioButton, &QRadioButton::toggled, [=](bool checked) {
        m_targetHighlighter->rehighlight();
        updateStartState();
    });

    connect(ui->ipV6RadioButton, &QRadioButton::toggled, [=](bool checked) {
        m_targetHighlighter->rehighlight();
        updateStartState();
    });

    auto pingEngines = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::RouteAnalyser::IPingEngineFactory>();

    if (pingEngines.count()) {
        auto minimumWidth = 0;

        QMultiMap<double, Nedrysoft::RouteAnalyser::IPingEngineFactory *> sortedPingEngines;

        for(auto pingEngine : pingEngines) {
            sortedPingEngines.insert(1-pingEngine->priority(), pingEngine);
        }

        for(auto pingEngine : sortedPingEngines) {
            ui->engineComboBox->addItem(
                    pingEngine->description(),
                    QVariant::fromValue<Nedrysoft::RouteAnalyser::IPingEngineFactory *>(pingEngine) );

            auto model = dynamic_cast<QStandardItemModel *>(ui->engineComboBox->model());

            model->item(model->rowCount()-1, 0)->setEnabled(pingEngine->available());
            model->item(model->rowCount()-1, 0)->setData(pingEngine->metaObject()->className(), DataRole);

            QFontMetrics fontMetrics(ui->engineComboBox->font());

            minimumWidth = qMax(minimumWidth, fontMetrics.boundingRect(pingEngine->description()).width()+ComboPadding);
        }

        ui->engineComboBox->view()->setMinimumWidth(minimumWidth);

        auto selectionIndex = ui->engineComboBox->findData(targetSettings->defaultPingEngine(), DataRole);

        if (selectionIndex==-1) {
            if (sortedPingEngines.count()) {
                selectionIndex = ui->engineComboBox->findData(
                        sortedPingEngines.first()->metaObject()->className(),
                        DataRole );
            }
        }

        ui->engineComboBox->setCurrentIndex(selectionIndex);
    }

    connect(ui->startButton, &Nedrysoft::Ribbon::RibbonButton::clicked, [=](bool checked) {
        QVariantMap map;

        auto pingEngineFactory =
                ui->engineComboBox->currentData().value<Nedrysoft::RouteAnalyser::IPingEngineFactory *>();

        auto target = ui->targetLineEdit->toPlainText().isEmpty() ?
                      ui->targetLineEdit->placeholderText() :
                      ui->targetLineEdit->toPlainText();

        auto interval = ui->intervalLineEdit->toPlainText().isEmpty() ?
                        ui->intervalLineEdit->placeholderText() :
                        ui->intervalLineEdit->toPlainText();

        if (!Nedrysoft::Utils::checkHostValid(target, nullptr)) {
            return;
        }

        double intervalTime;

        if (!Nedrysoft::Utils::parseIntervalString(interval, intervalTime)) {
            return;
        }

        Nedrysoft::Core::IPVersion ipVersion;

        if (ui->ipV4RadioButton->isChecked()) {
            ipVersion = Nedrysoft::Core::IPVersion::V4;
        } else {
            ipVersion = Nedrysoft::Core::IPVersion::V6;
        }

        map["host"] = target;
        map["interval"] = intervalTime;
#if (QT_VERSION_MAJOR>=6)
        map["ipversion"] = QVariant::fromValue(ipVersion);
#else
        map["ipversion"].setValue<Nedrysoft::Core::IPVersion>(ipVersion);
#endif
        openTarget(map, pingEngineFactory);
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

    ui->targetLineEdit->setMaximumHeight(LineEditHeight);
    ui->intervalLineEdit->setMaximumHeight(LineEditHeight);

    ui->targetLineEdit->setTabChangesFocus(true);
    ui->intervalLineEdit->setTabChangesFocus(true);
}

Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::~NewTargetRibbonGroup() {
    delete ui;

    qDeleteAll(m_favouritesMenuMap);

    m_favouritesMenuMap.clear();
}

auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::updateStartState() -> void {
    QAbstractSocket::NetworkLayerProtocol protocol;

    auto isValid = Nedrysoft::Utils::checkHostValid(ui->targetLineEdit->toPlainText(), &protocol);

    if (isValid) {
        switch(protocol) {
            case QAbstractSocket::IPv4Protocol: {
                isValid = ui->ipV4RadioButton->isChecked();
                break;
            }

            case QAbstractSocket::IPv6Protocol: {
                isValid = ui->ipV6RadioButton->isChecked();
                break;
            }

            default: {
                break;
            }
        }
    }

    ui->startButton->setEnabled(isValid);
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

    QAbstractSocket::NetworkLayerProtocol protocol;

    if (Nedrysoft::Utils::checkHostValid(target, &protocol)) {
        switch(protocol) {
            case QAbstractSocket::IPv4Protocol: {
                if (!ui->ipV4RadioButton->isChecked()) {
                    returnWidget = ui->targetLineEdit;
                }

                break;
            }

            case QAbstractSocket::IPv6Protocol: {
                if (!ui->ipV6RadioButton->isChecked()) {
                    returnWidget = ui->targetLineEdit;
                }

                break;
            }

            default: {
                break;
            }
        }
    } else {
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

auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::findInsertAction(
        QString buildPath,
        QString itemText,
        QMap<QString,
        QMenu *> &menuMap) -> QAction * {

    QAction *baseAction = nullptr;

    for (auto action : menuMap[buildPath.toLower()]->actions()) {
        if (action->text().compare(itemText, Qt::CaseInsensitive)<0) {
            continue;
        }

        baseAction = action;

        break;
    }

    return baseAction;
}

auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::addFavouriteAction(
        QAction *action,
        QString buildPath,
        QString itemText,
        QMap<QString,
        QMenu *> &menuMap) -> void {

    auto beforeAction = findInsertAction(buildPath, itemText.toLower(), menuMap);

    if (beforeAction) {
        menuMap[buildPath]->insertAction(beforeAction, action);
    } else {
        menuMap[buildPath]->addAction(action);
    }
}

void Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onImportFavourites(bool checked) {
    Q_UNUSED(checked)

    auto targetManager = Nedrysoft::RouteAnalyser::TargetManager::getInstance();

    targetManager->importFavourites(this);
}

void Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onExportFavourites(bool checked) {
    Q_UNUSED(checked)

    auto targetManager = Nedrysoft::RouteAnalyser::TargetManager::getInstance();

    targetManager->exportFavourites(this);
}

void Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::openTarget(
        QVariantMap parameters,
        Nedrysoft::RouteAnalyser::IPingEngineFactory *pingEngineFactory ) {

    auto editorManager = Nedrysoft::Core::IEditorManager::getInstance();

    if (editorManager) {
        auto target = parameters["host"].toString() ;
        auto intervalTime = parameters["interval"].toDouble();
        auto ipVersion = parameters["ipversion"].value<Nedrysoft::Core::IPVersion>();

        RouteAnalyserEditor *editor = new RouteAnalyserEditor;

        editor->setPingEngine(pingEngineFactory);
        editor->setTarget(target);
        editor->setIPVersion(ipVersion);
        editor->setInterval(intervalTime);

        editorManager->openEditor(editor);
    }
}

auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::populateRecentsMenu() -> void {
    auto recentTargets = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->recents();

    m_recentsMenu = new QMenu;

    for (auto target : recentTargets) {
        m_recentsMenu->addAction(target["host"].toString());
    }

    m_recentTargetsAction->setMenu(m_recentsMenu);
    m_recentTargetsAction->setDisabled(recentTargets.isEmpty());
}

void Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onEditFavourites(bool checked) {
    Q_UNUSED(checked)

    FavouritesManagerDialog favouritesDialog;

    favouritesDialog.exec();
}

void Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onNewFavourite(bool checked) {
    Q_UNUSED(checked)
    auto targetManager = Nedrysoft::RouteAnalyser::TargetManager::getInstance();
    QVariantMap newItemMap;

    FavouriteEditorDialog favouriteEditorDialog(tr("New Favourite"), newItemMap, this);

    if (favouriteEditorDialog.exec()) {
        auto favouritesList = targetManager->favourites();

        favouritesList.append(favouriteEditorDialog.map());

        targetManager->setFavourites(favouritesList);
    }
}

QVariantMap Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::onOpenFavourite(bool checked) {
    Q_UNUSED(checked)

    OpenFavouriteDialog openFavouriteDialog(Nedrysoft::Core::mainWindow());

    if (openFavouriteDialog.exec()) {
        auto favouriteMap = openFavouriteDialog.selectedItem();

        if (!favouriteMap.isEmpty()) {
            auto pingEngineFactory =
                    ui->engineComboBox->currentData().value<Nedrysoft::RouteAnalyser::IPingEngineFactory *>();

            if (pingEngineFactory) {
                favouriteMap["interval"] = favouriteMap["interval"].toDouble() / MillisecondsPerSecond;

                openTarget(favouriteMap, pingEngineFactory);
            }
        }
    }

    return QVariantMap();
}

auto Nedrysoft::RouteAnalyser::NewTargetRibbonGroup::populateFavouritesMenu() -> void {
    auto favouritesList = Nedrysoft::RouteAnalyser::TargetManager::getInstance()->favourites();

    if (!m_favouritesMenuMap.isEmpty()) {
        qDeleteAll(m_favouritesMenuMap);

        m_favouritesMenuMap.clear();
    }

    m_favouritesMenuMap["/"] = new QMenu;

    for (auto favourite : favouritesList) {
        QString buildPath = "/";
        QString entryName;
        auto menuPath = favourite["name"].toString().split("/");

        if (menuPath.length()) {
            auto menuIndex = 0;

            while(menuIndex<menuPath.length()-1) {
                auto thisSegment = menuPath.at(menuIndex).trimmed();

                if (!thisSegment.isEmpty()) {
                    auto nextBuildPath = (buildPath+thisSegment+"/").toLower();

                    if (!m_favouritesMenuMap.contains(nextBuildPath)) {
                        m_favouritesMenuMap[nextBuildPath] = new QMenu;

                        QAction *action = new QAction(menuPath.at(menuIndex));

                        action->setMenu(m_favouritesMenuMap[nextBuildPath]);

                        addFavouriteAction(action, buildPath, menuPath.at(menuIndex).toLower(), m_favouritesMenuMap);
                    }

                    buildPath += (menuPath.at(menuIndex)+"/").toLower();
                }

                menuIndex++;
            }

            entryName = menuPath.at(menuIndex);
        } else {
            entryName = favourite["name"].toString();
        }

        auto newAction = new QAction(entryName);

        connect(newAction, &QAction::triggered, [=](bool checked) {
            auto pingEngineFactory =
                    ui->engineComboBox->currentData().value<Nedrysoft::RouteAnalyser::IPingEngineFactory *>();

            if (pingEngineFactory) {
                QVariantMap map = newAction->data().toMap();

                map["interval"] = map["interval"].toDouble()/MillisecondsPerSecond;

                openTarget(map, pingEngineFactory);
            }
        });

        newAction->setData(favourite);

        addFavouriteAction(newAction, buildPath, entryName.toLower(), m_favouritesMenuMap);
    }

    m_favouritesAction->setMenu(m_favouritesMenuMap["/"]);
    m_favouritesAction->setDisabled(favouritesList.isEmpty());
}