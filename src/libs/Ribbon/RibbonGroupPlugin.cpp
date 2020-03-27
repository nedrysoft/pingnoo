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

#include "RibbonGroup.h"
#include "RibbonGroupPlugin.h"
#include <QtPlugin>

RibbonGroupPlugin::RibbonGroupPlugin(QObject *parent)
    : QObject(parent)
{
}

void RibbonGroupPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool RibbonGroupPlugin::isInitialized() const
{
    return initialized;
}

QWidget *RibbonGroupPlugin::createWidget(QWidget *parent)
{
    return new FizzyAde::Ribbon::RibbonGroup(parent);
}

QString RibbonGroupPlugin::name() const
{
    return QStringLiteral("FizzyAde::Ribbon::RibbonGroup");
}

QString RibbonGroupPlugin::group() const
{
    return QStringLiteral("Ribbon Widgets");
}

QIcon RibbonGroupPlugin::icon() const
{
    return QIcon();
}

QString RibbonGroupPlugin::toolTip() const
{
    return QString();
}

QString RibbonGroupPlugin::whatsThis() const
{
    return QString();
}

bool RibbonGroupPlugin::isContainer() const
{
    return true;
}

QString RibbonGroupPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Ribbon Group\">\n"
           " <widget class=\"FizzyAde::Ribbon::RibbonGroup\" name=\"ribbonGroup\">\n"

           "  <property name=\"groupName\">\n"
           "   <string>Group</string>\n"
           "  </property>\n"

           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"

           " </widget>\n"
           "</ui>\n";
}

QString RibbonGroupPlugin::includeFile() const
{
    return QStringLiteral("Ribbon/RibbonGroup.h");
}
