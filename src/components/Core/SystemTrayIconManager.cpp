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

#include "SystemTrayIconManager.h"

#if defined(Q_OS_MACOS)
#include <MacMenubarIcon>
#include <MacPopover>
#else
#include <QSystemTrayIcon>
#endif

#include <QLabel>
#include <QVBoxLayout>

constexpr auto TrayPixmap = ":/app/images/appicon/mono/appicon-1024x1024@2x.png";

Nedrysoft::Core::SystemTrayIconManager::SystemTrayIconManager(QObject *parent) :
        m_basePixmap(TrayPixmap) {

#if defined(Q_OS_MACOS)
    m_menubarIcon = new Nedrysoft::MacHelper::MacMenubarIcon(QPixmap(TrayPixmap));

    connect(m_menubarIcon, &Nedrysoft::MacHelper::MacMenubarIcon::clicked, [=] {
        auto popover = new Nedrysoft::MacHelper::MacPopover;

        QWidget *contentWidget = new QWidget;

        auto contentLayout = new QVBoxLayout;

        contentLayout->addWidget(new QLabel("Hello!"));

        contentWidget->setLayout(contentLayout);

        popover->show(m_menubarIcon,
                      contentWidget,
                      contentWidget->sizeHint(),
                      Nedrysoft::MacHelper::MacPopover::Edge::MaxYEdge);
    });
#else
    m_systemTrayIcon = new QSystemTrayIcon;
#endif
}

Nedrysoft::Core::SystemTrayIconManager::~SystemTrayIconManager() {

}

auto Nedrysoft::Core::SystemTrayIconManager::setVisible(bool visible) -> void {
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

auto Nedrysoft::Core::SystemTrayIconManager::setIconColour(const QColor &iconColour) -> void {
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
