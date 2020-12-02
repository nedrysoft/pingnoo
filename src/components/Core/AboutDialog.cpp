/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
 * An open source ping path analyser
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

    ui->titleLabel->setTextFormat(Qt::RichText);
    ui->titleLabel->setText(QString("<span style=\"font-size:16pt; font-weight:600;\">Pingnoo %1.%2.%3-%4</span>").arg(
            PINGNOO_GIT_YEAR).arg(PINGNOO_GIT_MONTH).arg(PINGNOO_GIT_DAY).arg(PINGNOO_GIT_BRANCH));

    ui->buildTimeLabel->setText(QString("Built on %1 %2").arg(__DATE__).arg(__TIME__));

    ui->buildRevisionLabel->setText(
            QString("From revision <a href=\"https://github.com/fizzyade/pingnoo/commit/%1\">%1</a>").arg(
                    PINGNOO_GIT_HASH));

    ui->qtVersionLabel->setText(
            QString("Based on Qt %1.%2.%3 (%4, %5 bit), runtime Qt %6").arg(QT_VERSION_MAJOR).arg(QT_VERSION_MINOR).arg(
                    QT_VERSION_PATCH).arg(compiler()).arg(QSysInfo::WordSize).arg(QLibraryInfo::version().toString()));
}

Nedrysoft::Core::AboutDialog::~AboutDialog() {
    delete ui;
}

QString Nedrysoft::Core::AboutDialog::compiler() {
#if defined(Q_CC_CLANG)
#if defined(__apple_build_version__)
    return QString("Clang %1.%2 (Apple)").arg(__clang_major__).arg(__clang_minor__);
#endif
    return QString("Clang %1.%2").arg(__clang_major__).arg(__clang_minor__);
#elif defined(Q_CC_GNU)
    return QString("GCC %1").arg(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER > 1999) {
        return QString("MSVC <unknown>");
    } else if (_MSC_VER >= 1920) {
        return QString("MSVC 2019");
    } else if if (_MSC_VER >= 1910) {
        return QString("MSVC 2017");
    } else if (_MSC_VER >= 1900) {
        return QString("MSVC 2015");
    }
#endif
    return QString("<unknown compiler>");
}
