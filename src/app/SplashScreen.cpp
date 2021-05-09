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

#include <QtGlobal>

#include "SplashScreen.h"

#include <QPainter>
#include <QDirIterator>
#include <QFontDatabase>

constexpr auto SplashScreenFilename = ":/app/images/splashscreen.png";
constexpr auto FontFamily = "Poppins";
#if defined(Q_OS_MAC)
constexpr auto FontSize = 25;
#else
constexpr auto FontSize = 20;
#endif

constexpr auto SplashScreenWidth = 700;
constexpr auto AlphaLevel = 255.0 * 0.8;
constexpr auto TextColour = qRgba(0xFF, 0xFF, 0xFF, AlphaLevel);
constexpr auto VersionRect = QRectF(45, 123, 210, 32);

Nedrysoft::SplashScreen::SplashScreen() :
        QSplashScreen(QPixmap(), Qt::WindowStaysOnTopHint) {

    auto pixmap = QPixmap(SplashScreenFilename);
    auto scaledPixmap = pixmap.scaledToWidth(SplashScreenWidth * pixmap.devicePixelRatio(), Qt::SmoothTransformation);

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
    auto font = QFont(FontFamily, FontSize, QFont::Weight::Bold);
    auto versionText = QString("%1.%2.%3").arg(PINGNOO_GIT_YEAR).arg(PINGNOO_GIT_MONTH).arg(PINGNOO_GIT_DAY);
    auto textRect = QRectF(VersionRect.topLeft()*m_scaleFactor, VersionRect.size()*m_scaleFactor).toRect();

    painter->save();

    painter->setFont(font);

    painter->setPen(QColor::fromRgba(TextColour));

    QFontMetrics metrics(font);

    auto boxRect = metrics.tightBoundingRect(versionText);

    textRect.adjust(0, (VersionRect.height()-boxRect.height()), 0, (VersionRect.height()-boxRect.height()));

    painter->drawText(VersionRect, Qt::AlignCenter | Qt::AlignVCenter, versionText);

    painter->restore();
}
