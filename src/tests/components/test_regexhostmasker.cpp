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

#include "ComponentSystem/ComponentLoader.h"
#include "ComponentSystem/IComponentManager.h"
#include "Core/IHostMasker.h"
#include "catch.hpp"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>

QByteArray fileContent(QString filename) {
    QFile file(filename);

    if (file.open(QFile::ReadOnly)) {
        return file.readAll();
    }

    return QByteArray();
}

TEST_CASE("RegExHostMasker Tests", "[app][components][network]") {
    SECTION("check host masking") {
        Nedrysoft::ComponentSystem::ComponentLoader componentLoader;
        QString hostName, hostAddress, maskedHostName, maskedHostAddress;
        bool didMatchMask;

        componentLoader.addComponents(PINGNOO_TEST_COMPONENTS_DIR);

        componentLoader.loadComponents();

        Nedrysoft::Core::IHostMasker *regExHostMasker = nullptr;

        for (auto hostMasker : Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IHostMasker>()) {
            if (QString::fromLatin1(hostMasker->metaObject()->className())=="Nedrysoft::RegExHostMasker::RegExHostMasker") {
                regExHostMasker = hostMasker;
                break;
            }
        }

        REQUIRE_MESSAGE(regExHostMasker!=nullptr, "Unable to find Nedrysoft::RegExHostMasker::RegExHostMasker");

        regExHostMasker->loadConfiguration(QJsonDocument::fromJson(fileContent(":/test_regexhostmasker_host.json")).object());

        hostName = "1.123-123-123.static.test.co.uk";
        hostAddress = "1.123.123.123";

        didMatchMask = regExHostMasker->mask(0, hostName, hostAddress, maskedHostName, maskedHostAddress);

        REQUIRE_MESSAGE(didMatchMask==true, "Masking did not occur when it should have.");
        REQUIRE_MESSAGE(maskedHostName.toStdString()==std::string("<hidden>.static.test.co.uk"), "Host name was incorrectly masked.");
        REQUIRE_MESSAGE(maskedHostAddress.toStdString()==std::string("1.123.123.123"), "Host address was masked and shouldn't have been.");

        hostName = "1.123-123-123.static.test2.co.uk";
        hostAddress = "1.123.123.123";

        didMatchMask = regExHostMasker->mask(0, hostName, hostAddress, maskedHostName, maskedHostAddress);

        REQUIRE_MESSAGE(didMatchMask==false, "Masking occured when it shouldn't have.");

        REQUIRE_MESSAGE(maskedHostName.toStdString()==std::string("1.123-123-123.static.test2.co.uk"), "Host name was masked and shouldn't have been.");
        REQUIRE_MESSAGE(maskedHostAddress.toStdString()==std::string("1.123.123.123"), "Host address was masked and shouldn't have been.");
    }

    SECTION("check address masking") {
        Nedrysoft::ComponentSystem::ComponentLoader componentLoader;
        QString hostName, hostAddress, maskedHostName, maskedHostAddress;
        bool didMatchMask;

        componentLoader.addComponents("Components");

        componentLoader.loadComponents();

        Nedrysoft::Core::IHostMasker *regExHostMasker = nullptr;

        for (auto hostMasker : Nedrysoft::ComponentSystem::getObjects<Nedrysoft::Core::IHostMasker>()) {
            if (QString::fromLatin1(hostMasker->metaObject()->className())=="Nedrysoft::RegExHostMasker::RegExHostMasker") {
                regExHostMasker = hostMasker;
                break;
            }
        }

        REQUIRE_MESSAGE(regExHostMasker!=nullptr, "Unable to find Nedrysoft::RegExHostMasker::RegExHostMasker");

        regExHostMasker->loadConfiguration(QJsonDocument::fromJson(fileContent(":/test_regexhostmasker_address.json")).object());

        hostName = "1.123-123-123.static.test.co.uk";
        hostAddress = "1.123.123.123";

        didMatchMask = regExHostMasker->mask(0, hostName, hostAddress, maskedHostName, maskedHostAddress);

        REQUIRE_MESSAGE(didMatchMask==true, "Masking did not occur when it should have.");
        REQUIRE_MESSAGE(maskedHostName.toStdString()==std::string("1.123-123-123.static.test.co.uk"), "Host name was masked and it shouldn't have been.");
        REQUIRE_MESSAGE(maskedHostAddress.toStdString()==std::string("<hidden>"), "Host address was incorrectly masked.");

        hostName = "1.123-123-123.static.test2.co.uk";
        hostAddress = "1.123.123.123";

        didMatchMask = regExHostMasker->mask(0, hostName, hostAddress, maskedHostName, maskedHostAddress);

        REQUIRE_MESSAGE(didMatchMask==false, "Masking occured when it shouldn't have.");
        REQUIRE_MESSAGE(maskedHostName.toStdString()==std::string("1.123-123-123.static.test2.co.uk"), "Host name was masked and shouldn't have been.");
        REQUIRE_MESSAGE(maskedHostAddress.toStdString()==std::string("1.123.123.123"), "Host address was masked and shouldn't have been.");
    }
}
