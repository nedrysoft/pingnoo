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

#include "RibbonWidget.h"
#include "RibbonWidgetPlugin.h"
#include <QtPlugin>

RibbonWidgetPlugin::RibbonWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<RibbonWidgetPlugin *>("RibbonWidgetPlugin");
}

void RibbonWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core)

    if (initialized)
        return;

    initialized = true;
}

bool RibbonWidgetPlugin::isInitialized() const
{
    return initialized;
}

QWidget *RibbonWidgetPlugin::createWidget(QWidget *parent)
{
    return new FizzyAde::Ribbon::RibbonWidget(parent);
}

QString RibbonWidgetPlugin::name() const
{
    return QStringLiteral("FizzyAde::Ribbon::RibbonWidget");
}

QString RibbonWidgetPlugin::group() const
{
    return QStringLiteral("Ribbon Widgets");
}

QIcon RibbonWidgetPlugin::icon() const
{
    return QIcon();
}

QString RibbonWidgetPlugin::toolTip() const
{
    return QString();
}

QString RibbonWidgetPlugin::whatsThis() const
{
    return QString();
}

bool RibbonWidgetPlugin::isContainer() const
{
    return false;
}
/*

           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"
*/
QString RibbonWidgetPlugin::domXml() const
{
    /*return QLatin1String("\
     <ui language=\"c++\" displayname=\"Ribbon Widget\">\
         <widget class=\"FizzyAde::Ribbon::RibbonWidget\" name=\"ribbonWidget\">\
             <widget class=\"QWidget\" name=\"tab\" />\
         </widget>\
         <customwidgets>\
             <customwidget>\
                 <class>FizzyAde::Ribbon::RibbonWidget</class>\
                 <extends>QTabWidget</extends>\
                 <addpagemethod>addPage</addpagemethod>\
             </customwidget>\
         </customwidgets>\
     </ui>");*/

    return "<ui language=\"c++\" displayname=\"Ribbon Widget\">\n"
           " <widget class=\"FizzyAde::Ribbon::RibbonWidget\" name=\"ribbonWidget\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString RibbonWidgetPlugin::includeFile() const
{
    return QStringLiteral("Ribbon/RibbonWidget.h");
}
