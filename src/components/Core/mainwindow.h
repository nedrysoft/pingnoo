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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

namespace FizzyAde::Core
{
    class IMenu;
}

namespace FizzyAde::Core
{
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        void initialise();

    private slots:

        void onCutButtonClicked();

    private:
        void createDefaultCommands();
        void createCommand(QString commandId, QAbstractButton *button);
        FizzyAde::Core::IMenu *createMenu(QString menuId, QString parentMenuId=QString());
        FizzyAde::Core::IMenu *findMenu(QString menuId);
        void addMenuCommand(QString menuId, QString commandId);
    protected:

    private:
        Ui::MainWindow *ui;

        /*
        QLabel *m_pointInfoLabel;
        QLabel *m_hopInfoLabel;
        QLabel *m_hostInfoLabel;
        QLabel *m_timeInfoLabel;
        */
    };
}

#endif // MAINWINDOW_H
