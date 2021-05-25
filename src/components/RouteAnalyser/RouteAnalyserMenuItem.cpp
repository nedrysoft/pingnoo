/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 23/05/2021.
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

#include "RouteAnalyserMenuItem.h"

#include <ColourManager.h>
#include <IComponentManager>
#include <QFontDatabase>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <random>

constexpr auto HeaderString = R"(
<span style="font-weight: bold">LOSS: </span>
<span style="font-weight: light;">%1%</span>
)";

#if defined(Q_OS_MACOS)
constexpr auto TitleFontSize = 14;
constexpr auto DefaultFontSize = 12;
#else
constexpr auto TitleFontSize = 10;
constexpr auto DefaultFontSize = 8;
#endif

int hostIndex = 0;

Nedrysoft::RouteAnalyser::RouteAnalyserMenuItem::RouteAnalyserMenuItem(QWidget *parent) :
        QWidget(parent) {

    auto mainLayout = new QVBoxLayout;
    auto footerLayout = new QHBoxLayout;
    auto headerLayout = new QHBoxLayout;

    auto targetLabel = new QLabel;
    auto lossLabel = new QLabel;

    QFont font("Roboto Mono");

    auto colourName = QColor(Nedrysoft::RouteAnalyser::ColourManager::getCriticalColour()).name();

    headerLayout->setSpacing(0);
    headerLayout->setContentsMargins(0,0,0,0);

    font.setPointSize(DefaultFontSize);

    lossLabel->setFont(font);
    lossLabel->setText(QString(HeaderString).arg((double)(rand()%100)/30.0, 0, 'f', 2));
    lossLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    font.setPointSize(TitleFontSize);

    auto targetHosts = QStringList() << "github.com" << "google.com" << "apple.com" << "cloudflare.com" << "qt.io";

    auto targetLayout = new QVBoxLayout;

    targetLabel->setFont(font);
    targetLabel->setText(QString("<u><b>%1</u></b>").arg(targetHosts[hostIndex]));
    targetLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);

    hostIndex = hostIndex % 5;

    targetLayout->addWidget(targetLabel);
    targetLayout->addSpacerItem(new QSpacerItem(0, 3));

    headerLayout->addWidget(lossLabel);
    headerLayout->addLayout(targetLayout);

    mainLayout->addLayout(headerLayout);

    mainLayout->setContentsMargins(3, 3, 3, 3);
    mainLayout->setSpacing(0);

    auto graphWidget = new QLabel;

    auto idealColourName = QColor(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour()).name();

    graphWidget->setStyleSheet("background: "+idealColourName);
    graphWidget->setMinimumHeight(80);
    graphWidget->setMaximumHeight(80);

    mainLayout->addSpacerItem(new QSpacerItem(0, 2));
    mainLayout->addWidget(graphWidget);

    auto basePoint = rand() % 100;

    auto minString = QString("%1.%2 ms").arg(basePoint).arg(rand()%10, 2, 10, QLatin1Char('0'));
    auto maxString = QString("%1.%2 ms").arg(60+basePoint).arg(rand()%10, 2, 10, QLatin1Char('0'));
    auto avgString = QString("%1.%2 ms").arg(35+basePoint).arg(rand()%10, 2, 10, QLatin1Char('0'));

    auto minLabel = new QLabel(
            QString("<span style=\"font-weight: bold\">MIN:</span> <span style=\"font-weight: light\">%1</span>")
                .arg(minString));

    auto maxLabel = new QLabel(
            QString("<span style=\"font-weight: bold\">MAX:</span> <span style=\"font-weight: light\">%1</span>")
                .arg(maxString));

    auto avgLabel = new QLabel(
            QString("<span style=\"font-weight: bold\">AVG:</span> <span style=\"font-weight: light\">%1</span>")
                .arg(avgString));

    minLabel->setAlignment(Qt::AlignLeft);
    maxLabel->setAlignment(Qt::AlignLeft);
    avgLabel->setAlignment(Qt::AlignRight);

    font.setPointSize(DefaultFontSize);

    minLabel->setFont(font);
    maxLabel->setFont(font);
    avgLabel->setFont(font);

    footerLayout->setContentsMargins(0, 0, 0, 0);

    footerLayout->addWidget(minLabel);
    footerLayout->addWidget(maxLabel);
    footerLayout->addWidget(avgLabel);

    mainLayout->addSpacerItem(new QSpacerItem(0, 2));
    mainLayout->addLayout(footerLayout);

    setLayout(mainLayout);

    QPixmap pixmap(350, 80);

    QPainter p(&pixmap);

    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(pixmap.rect(), QBrush(QColor(Nedrysoft::RouteAnalyser::ColourManager::getIdealColour())));

    p.setPen(Qt::black);

    int lastPoint = -1;
    int thisPoint;

    double origin = 80-(rand() %20)-10;

    for (int x = 0; x < 350; x+=1) {

        if (lastPoint==-1) {
            lastPoint = origin;
        }

        int sizeValue = rand() % 50;

        if (sizeValue>46) {
            thisPoint = origin-(rand() % 30)-30;;
        } else if (sizeValue>20) {
            thisPoint = origin-(rand() % 30);;
        } else {
            thisPoint = origin-(rand() % 8);;
        }

        thisPoint += (double)(rand() % 8);

        p.drawLine(x, lastPoint, x, thisPoint);
    }


    graphWidget->setPixmap(pixmap);

    setMinimumWidth(350);
}

Nedrysoft::RouteAnalyser::RouteAnalyserMenuItem::~RouteAnalyserMenuItem() {

}