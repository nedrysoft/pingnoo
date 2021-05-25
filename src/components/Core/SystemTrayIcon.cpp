/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 03/05/2021.
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

#include "SystemTrayIcon.h"

#if defined(Q_OS_MACOS)
#include <MacMenubarIcon>
#include <MacPopover>
#else
#include <QSystemTrayIcon>
#endif

#include <QLabel>
#include <QVBoxLayout>

constexpr auto DefaultTrayPixmap = ":/app/images/appicon/mono/appicon-1024x1024@2x.png";

Nedrysoft::Core::SystemTrayIcon::SystemTrayIcon(QObject *parent) :
        m_basePixmap(QPixmap(DefaultTrayPixmap)) {

    createIcon();
}

Nedrysoft::Core::SystemTrayIcon::SystemTrayIcon(const QPixmap &pixmap, QObject *parent) :
        m_basePixmap(pixmap) {

    createIcon();
}

auto Nedrysoft::Core::SystemTrayIcon::createIcon() -> void {
#if defined(Q_OS_MACOS)
    m_menubarIcon = new Nedrysoft::MacHelper::MacMenubarIcon(m_basePixmap);

    connect(m_menubarIcon,
            &Nedrysoft::MacHelper::MacMenubarIcon::clicked,
            [=](Nedrysoft::MacHelper::MouseButton button) {

        switch(button) {
            case Nedrysoft::MacHelper::MouseButton::Left: {
                Q_EMIT clicked(Nedrysoft::Core::ISystemTrayIcon::MouseButton::Left);

                break;
            }

            case Nedrysoft::MacHelper::MouseButton::Right: {
                Q_EMIT clicked(Nedrysoft::Core::ISystemTrayIcon::MouseButton::Right);

                break;
            }

            default: {
                break;
            }
        }
    });
#else
    m_systemTrayIcon = new QSystemTrayIcon;

    m_systemTrayIcon->show();

    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
        if (reason==QSystemTrayIcon::Trigger) {
            Q_EMIT clicked(Nedrysoft::Core::ISystemTrayIcon::MouseButton::Left);
        } else if (reason==QSystemTrayIcon::Context) {
            Q_EMIT clicked(Nedrysoft::Core::ISystemTrayIcon::MouseButton::Right);
        }
    });
#endif
}

Nedrysoft::Core::SystemTrayIcon::~SystemTrayIcon() {
#if !defined(Q_OS_MACOS)
    delete m_systemTrayIcon;
#endif
}

auto Nedrysoft::Core::SystemTrayIcon::geometry() -> QRect {
#if defined(Q_OS_MACOS)
    return m_statusbarHelper-buttonRect();
#else
    return m_systemTrayIcon->geometry();
#endif
}

auto Nedrysoft::Core::SystemTrayIcon::setVisible(bool visible) -> void {
    m_visible = visible;

#if defined(Q_OS_MACOS)
    if (m_visible) {
        m_menubarIcon->show();
    } else {
        m_menubarIcon->hide();
    }
#else
    if (m_visible) {
        m_systemTrayIcon->show();
    } else {
        m_systemTrayIcon->hide();
    }
#endif
}

auto Nedrysoft::Core::SystemTrayIcon::setColour(const QColor &iconColour) -> void {
    QImage temporaryImage = m_basePixmap.toImage();

    for(int y=0;y<temporaryImage.height();y++) {
        for (int x = 0; x < temporaryImage.width(); x++) {
            auto pixelColour = temporaryImage.pixelColor(x, y);

            pixelColour.setRedF(( pixelColour.redF() + iconColour.redF()) / 2.0);
            pixelColour.setGreenF(( pixelColour.greenF() + iconColour.greenF()) / 2.0);
            pixelColour.setBlueF(( pixelColour.blueF() + iconColour.blueF()) / 2.0);

            pixelColour.setAlphaF(pixelColour.alphaF() * iconColour.alphaF());

            temporaryImage.setPixelColor(x, y, pixelColour);
        }
    }

#if defined(Q_OS_MACOS)
    m_menubarIcon->setPixmap(QPixmap::fromImage(temporaryImage));
#else
    m_systemTrayIcon->setIcon(QIcon(QPixmap::fromImage(temporaryImage)));
#endif
}

#if defined(Q_OS_MACOS)
auto Nedrysoft::Core::SystemTrayIcon::menubarIcon() -> Nedrysoft::MacHelper::MacMenubarIcon * {
    return m_menubarIcon;
}
#endif