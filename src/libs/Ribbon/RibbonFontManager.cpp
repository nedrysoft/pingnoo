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

#include "RibbonFontManager.h"
#include <QFontDatabase>

FizzyAde::Ribbon::RibbonFontManager::RibbonFontManager()
{
    m_regularFontId = QFontDatabase::addApplicationFont(":/FizzyAde/Ribbon/OpenSans/OpenSans-Regular.ttf");
    m_boldFontId = QFontDatabase::addApplicationFont(":/FizzyAde/Ribbon/OpenSans/OpenSans-Bold.ttf");
}

FizzyAde::Ribbon::RibbonFontManager *FizzyAde::Ribbon::RibbonFontManager::getInstance()
{
    static auto instance = new RibbonFontManager();

    return(instance);
}

QString FizzyAde::Ribbon::RibbonFontManager::normalFont()
{
    return(QFontDatabase::applicationFontFamilies(m_regularFontId).at(0));
}

QString FizzyAde::Ribbon::RibbonFontManager::boldFont()
{
    return(QFontDatabase::applicationFontFamilies(m_boldFontId).at(0));
}
