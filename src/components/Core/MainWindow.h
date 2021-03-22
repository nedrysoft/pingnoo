/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
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

namespace Nedrysoft::Core::Ui {

}

namespace Nedrysoft::SettingsDialog {
    class SettingsDialog;
}

namespace Nedrysoft::Core {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class IMenu;
    class RibbonBarManager;
    class EditorManager;

    /**
     * @brief       The MainWindow class provides the Main Application window.
     */
    class MainWindow :
            public QMainWindow {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new MainWindow instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            MainWindow(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the MainWindow.
             */
            ~MainWindow();

            /**
             * @brief       The initialise class sets up various subsystems for the main application.
             */
            auto initialise() -> void;

        protected:
            /**
             * @brief       Reimplements QMainWindow::closeEvent(QCloseEvent *event).
             */
            virtual void closeEvent(QCloseEvent *closeEvent);

        private:
            /**
             * @brief       Creates the placeholders for the default commands which other plugins
             *              can then use these in their own specific context.
             */
            auto createDefaultCommands() -> void;

            /**
             * @brief       Registers the default actions for the global context to the
             *              registered commands.
             */
            auto registerDefaultCommands() -> void;

            /**
             * @brief      Creates a Command and registers it with the system.
             *
             * @param[in]  commandId the identifier string for the command.
             * @param[in]  button the button class if this is also bound to a button.
             * @param[in]   menuRole the menu role is used by some platforms to put menu items in the correct location.
             *
             * @returns     an ICommand instance of the newly created command.
             */
            auto createCommand(
                    QString commandId,
                    QAbstractButton *button = nullptr,
                    QAction::MenuRole menuRole = QAction::NoRole ) -> Nedrysoft::Core::ICommand *;

            /**
             * @brief       Adds a command to a menu.
             * @param[in]   commandId the command identifier.
             * @param[in]   menuId the menu identifier.
             * @param[in]   groupId the group that the item is part of; otherwise nullptr if not a specific group.
             */
            auto addMenuCommand(QString commandId, QString menuId, QString groupId = QString()) -> void;

            /**
             * @brief       Creates a menu with the given identifier.
             * @param[in]   menuId the identifier of the menu.
             * @param[in]   parentMenuId  if the menu is a submenu, then the identifier of the parent menu.
             *
             * @returns     an IMenu pointer to the menu.
             */
            auto createMenu(QString menuId, QString parentMenuId = QString()) -> Nedrysoft::Core::IMenu *;

            /**
             * @brief       Returns the IMenu pointer for a named menu.
             * @param[in]   menuId the identifier of the menu to find.
             *
             * @returns     The IMenu pointer if found; otherwise nullptr.
             */
            auto findMenu(QString menuId) -> Nedrysoft::Core::IMenu *;

        private:
            Ui::MainWindow *ui;

            Nedrysoft::Core::RibbonBarManager *m_ribbonBarManager;

            Nedrysoft::Core::EditorManager *m_editorManager;

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
