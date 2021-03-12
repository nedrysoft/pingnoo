/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 12/03/2021.
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

#ifndef NEDRYSOFT_FAVOURITESSORTPROXYFILTERMODEL_H
#define NEDRYSOFT_FAVOURITESSORTPROXYFILTERMODEL_H

#include <QSortFilterProxyModel>

namespace Nedrysoft::RouteAnalyser {
    /**
     * @brief       Implements a proxy filter model using rapidfuzz for string matching
     */
    class FavouritesSortProxyFilterModel :
            public QSortFilterProxyModel {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Sets the filter string to be used.
             * @param[in]   text the filter string.
             */
            auto setFilterText(QString text) -> void;

        protected:
            /**
             * @brief       Reimplements QSortFilterProxyModel::filterAcceptsRow
             *
             * @param[in]   sourceRow the row to be tested.
             * @param[in]   sourceParent the model index of the item in the source model.
             *
             * @returns     true if row is displayed; otherwise false.
             */
            bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

        private:
            QString m_filterText;
    };
}

#endif //NEDRYSOFT_FAVOURITESSORTPROXYFILTERMODEL_H
