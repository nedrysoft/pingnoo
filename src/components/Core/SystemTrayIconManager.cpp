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

#include <QMenu>
#include <QSystemTrayIcon>
#include <qglobal.h>
#include <QMenu>
#include <QLabel>
#include <QDebug>
#include <QMap>
#include <QString>
#include <QPushButton>

constexpr auto trayPixmap = ":/app/images/appicon/mono/appicon-1024x1024@2x.png";

extern "C" void showPopover(QSystemTrayIcon *systemTrayIcon);

extern "C" QMap<QString, void *> addStatusIcon(QWidget *contentWidget);
extern "C" void removeStatusIcon(QMap<QString, void *> map);

QMap<QString, void *> tempMap;

Nedrysoft::Core::SystemTrayIconManager::~SystemTrayIconManager() {
    removeStatusIcon(tempMap);
}

Nedrysoft::Core::SystemTrayIconManager::SystemTrayIconManager(QObject *parent) :
        m_basePixmap(trayPixmap),
        m_systemTrayIcon(nullptr) {//new QSystemTrayIcon(QIcon(QPixmap(trayPixmap)), parent)) {

    auto label = new QPushButton("Har");

    label->setStyleSheet("QPushButton:hover {\n"
                         "     background-color: rgb(224, 255, 0);\n"
                         " }");

    tempMap = addStatusIcon(label);

    //setIconColour(Qt::white);
    //setVisible(false);
/*
    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
        QWidget widget;

        auto rect = m_systemTrayIcon->geometry();

        auto popupWidget = new QWidget(nullptr, Qt::Popup);

        auto popupSize = QSize( 300, 300);
#if defined(Q_OS_MACOS)
        auto popupRect = QRect(
                rect.center().x()-(popupSize.width()/2),
                rect.bottom(),
                popupSize.width(),
                popupSize.height() );
#elif defined(Q_OS_UNIX)
#error "functionality for unix needs to be implemented"
#elif defined(Q_OS_MACOS)

#endif
        popupWidget->setGeometry(popupRect);

        popupWidget->show();

        showPopover(m_systemTrayIcon);
    });*/
}

auto Nedrysoft::Core::SystemTrayIconManager::setVisible(bool visible) -> void {
    /*m_visible = visible;

    if (m_visible) {
        m_systemTrayIcon->show();
    } else {
        m_systemTrayIcon->hide();
    }*/
}

auto Nedrysoft::Core::SystemTrayIconManager::setIconColour(const QColor &iconColour) -> void {
    /*QImage temporaryImage = m_basePixmap.toImage();

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

    m_systemTrayIcon->setIcon(QIcon(QPixmap::fromImage(temporaryImage)));*/
}
