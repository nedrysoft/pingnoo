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

#include "HostMaskerSettingsPage.h"

#include "HostMaskerSettingsPageWidget.h"

Nedrysoft::Core::HostMaskerSettingsPage::HostMaskerSettingsPage(QWidget *parent) {
    Q_UNUSED(parent)
}

Nedrysoft::Core::HostMaskerSettingsPage::HostMaskerSettingsPage::~HostMaskerSettingsPage() {
}

auto Nedrysoft::Core::HostMaskerSettingsPage::section() -> QString {
    return tr("Host Maskers");
}

auto Nedrysoft::Core::HostMaskerSettingsPage::category() -> QString {
    return tr("Configuration");
}

auto Nedrysoft::Core::HostMaskerSettingsPage::description() -> QString {
    return tr("The configuration options for the available host maskers.");
}

auto Nedrysoft::Core::HostMaskerSettingsPage::icon() -> QIcon {
#if !defined(Q_OS_MACOS)
    return QIcon(":/Core/Icons/mask-inverted.png");
#else
    return QIcon(":/Core/Icons/mask.png");
#endif
}

auto Nedrysoft::Core::HostMaskerSettingsPage::createWidget() -> QWidget * {
    auto pageWidget = new HostMaskerSettingsPageWidget;

    pageWidget->initialise();

    return pageWidget;
}

auto Nedrysoft::Core::HostMaskerSettingsPage::canAcceptSettings() -> bool {
    return true;
}

auto Nedrysoft::Core::HostMaskerSettingsPage::acceptSettings() -> void {

}
