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

#ifndef NEDRYSOFT_PINGNOO_CONSTANTS_H
#define NEDRYSOFT_PINGNOO_CONSTANTS_H

#include <QtGlobal>

#include <QMap>
#include <QObject>

namespace Pingnoo { namespace Constants {
    constexpr auto EditCut = "Edit.Cut";
    constexpr auto EditCopy = "Edit.Copy";
    constexpr auto EditPaste = "Edit.Paste";

    constexpr auto ApplicationMenuBar = "Pingnoo.ApplicationMenu";
    constexpr auto ApplicationContextMenu = "Pingnoo.ApplicationContexyMenu";
    constexpr auto MenuApplication = "Menu.Pingnoo";
    constexpr auto MenuFile = "Menu.File";
    constexpr auto MenuEdit = "Menu.Edit";
    constexpr auto MenuHelp = "Menu.Help";

    constexpr auto FileOpen = "Menu.File.Open";
    constexpr auto FileQuit = "Menu.File.Quit";

    constexpr auto HelpAbout = "Menu.Help.About";
    constexpr auto HelpAboutComponents = "Menu.Help.AboutComponents";

    constexpr auto FilePreferences = "Menu.File.Preferences";

    constexpr auto DefaultGroupTop = "Group.Top";
    constexpr auto DefaultGroupMiddle = "Group.Middle";
    constexpr auto DefaultGroupBottom = "Group.Bottom";

    constexpr auto FileNewGroup = "Group.File.New";
    constexpr auto FileOpenGroup = "Group.File.Open";
    constexpr auto FileSaveGroup = "Group.File.Save";
    constexpr auto FileMiscGroup = "Group.File.Misc";
    constexpr auto FileExitGroup = "Group.File.Exit";

    constexpr auto RibbonHomePage = "Ribbon.Page.Home";
    constexpr auto RibbonHostMaskingPage = "Ribbon.Page.HostMasking";

    const QMap<QString, QString> CommandStringMap = {
            {EditCut, QT_TR_NOOP("Cut")},
            {EditCopy, QT_TR_NOOP("Copy")},
            {EditPaste, QT_TR_NOOP("Paste")},
            {FileOpen, QT_TR_NOOP("Open...")},
            {FileQuit, QT_TR_NOOP("Exit")},
#if defined(Q_OS_MACOS)
            {FilePreferences, QT_TR_NOOP("Preferences...")},
#else
            {FilePreferences,     QT_TR_NOOP("Settings...")},
#endif
            {HelpAbout, QT_TR_NOOP("About Pingnoo")},
            {HelpAboutComponents, QT_TR_NOOP("About Components...")}
    };

    const QMap<QString, QString> MenuStringMap = {
            {MenuFile,                  QT_TR_NOOP("File")},
            {MenuEdit,                  QT_TR_NOOP("Edit")},
            {MenuHelp,                  QT_TR_NOOP("Help")},
            {MenuApplication,           QT_TR_NOOP("Pingnoo")}
    };

    inline QString commandText(const QString &string) {
        if (CommandStringMap.contains(string))
            return QObject::tr(CommandStringMap[string].toUtf8());

        return string;
    }

    inline QString menuText(const QString &string) {
        if (MenuStringMap.contains(string))
            return QObject::tr(MenuStringMap[string].toUtf8());

        return string;
    }
}}

#endif // NEDRYSOFT_PINGNOO_CONSTANTS_H
