/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 20/12/2020.
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
// You may need to build the project (run Qt uic code generator) to get "ui_LatencySettingsPage.h" resolved

#include "TargetSettingsPage.h"

#include "TargetSettingsPageWidget.h"

Nedrysoft::RouteAnalyser::TargetSettingsPage::TargetSettingsPage(QWidget *parent) :
        m_widget(new TargetSettingsPageWidget) {

    Q_UNUSED(parent)
}

Nedrysoft::RouteAnalyser::TargetSettingsPage::TargetSettingsPage::~TargetSettingsPage() {
    if (m_widget->parentWidget()==nullptr) {
        delete m_widget;
    }
}

auto Nedrysoft::RouteAnalyser::TargetSettingsPage::section() -> QString {
    return tr("Route Analyser");
}

auto Nedrysoft::RouteAnalyser::TargetSettingsPage::category() -> QString {
    return tr("Target");
}

auto Nedrysoft::RouteAnalyser::TargetSettingsPage::description() -> QString {
    return "";
}

auto Nedrysoft::RouteAnalyser::TargetSettingsPage::icon() -> QIcon {
    return QIcon(":/RouteAnalyser/icons/analyser.png");
}

auto Nedrysoft::RouteAnalyser::TargetSettingsPage::widget() -> QWidget * {
    return m_widget;
}

auto Nedrysoft::RouteAnalyser::TargetSettingsPage::canAcceptSettings() -> bool {
    return true;
}

auto Nedrysoft::RouteAnalyser::TargetSettingsPage::acceptSettings() -> void {

}
