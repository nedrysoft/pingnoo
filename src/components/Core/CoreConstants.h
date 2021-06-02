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

namespace Nedrysoft { namespace Core { namespace Constants {
    namespace MenuBars {
        constexpr auto Application = "Pingnoo.Application";
    };

    namespace Menus {
        constexpr auto File = "Pingnoo.File";
        constexpr auto Edit = "Pingnoo.Edit";
        constexpr auto Help = "Pingnoo.Help";
        constexpr auto Application = "Pingnoo.Application";

        const QMap<QString, QString> MAP = {
            {File, QT_TR_NOOP("File")},
            {Edit, QT_TR_NOOP("Edit")},
            {Help, QT_TR_NOOP("Help")},
            {Application, QT_TR_NOOP("Application")}
        };
    }

    namespace MenuGroups {
        constexpr auto Top = "Pingnoo.Top";
        constexpr auto Middle = "Pingnoo.Middle";
        constexpr auto Bottom = "Pingnoo.Bottom";

        constexpr auto FileNew = "File.New";
        constexpr auto FileOpen = "File.Open";
        constexpr auto FileSave = "File.Save";
        constexpr auto FileMisc = "File.Misc";
        constexpr auto FileExit = "File.Exit";
    }

    namespace Commands {
        constexpr auto Preferences = "Pingnoo.Preferences";

        constexpr auto Open = "Pingnoo.Open";
        constexpr auto Quit = "Pingnoo.Quit";

        constexpr auto Cut = "Pingnoo.Cut";
        constexpr auto Copy = "Pingnoo.Copy";
        constexpr auto Paste = "Pingnoo.Paste";

        constexpr auto About = "Pingnoo.About";

        constexpr auto AboutComponents = "Pingnoo.AboutComponents";

        constexpr auto ShowApplication = "Pingnoo.ShowApplication";
        constexpr auto HideApplication = "Pingnoo.HideApplication";

        const QMap<QString, QString> MAP = {
            {Cut, QT_TR_NOOP("Preferences")},

            {Cut, QT_TR_NOOP("Cut")},
            {Copy, QT_TR_NOOP("Copy")},
            {Paste,QT_TR_NOOP("Paste")},

            {Open, QT_TR_NOOP("Open...")},
            {Quit, QT_TR_NOOP("Exit")},

            {About, QT_TR_NOOP("About Pingnoo")},
            {AboutComponents, QT_TR_NOOP("About Components")},

            {ShowApplication, QT_TR_NOOP("Show Application")},
            {HideApplication, QT_TR_NOOP("Hide Application")}
        };
    };

    namespace RibbonPages {
        constexpr auto Home = "Pingnoo.Home";
        constexpr auto HostMasking = "Pingnoo.HostMasking";
    };

    inline QString menuText(const QString &string) {
        if (Menus::MAP.contains(string))
            return QObject::tr(Menus::MAP[string].toUtf8());

        return string;
    }

    inline QString commandText(const QString &string) {
        if (Commands::MAP.contains(string))
            return QObject::tr(Commands::MAP[string].toUtf8());

        return string;
    }
}}}

#endif // NEDRYSOFT_PINGNOO_CONSTANTS_H
