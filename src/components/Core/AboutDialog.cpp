/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 25/07/2020.
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

#include "AboutDialog.h"

#include "ui_AboutDialog.h"

#include <QDebug>
#include <QLibraryInfo>
#include <QSysInfo>

Nedrysoft::Core::AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Nedrysoft::Core::Ui::AboutDialog) {

    ui->setupUi(this);

    ui->iconLabel->setPixmap(QPixmap(":/app/images/appicon-512x512@2x.png"));

    ui->titleLabel->setTextFormat(Qt::RichText);

    auto versionOpenTag = "<span style=\"font-size:16pt; font-weight:600;\">";
    auto versionCloseTag = "</span>";
    auto versionText = versionOpenTag+QString(tr("Pingnoo %1.%2.%3-%4"))
            .arg(PINGNOO_GIT_YEAR)
            .arg(PINGNOO_GIT_MONTH)
            .arg(PINGNOO_GIT_DAY)
            .arg(PINGNOO_GIT_BRANCH)+versionCloseTag;

    ui->titleLabel->setText(versionText);

    ui->buildTimeLabel->setText(QString(tr("Built on %1 %2")).arg(__DATE__).arg(__TIME__));

    auto revisionOpenTag = QString("<a href=\"https://github.com/nedrysoft/pingnoo/commit/%1\">").arg(PINGNOO_GIT_HASH);
    auto revisionCloseTag = "</a>";
    auto revisionText = QString(tr("From revision %1"))
            .arg(revisionOpenTag+PINGNOO_GIT_HASH+revisionCloseTag);

    ui->buildRevisionLabel->setText(revisionText);

    ui->qtVersionLabel->setText(
            QString(tr("Based on Qt %1.%2.%3 (%4, %5 bit), runtime Qt %6"))
            .arg(QT_VERSION_MAJOR)
            .arg(QT_VERSION_MINOR)
            .arg(QT_VERSION_PATCH)
            .arg(compiler())
            .arg(QSysInfo::WordSize).arg(QLibraryInfo::version().toString()) );
}

Nedrysoft::Core::AboutDialog::~AboutDialog() {
    delete ui;
}

auto Nedrysoft::Core::AboutDialog::compiler() -> QString {
#if defined(Q_CC_CLANG) && defined(__apple_build_version__)
    return QString(tr("Clang %1.%2 (Apple)")).arg(__clang_major__).arg(__clang_minor__);
#elif defined(__apple_build_version__)
    return QString("Clang %1.%2").arg(__clang_major__).arg(__clang_minor__);
#elif defined(Q_CC_GNU)
    return QString("GCC %1").arg(__VERSION__);
#elif defined(Q_CC_MSVC)
#if (_MSC_VER > 1999)
    return QString("MSVC <unknown>");
#elif (_MSC_VER >= 1920)
    return QString("MSVC 2019");
#elif (_MSC_VER >= 1910)
    return QString("MSVC 2017");
#elif (_MSC_VER >= 1900)
    return QString("MSVC 2015");
#else
    return QString("<unknown compiler>");
#endif
#endif
}
