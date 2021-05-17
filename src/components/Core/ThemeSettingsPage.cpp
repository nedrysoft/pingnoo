/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 01/02/2021.
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

#include "ThemeSettingsPage.h"

#include <QLabel>

//#include "ThemeSettingsPageWidget.h"

Nedrysoft::Core::ThemeSettingsPage::ThemeSettingsPage(QWidget *parent) :
        m_settingsWidget(nullptr) {

    Q_UNUSED(parent)
}

Nedrysoft::Core::ThemeSettingsPage::~ThemeSettingsPage() {
}

auto Nedrysoft::Core::ThemeSettingsPage::section() -> QString {
    return tr("General");
}

auto Nedrysoft::Core::ThemeSettingsPage::category() -> QString {
    return tr("Theme");
}

auto Nedrysoft::Core::ThemeSettingsPage::description() -> QString {
    return tr("The configuration options for the application theme.");
}

auto Nedrysoft::Core::ThemeSettingsPage::icon(bool isDarkMode) -> QIcon {
    if (isDarkMode) {
        return QIcon(":/Core/icons/2x/settings_white_24dp.png");
    } else {
        return QIcon(":/Core/icons/2x/settings_black_24dp.png");
    }
}

auto Nedrysoft::Core::ThemeSettingsPage::createWidget() -> QWidget * {
    if (!m_settingsWidget) {
        m_settingsWidget = new QLabel("Hello World!");//HostMaskerSettingsPageWidget;

        connect(m_settingsWidget, &QWidget::destroyed, [=](QObject *object) {
           m_settingsWidget = nullptr;
        });
    }

    //m_settingsWidget->initialise();

    return m_settingsWidget;
}

auto Nedrysoft::Core::ThemeSettingsPage::canAcceptSettings() -> bool {
    if (m_settingsWidget) {
        //return m_settingsWidget->canAcceptSettings();
    }

    return true;
}

auto Nedrysoft::Core::ThemeSettingsPage::acceptSettings() -> void {
    if (m_settingsWidget) {
        //m_settingsWidget->acceptSettings();
    }
}
