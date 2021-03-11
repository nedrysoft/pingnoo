/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 06/03/2021.
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

#ifndef NEDRYSOFT_TARGETMANAGER_H
#define NEDRYSOFT_TARGETMANAGER_H

#include "Core/ICore.h"

#include <QJsonObject>
#include <QObject>

namespace Nedrysoft::RouteAnalyser {
    enum class FavouriteField {
        Host,
        Name,
        Description,
        IPVersion
    };

    /**
     * @brief       The favourites manager controls the manipulation of favourites and storage.
     */
    class TargetManager :
            public QObject {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new TargetManager.
             */
            TargetManager();

            /**
             * @brief       Gets the singleton instance of the favourites manager.
             *
             * @returns     a pointer to the instance.
             */
            static auto getInstance() -> Nedrysoft::RouteAnalyser::TargetManager *;

            /**
             * @brief       Adds a favourite to the favourites list.
             *
             * @note        if it already exists, then it is moved automatically to the top so it behaves
             *              like recent files.
             *
             * @param[in]   host the host target.
             * @param[in]   name the user friendly name assigned to this facourite.
             * @param[in]   description the descriptive name of the favourite.
             * @param[in]   ipVersion the IP version to use for this favourite.
             */
            auto addFavourite(
                    QString host,
                    QString name,
                    QString description,
                    Nedrysoft::Core::IPVersion ipVersion ) -> void;
            /**
             * @brief       Adds a target to recent list.
             *
             * @note        if it already exists, then it is moved automatically to the top so it behaves
             *              like recent files.
             *
             * @param[in]   host the host target.
             * @param[in]   name the user friendly name assigned to this facourite.
             * @param[in]   description the descriptive name of the favourite.
             * @param[in]   ipVersion the IP version to use for this favourite.
             */
            auto addRecent(
                    QString host,
                    QString name,
                    QString description,
                    Nedrysoft::Core::IPVersion ipVersion ) -> void;

            /**
             * @brief       Adds a target to recent list.
             *
             * @param[in]   parameters the key/value map of a target.
             *
             */
            auto addRecent(QVariantMap parameters) -> void;

            /**
             * @brief       Returns the list of favourites as a map
             *
             * @returns     the list of favourites.
             */
            auto favourites() -> QList<QVariantMap>;

            /**
             * @brief       Sets the favourites to the given list and saves to storage.
             *
             * @param[in]   favourites the list of favourites.
             */
            auto setFavourites(QList<QVariantMap> favourites) ->void;

            /**
             * @brief       Returns the list of recent targets
             *
             * @returns     the list of favourites.
             */
            auto recents() -> QList<QVariantMap>;

            /**
             * @brief       Saves the favourites managers state.
             *
             * @returns     the favourites as a JSON object.
             */
            auto saveConfiguration() -> QJsonObject;

            /**
             * @brief       Loads the favourites managers state.
             *
             * @param[in]   configuration is the JSON object to load.
             *
             * @returns     true if the configuration was loaded; otherwise false.
             */
            auto loadConfiguration(QJsonObject configuration) -> bool;

            /**
             * @brief       Saves the current favourites configuration to disk.
             *
             * @param[ib]   filename is the name of the file to be saved, if empty then the components data
             *              storage location is used.
             *
             * @return      returns true if saved; otherwise false.
             */
            auto saveFavourites(QString filename = QString()) -> bool;

            /**
             * @brief       Loads the current favourites configuration file.
             *
             * @param[in]   filename is the name of the file to be loaded, if empty then the components data
             *              storage location is used.
             *
             * @param[in]   append true to append to the list; otherwise overwrite.
             *
             * @return      returns true if loaded; otherwise false.
             */
            auto loadFavourites(QString filename = QString(), bool append = false) -> bool;

            /**
             * @brief       Imports the favourites prompting for a filename.
             *
             * @param[in]   parent the parent window of the dialog.
             */
            auto importFavourites(QWidget *parent=nullptr) -> void;

            /**
             * @brief       Exports the favourites prompting for a filename.
             *
             * @param[in]   parent the parent window of the dialog.
             */
            auto exportFavourites(QWidget *parent=nullptr) -> void;

            /**
             * @brief       This signal is emitted when the favourites "database" has changed.
             */
            Q_SIGNAL void favouritesChanged();

            /**
             * @brief       This signal is emitted when the most recently used target list has changed.
             */
            Q_SIGNAL void recentsChanged();

        private:
            QList<QVariantMap> m_favouriteList;
            QList<QVariantMap> m_recentsList;
    };
}

#endif //NEDRYSOFT_TARGETMANAGER_H
