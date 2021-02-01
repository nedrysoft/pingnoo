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

#include "LatencySettingsPage.h"

#include "LatencySettingsPageWidget.h"

Nedrysoft::RouteAnalyser::LatencySettingsPage::LatencySettingsPage(QWidget *parent) {
    Q_UNUSED(parent)
}

Nedrysoft::RouteAnalyser::LatencySettingsPage::LatencySettingsPage::~LatencySettingsPage() {

}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::section() -> QString {
    return tr("Route Analyser");
}


auto Nedrysoft::RouteAnalyser::LatencySettingsPage::category() -> QString {
    return tr("Latency");
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::description() -> QString {
    return "";
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::icon() -> QIcon {
    return QIcon(":/RouteAnalyser/icons/analyser.png");
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::widget() -> QWidget * {
    static auto widget = new LatencySettingsPageWidget;

    return widget;
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::canAcceptSettings() -> bool {
    return true;
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::acceptSettings() -> void {

}
