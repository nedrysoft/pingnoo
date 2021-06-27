/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 26/06/2021.
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

#ifndef PINGNOO_COMPONENTS_CORE_RIBBONDROPBUTTONACTION_H
#define PINGNOO_COMPONENTS_CORE_RIBBONDROPBUTTONACTION_H

#include <QAction>

namespace Nedrysoft { namespace Core {
    class RibbonDropButtonAction :
            public QAction {

        private:
            Q_OBJECT

        public:
            Q_SIGNAL void triggered(bool dropdown);
    };
}}

#endif // PINGNOO_COMPONENTS_CORE_RIBBONDROPBUTTONACTION_H
