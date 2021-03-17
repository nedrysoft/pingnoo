/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 01/12/2020.
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

#include "SplashScreen.h"

#include <QPainter>
#include <QDirIterator>
#include <QFontDatabase>

constexpr auto splashScreenFilename = ":/app/images/splashscreen.png";
constexpr auto fontFamily = "Poppins";
#if defined(Q_OS_MAC)
constexpr auto fontSize = 25;
#else
constexpr auto fontSize = 20;
#endif

constexpr auto splashScreenWidth = 700;
constexpr auto alphaLevel = 255.0 * 0.8;
constexpr auto textColour = qRgba(0xFF, 0xFF, 0xFF, alphaLevel);
constexpr auto versionRect = QRectF(45, 123, 210, 32);

Nedrysoft::SplashScreen::SplashScreen() :
        QSplashScreen(QPixmap(), Qt::WindowStaysOnTopHint) {

    auto pixmap = QPixmap(splashScreenFilename);
    auto scaledPixmap = pixmap.scaledToWidth(splashScreenWidth * pixmap.devicePixelRatio(), Qt::SmoothTransformation);

    QDirIterator fontDirIterator(":/app/fonts", QDirIterator::Subdirectories);

    while (fontDirIterator.hasNext()) {
        fontDirIterator.next();

        QFontDatabase::addApplicationFont(fontDirIterator.filePath());
    }

    m_scaleFactor = static_cast<float>(scaledPixmap.width()) / static_cast<float>(pixmap.width());

    setPixmap(scaledPixmap);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    show();
}

Nedrysoft::SplashScreen::~SplashScreen() {

}

auto Nedrysoft::SplashScreen::getInstance() -> Nedrysoft::SplashScreen * {
    static Nedrysoft::SplashScreen *instance;

    if (!instance) {
        instance = new Nedrysoft::SplashScreen;
    }

    return ( instance );
}

auto Nedrysoft::SplashScreen::drawContents(QPainter *painter) -> void {
    auto font = QFont(fontFamily, fontSize, QFont::Weight::Bold);
    auto versionText = QString("%1.%2.%3").arg(PINGNOO_GIT_YEAR).arg(PINGNOO_GIT_MONTH).arg(PINGNOO_GIT_DAY);
    auto textRect = QRectF(versionRect.topLeft()*m_scaleFactor, versionRect.size()*m_scaleFactor).toRect();

    painter->save();

    painter->setFont(font);

    painter->setPen(QColor::fromRgba(textColour));

    QFontMetrics metrics(font);

    auto boxRect = metrics.tightBoundingRect(versionText);

    textRect.adjust(0, (versionRect.height()-boxRect.height()), 0, (versionRect.height()-boxRect.height()));

    painter->drawText(versionRect, Qt::AlignCenter | Qt::AlignVCenter, versionText);

    painter->restore();
}
