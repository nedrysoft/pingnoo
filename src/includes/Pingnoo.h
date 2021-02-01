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

#include <QMap>
#include <QObject>

namespace Pingnoo::Constants {
    constexpr auto editCut = "Edit.Cut";
    constexpr auto editCopy = "Edit.Copy";
    constexpr auto editPaste = "Edit.Paste";

    constexpr auto applicationMenuBar = "Pingnoo.ApplicationMenu";
    constexpr auto menuApplication = "Menu.Pingnoo";
    constexpr auto menuFile = "Menu.File";
    constexpr auto menuEdit = "Menu.Edit";
    constexpr auto menuHelp = "Menu.Help";

    constexpr auto fileOpen = "Menu.File.Open";
    constexpr auto fileExit = "Menu.File.Exit";

    constexpr auto helpAbout = "Menu.Help.About";
    constexpr auto helpAboutComponents = "Menu.Help.AboutComponents";

    constexpr auto filePreferences = "Menu.File.Preferences";

    constexpr auto defaultGroupTop = "Group.Top";
    constexpr auto defaultGroupMiddle = "Group.Middle";
    constexpr auto defaultGroupBottom = "Group.Bottom";

    constexpr auto fileNewGroup = "Group.File.New";
    constexpr auto fileOpenGroup = "Group.File.Open";
    constexpr auto fileSaveGroup = "Group.File.Save";
    constexpr auto fileMiscGroup = "Group.File.Misc";
    constexpr auto fileExitGroup = "Group.File.Exit";

    constexpr auto ribbonHomePage = "Ribbon.Page.Home";
    constexpr auto ribbonHostMaskingPage = "Ribbon.Page.HostMasking";

    const QMap<QString, QString> commandStringMap = {
            {editCut, QT_TR_NOOP("Cut")},
            {editCopy, QT_TR_NOOP("Copy")},
            {editPaste, QT_TR_NOOP("Paste")},
            {fileOpen, QT_TR_NOOP("Open...")},
            {fileExit, QT_TR_NOOP("Exit")},
#if defined(Q_OS_MACOS)
            {filePreferences, QT_TR_NOOP("Preferences...")},
#else
            {filePreferences,     QT_TR_NOOP("Settings...")},
#endif
            {helpAbout, QT_TR_NOOP("About Pingnoo")},
            {helpAboutComponents, QT_TR_NOOP("About Components...")}
    };

    const QMap<QString, QString> menuStringMap = {
            {menuFile,        QT_TR_NOOP("File")},
            {menuEdit,        QT_TR_NOOP("Edit")},
            {menuHelp,        QT_TR_NOOP("Help")},
            {menuApplication, QT_TR_NOOP("Pingnoo")}
    };

    inline QString commandText(const QString &string) {
        if (commandStringMap.contains(string))
            return QObject::tr(commandStringMap[string].toUtf8());

        return string;
    }

    inline QString menuText(const QString &string) {
        if (menuStringMap.contains(string))
            return QObject::tr(menuStringMap[string].toUtf8());

        return string;
    }
}

#endif // NEDRYSOFT_PINGNOO_CONSTANTS_H
