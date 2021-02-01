/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/07/2020.
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

#ifndef NEDRYSOFT_ROUTEANALYSER_NEWTARGETDIALOG_H
#define NEDRYSOFT_ROUTEANALYSER_NEWTARGETDIALOG_H

#include "Core/IPingEngineFactory.h"

#include <QDialog>

class QTextEdit;

namespace Nedrysoft::RouteAnalyser {
    namespace Ui {
        class NewTargetDialog;
    }

    class LineSyntaxHighlighter;

    /**
     * @brief       The NewTargetDialog class is a dialog that is used to create a new target.
     *
     * @details     The dialog displayed when creating a new route analyser instance, allows
     *              the user to select the target, ping engine, interval etc.
     */
    class NewTargetDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new NewTargetDialog instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            explicit NewTargetDialog(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the NewTargetDialog.
             */
            ~NewTargetDialog();

            /**
             * @brief       Returns the selected ping engine factory.
             *
             * @returns     the selected IPingEngineFactory.
             */
            auto pingEngineFactory() -> Nedrysoft::Core::IPingEngineFactory *;

            /**
             * @brief       Returns the selected target.
             *
             * @returns     the target. (host or ip address)
             */
            auto pingTarget() -> QString;

            /**
             * @brief       Returns the selected IP version.
             *
             * @returns     the ip version, (V4 or V6)
             */
            auto ipVersion() -> Nedrysoft::Core::IPVersion;

            /**
             * @brief       Returns he ping interval.
             *
             * @details     The ping interval is the number of seconds between ping clusters,
             *              this can be a sub-second value.
             *
             * @returns     the ping internal in seconds.
             */
            auto interval() -> double;

        protected:
            /**
             * @brief       Reimplements QWidget::eventFilter(QObject *watched, QEvent *event)
             *
             * @param[in]   watched the object that caused the event.
             * @param[in]   event the event.
             *
             * @returns     true to stop further event processing; otherwise false.
             */
            auto eventFilter(QObject *watched, QEvent *event) -> bool override;

        private:
            /**
             * @brief       Validate the fields of the dialog.
             *
             * @param[out]  string the error string if validation failed.
             *
             * @returns     nullptr on no error; otherwise the first widget that failed validation.
             */
            auto checkFieldsValid(QString &string) -> QWidget *;

            /**
             * @brief       Validates the fields of the dialog and enables/disables the ok/apply buttons.
             */
            auto validateFields() -> void ;

        private:
            Ui::NewTargetDialog *ui;
            LineSyntaxHighlighter *m_targetHighlighter;
            LineSyntaxHighlighter *m_intervalHighlighter;
    };
}

#endif // NEDRYSOFT_ROUTEANALYSER_NEWTARGETDIALOG_H
