#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 27/03/2020.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

pingnoo_start_executable(Pingnoo)

set(MACOSX_BUNDLE_GUI_IDENTIFIER com.nedrysoft.pingnoo)

pingnoo_add_sources(
    App.qrc
    AppIcon.icns
    AppIcon.rc
    SplashScreen.cpp
    SplashScreen.h
    WindowsIcon.qrc
    main.cpp
)

pingnoo_use_qt_libraries(Widgets)

pingnoo_use_shared_library(ComponentSystem)

pingnoo_add_optional_command("Linux" SetRawCapabilities "Set RAW SOCKET capabilities" ON POST_BUILD COMMAND sudo -n /usr/sbin/setcap cap_net_raw,cap_net_admin=eip ${PINGNOO_BINARY_ROOT}/${pingnooCurrentProjectName})

pingnoo_use_frameworks("Foundation" "Cocoa")
pingnoo_use_system_libraries(APPLE stdc++ objc)

pingnoo_end_executable()


