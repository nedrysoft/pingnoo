#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
# An open source ping path analyser
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

include(../../application.pri)

TARGET = Pingnoo_tests

QT += \
    testlib

QT -= \
    gui

QT += \
    core \

CONFIG += \
    warn_on \
    depend_includepath

INCLUDEPATH += \
    $${APPLICATION_BUILD_ROOT}/src/libs/Catch2

DESTDIR = $${APPLICATION_BINARY_FOLDER}

SOURCES += \
    libs/test_componentsystem.cpp \
    components/test_regexhostmasker.cpp \
    main.cpp \
    libs/test_icmppacket.cpp

LIBS += \
    -L$${APPLICATION_BINARY_FOLDER}/ \
    -L$${APPLICATION_BINARY_FOLDER}/Components/

CONFIG -= \
    app_bundle create_prl

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,$${TARGET_SUFFIX})
}

LIBS += \
    $$COMPONENT(Core) \
    $$STATICLIBRARY(ComponentSystem) \
#    $$COMPONENT(RegExHostMasker) \
    $$STATICLIBRARY(ICMPPacket) \
    $$STATICLIBRARY(Ribbon)

macx {
    QMAKE_CXXFLAGS += \
        -Wno-deprecated-declarations
}

linux {
    QMAKE_CXXFLAGS += \
        -Wno-deprecated-declarations

    QMAKE_LFLAGS += \
        -Wl,--rpath=.
}

HEADERS += \
    libs/test_componentsystem.h

RESOURCES += \
    testresources.qrc

DISTFILES += \
    components/test_regexhostmasker_address.json \
    components/test_regexhostmasker_host.json



