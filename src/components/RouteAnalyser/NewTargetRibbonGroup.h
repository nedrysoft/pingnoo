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

#ifndef NEDRYSOFT_NEWTARGETRIBBONGROUP_H
#define NEDRYSOFT_NEWTARGETRIBBONGROUP_H

#include "LineSyntaxHighlighter.h"

#include <QWidget>

class QMenu;

namespace Nedrysoft::Ribbon {
    class RibbonLineEdit;
}

namespace Nedrysoft::Utils {
    class ThemeSupport;
}

namespace Nedrysoft::Core {
    class IPingEngineFactory;
}

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class NewTargetRibbonGroup;
    }

    /**
     * @brief       The NewTargetRibbonGroup is a Ribbon group that allows a new target to be created.
     */
    class NewTargetRibbonGroup :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new NewTargetRibbonGroup instance which is a child of the parent.
             *
             * @param[in]   parent the parent for this widget.
             */
            explicit NewTargetRibbonGroup(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the NewTargetRibbonGroup.
             */
            ~NewTargetRibbonGroup() override;

            /**
             * @brief       Called when light/dark mode changes
             *
             * @param[in]   isDarkMode whether dark mode is active.
             */
            Q_SLOT void updateIcons(bool isDarkMode);

        private:
            /**
             * @brief       Validate the fields of the dialog.
             *
             * @param[out]  string the error string if validation failed.
             *
             * @returns     nullptr on no error; otherwise the first widget that failed validation.
             */
            auto checkFieldsValid(QString &string) -> QWidget *;

            /**
             * @brief       Validates the fields of the dialog and enables/disables the ok/apply buttons.
             */
            auto validateFields() -> void ;

        private:
            /**
             * @brief       Finds the build path in the map using case insensitivity.
             *
             * @param[in]   buildPath the menu path (i.e /gennaro/malcom/nedry)
             * @param[in]   itemText is the name of the item we are looking to insert.
             * @param[in]   menuMap the map of buildpath -> menus
             *
             * @returns     the action before our insertion or nullptr to append to end.
             */
            auto findInsertAction(QString buildPath, QString itemText, QMap<QString, QMenu *> &menuMap) -> QAction *;

            /**
             * @brief       Adds an action to the favourites menu ensuring it appears in order.
             *
             * @param[in]   action the action to insert.
             * @param[in]   buildPath the menu path (i.e /gennaro/malcom/nedry)
             * @param[in]   itemText is the name of the item we are looking to insert.
             * @param[in]   menuMap the map of buildpath -> menus
             */
            auto addFavouriteAction(
                    QAction *action,
                    QString buildPath,
                    QString itemText,
                    QMap<QString,
                    QMenu *> &menuMap) -> void;

            /**
             * @brief       Imports favourites prompting for a file and whether to append or overrwrite.
             *
             * @param[in]   checked if the button was checked.
             */
            Q_SLOT void onImportFavourites(bool checked);

            /**
             * @brief       Exports favourites prompting for a file.
             *
             * @param[in]   checked if the button was checked.
             */
            Q_SLOT void onExportFavourites(bool checked);

            /**
             * @brief       Opens and begins analysis to a host.
             *
             * @param[in]   parameters the ping target parameters
             * @param[in]   pingEngine the ping engine factory to use.
             */
            Q_SLOT void openTarget(QVariantMap parameters, Nedrysoft::Core::IPingEngineFactory *pingEngine);

            /**
             * @brief       Populates the recents menu with the most recently used targets.
             */
            auto populateRecentsMenu() -> void;

            /**
             * @brief       Populates the favourites menu.
             */
            auto populateFavouritesMenu() -> void;

            /**
             * @brief       Opens the favourites manager.
             *
             * @param[in]   checked whether the action was checked.
             */
            Q_SLOT void onEditFavourites(bool checked);

            /**
             * @brief       Opens the favourite editor .
             *
             * @param[in]   checked whether the action was checked.
             */
            Q_SLOT void onNewFavourite(bool checked);

            /**
             * @brief       Opens the favourite selecton dialog.
             *
             * @returns     The favourite data map.
             */
            Q_SLOT QVariantMap onOpenFavourite(bool checked);

        private:
            Ui::NewTargetRibbonGroup *ui;

            LineSyntaxHighlighter *m_targetHighlighter;
            LineSyntaxHighlighter *m_intervalHighlighter;

            Nedrysoft::Utils::ThemeSupport *m_themeSupport;
            QMenu *m_recentsMenu;
            QMap<QString, QMenu *> m_favouritesMenuMap;

            QAction *m_recentTargetsAction;
            QAction * m_favouritesAction;
            QAction * m_openFavouriteAction;
            QAction * m_saveFavouriteAction;
            QAction * m_newFavouriteAction;
            QAction * m_editFavouritesAction;
            QAction * m_importFavouritesAction;
            QAction * m_exportFavouritesAction;
    };
}

#endif //NEDRYSOFT_NEWTARGETRIBBONGROUP_H
