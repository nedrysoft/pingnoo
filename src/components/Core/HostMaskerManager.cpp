/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 19/06/2021.
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

#include "HostMaskerManager.h"

#include <QMetaEnum>
#include <QSettings>

Nedrysoft::Core::HostMaskerManager::HostMaskerManager() {
    QSettings settings;

    m_maskingState[Nedrysoft::Core::HostMaskType::Screen] = settings.value("HostMaskerManager/Screen",true).toBool();
    m_maskingState[Nedrysoft::Core::HostMaskType::Output] = settings.value("HostMaskerManager/Output",true).toBool();
    m_maskingState[Nedrysoft::Core::HostMaskType::Clipboard] = settings.value(
        "HostMaskerManager/Clipboard",
        true
    ).toBool();
}

Nedrysoft::Core::HostMaskerManager::~HostMaskerManager() {
}

auto Nedrysoft::Core::HostMaskerManager::setEnabled(Nedrysoft::Core::HostMaskType type, bool state) -> void {
    QSettings settings;

    if (m_maskingState.contains(type)) {
        if (m_maskingState[type]==state) {
            return;
        }
    }

    m_maskingState[type] = state;

    QMetaEnum metaEnum = QMetaEnum::fromType<Nedrysoft::Core::HostMaskType>();

    auto keyName = metaEnum.valueToKey(static_cast<int>(type));

    settings.setValue(QString("HostMaskerManager/%1").arg(keyName),state);

    Q_EMIT maskStateChanged(type, state);
}

auto Nedrysoft::Core::HostMaskerManager::enabled(Nedrysoft::Core::HostMaskType type) -> bool {
    if (m_maskingState.contains(type)) {
        return m_maskingState[type];
    }

    return false;
}

auto Nedrysoft::Core::HostMaskerManager::add(Nedrysoft::Core::IHostMasker *hostMasker) -> void {
    if (!m_maskers.contains(hostMasker)) {
        m_maskers.append(hostMasker);
    }
}

auto Nedrysoft::Core::HostMaskerManager::remove(Nedrysoft::Core::IHostMasker *hostMasker) -> void {
    m_maskers.removeAll(hostMasker);
}

auto Nedrysoft::Core::HostMaskerManager::maskers() -> QList<Nedrysoft::Core::IHostMasker *> {
    return m_maskers;
}