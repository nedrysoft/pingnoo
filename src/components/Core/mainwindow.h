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
#include <QAction>
#include "Command.h"

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

        void on_toolButton_clicked();

    private:
        void createDefaultCommands();
        void registerDefaultCommands();

        FizzyAde::Core::ICommand *createCommand(QString commandId, QAbstractButton *button, QAction::MenuRole menuRole=QAction::NoRole);
        void addMenuCommand(QString menuId, QString commandId);

        FizzyAde::Core::IMenu *createMenu(QString menuId, QString parentMenuId=QString());
        FizzyAde::Core::IMenu *findMenu(QString menuId);

    protected:

    private:
        Ui::MainWindow *ui;

        QAction *m_aboutComponentsAction;
        QAction *m_aboutAction;

        /*
        QLabel *m_pointInfoLabel;
        QLabel *m_hopInfoLabel;
        QLabel *m_hostInfoLabel;
        QLabel *m_timeInfoLabel;
        */
    };
}

#endif // MAINWINDOW_H
