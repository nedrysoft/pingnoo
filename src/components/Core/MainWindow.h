/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/03/2020.
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

#include "Command.h"

#include <QAbstractButton>
#include <QAction>
#include <QMainWindow>

/**
 * @brief       Namespace for the core component user interface.
 */
namespace Nedrysoft::Core::Ui {}

namespace Nedrysoft::SettingsDialog {
    class SettingsDialog;
};

namespace Nedrysoft::Core {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class IMenu;

    class MainWindow :
            public QMainWindow {

        private:
            Q_OBJECT

        public:
            MainWindow(QWidget *parent = nullptr);

            ~MainWindow();

            void initialise();

        private:
            void createDefaultCommands();

            void registerDefaultCommands();

            Nedrysoft::Core::ICommand *
            createCommand(QString commandId, QAbstractButton *button, QAction::MenuRole menuRole = QAction::NoRole);

            void addMenuCommand(QString commandId, QString menuId, QString groupId = QString());

            Nedrysoft::Core::IMenu *createMenu(QString menuId, QString parentMenuId = QString());

            Nedrysoft::Core::IMenu *findMenu(QString menuId);

        protected:

        private:
            Ui::MainWindow *ui;

            QAction *m_aboutComponentsAction;
            QAction *m_aboutAction;
            QAction *m_settingsAction;
            QAction *m_exitAction;

            Nedrysoft::SettingsDialog::SettingsDialog *m_settingsDialog;

            /*
            QLabel *m_pointInfoLabel;
            QLabel *m_hopInfoLabel;
            QLabel *m_hostInfoLabel;
            QLabel *m_timeInfoLabel;
            */
    };
}

#endif // MAINWINDOW_H
