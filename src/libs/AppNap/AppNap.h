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

#ifndef FIZZYADE_APPNAP_APPNAP_H
#define FIZZYADE_APPNAP_APPNAP_H

#include <QtGlobal>
#include <QObject>
#include <QRecursiveMutex>

#if (defined(FIZZYADE_LIBRARY_APPNAP_EXPORT))
#define FIZZYADE_APPNAP_DLLSPEC Q_DECL_EXPORT
#else
#define FIZZYADE_APPNAP_DLLSPEC Q_DECL_IMPORT
#endif

#if defined(Q_OS_MACOS)
Q_FORWARD_DECLARE_OBJC_CLASS(NSObject);
#endif

namespace FizzyAde::AppNap
{
    class FIZZYADE_APPNAP_DLLSPEC AppNap
    {
        private:
            AppNap();

        public:
            /**
             * Gets the AppNap singleton
             *
             * @return the singleton instance
             */
            static FizzyAde::AppNap::AppNap *getInstance();

            /**
             * Prevents app from idle napping
             *
             * prevents the app from entering idle state, calls to prevent
             * should be matched with a allow to allow the app to enter nap
             * after the section.
             *
             * @param[in] reason reason for preventing app nap
             */
            void prevent(const QString &reason);

            /**
             * Allows the app to enter idle nap
             *
             * allows the app to enter the idle nap state
             */
            void allow();

#if defined(Q_OS_MACOS)
        private:
            void mac_init();
            void mac_prevent(const QString &reason);
            void mac_allow();

        private:
            QRecursiveMutex m_mutex;
            NSObject *m_activity = {};
            int m_count = {};
#endif
    };
}

#endif // FIZZYADE_UTILS_APPNAP_H
