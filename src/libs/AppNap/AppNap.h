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
    /**
     * @brief       Provides control over sleep modes
     *
     * @details     macOS uses a sleep mode system which results in pings not working correctly if
     *              AppNap is enabled, this class under macOS disables AppNap on startup so that
     *              the app does not go to sleep.  On other platforms it does nothing.
     *
     */
    class FIZZYADE_APPNAP_DLLSPEC AppNap
    {
        private:
            AppNap();

        public:
            /**
             * @brief       Gets the AppNap singleton
             *
             * @return      the singleton instance
             *
             */
            static FizzyAde::AppNap::AppNap *getInstance();

            /**
             * @brief       Prevents app from idle napping
             *
             * @details     prevents the app from entering idle state, calls to prevent
             *              should be matched with a allow to allow the app to enter nap
             *              after the section.
             *
             * @param[in]   reason          reason for preventing app nap
             *
             */
            void prevent(const QString &reason);

            /**
             * @brief       Allows the app to enter idle nap
             *
             * @details     allows the app to enter the idle nap state
             *
             */
            void allow();

#if defined(Q_OS_MACOS)
        private:
            /**
             * @brief       initialise app nap
             *
             * @details     provides a "C" linkage for the objective c code which controls appnap
             *
             */
            void mac_init();

            /**
             * @brief       prevent app nap from being used
             *
             * @details     provides a "C" linkage for the objective c code which disables appnap
             *
             */
            void mac_prevent(const QString &reason);

            /**
             * @brief       allow app nap to be used
             *
             * @details     provides a "C" linkage for the objective c code which enables appnap
             *
             */
            void mac_allow();

        private:
            QRecursiveMutex m_mutex;
            NSObject *m_activity = {};
            int m_count = {};
#endif
    };
}

#endif // FIZZYADE_UTILS_APPNAP_H
