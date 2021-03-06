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

Nedrysoft::RouteAnalyser::LatencySettingsPage::LatencySettingsPage(QWidget *parent) :
        m_settingsPageWidget(nullptr) {

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

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::icon(bool isDarkMode) -> QIcon {
#if !defined(Q_OS_MACOS)
    return QIcon(":/RouteAnalyser/icons/2x/twotone_alt_route_black_24dp.png");
#else
    if (isDarkMode) {
        return QIcon(":/RouteAnalyser/icons/2x/twotone_alt_route_white_24dp.png");
    } else {
        return QIcon(":/RouteAnalyser/icons/2x/twotone_alt_route_black_24dp.png");
    }
#endif
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::createWidget() -> QWidget * {
    if (!m_settingsPageWidget) {
        m_settingsPageWidget = new LatencySettingsPageWidget;

        connect(m_settingsPageWidget, &QWidget::destroyed, [=](QObject *) {
            m_settingsPageWidget = nullptr;
        });
    }

    return m_settingsPageWidget;
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::canAcceptSettings() -> bool {
    if (m_settingsPageWidget) {
        return m_settingsPageWidget->canAcceptSettings();
    }

    return true;
}

auto Nedrysoft::RouteAnalyser::LatencySettingsPage::acceptSettings() -> void {
    if (m_settingsPageWidget) {
        m_settingsPageWidget->acceptSettings();
    }
}

