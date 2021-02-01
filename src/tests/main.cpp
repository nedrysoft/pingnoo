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

#define CATCH_CONFIG_RUNNER

#include "catch.hpp"

#include <QApplication>
#include <QtTest>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTEST_SET_MAIN_SOURCE_PATH

    spdlog::set_level(spdlog::level::debug);

    auto result = Catch::Session().run(argc, argv);

    return ( result < 0xff ? result : 0xff );
}
