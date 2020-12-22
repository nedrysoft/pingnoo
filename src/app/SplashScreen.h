/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 01/12/2020.
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

#ifndef NEDRYSOFT_SPLASHSCREEN_H
#define NEDRYSOFT_SPLASHSCREEN_H

#include <QSplashScreen>

namespace Nedrysoft {
    /**
    * @brief       The SplashScreen dialog for the application.
    */
    class SplashScreen :
            public QSplashScreen {

        private:
            Q_OBJECT

        private:
            /**
             * @brief       Constructs a new SplashScreen instance.
             *
             * @note        The constructor is private as SplashScreen is a singleton class, to obtain the SplashScreen
             *              use the getInstance() function.
             */
            SplashScreen();

            /**
             * @brief       Destroys the SplashScreen.
             */
            ~SplashScreen() override;

            /**
             * @brief       Delete the copy constructor.
             */
            SplashScreen(const SplashScreen &) = delete;

            /**
             * @brief       Delete the assignment operator.
             */
            SplashScreen &operator=(const SplashScreen &) = delete;

        public:
            /**
             * @brief           Returns the instance of the SplashScreen class.
             *
             * @returns         the SplashScreen instance.
             */
            static auto getInstance() -> SplashScreen *;

        protected:
            /**
             * @brief           Draw the contents of splash screen.
             *
             * @details         Draws the splash screen, uses the default implementation and then overlays version
             *                  information over the top.
             *
             * @param[in]       painter the painter to draw to.
             */
            auto drawContents(QPainter *painter) -> void override;

        private:
            float m_scaleFactor;
    };
}

#endif // NEDRYSOFT_SPLASHSCREEN_H
