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

#include "test_componentsystem.h"

#include "ComponentSystem/Component.h"
#include "ComponentSystem/ComponentLoader.h"
#include "ComponentSystem/IComponentManager.h"
#include "Core/CoreComponent.h"
#include "catch.hpp"

#include <QObject>
#include <QString>

TEST_CASE("ComponentSystem Tests", "[app][libs][componentsystem]") {
    SECTION("check core component") {
        Nedrysoft::ComponentSystem::ComponentLoader componentLoader;
        QMap<QString, Nedrysoft::ComponentSystem::Component *> loadedComponents;

        componentLoader.addComponents(PINGNOO_TEST_COMPONENTS_DIR);

        componentLoader.loadComponents([=](Nedrysoft::ComponentSystem::Component *component)->bool {
            if (component->name()=="Nedrysoft::Core") {
                return true;
            }

            return false;
        });

        REQUIRE_MESSAGE(componentLoader.components().count()!=0, "No components were loaded.");

        for (auto component : componentLoader.components()) {
            loadedComponents[component->name()] = component;
        }

        REQUIRE_MESSAGE(loadedComponents.contains("Core")==true, "Core component was not loaded.");

        REQUIRE_MESSAGE(loadedComponents["Core"]->loadStatus()!=Nedrysoft::ComponentSystem::ComponentLoader::Loaded,
                "Core component loaded with errors.");
    }

    SECTION("check component manager") {
        REQUIRE_MESSAGE (Nedrysoft::ComponentSystem::allObjects().count()==0, "Objects found in the registry when there should be none.");

        Nedrysoft::ComponentSystem::addObject(new QObject);

        REQUIRE_MESSAGE (Nedrysoft::ComponentSystem::allObjects().count()==1, "There should be one object in the registry.");

        REQUIRE_MESSAGE (Nedrysoft::ComponentSystem::getObjects<QObject>().count()==1, "There should be one QObject in the registry.");

        REQUIRE_MESSAGE (Nedrysoft::ComponentSystem::getObject<QObject>()!=nullptr, "Unable to get QObject from the registry.");

        Nedrysoft::ComponentSystem::addObject(new TestObject);

        REQUIRE_MESSAGE (Nedrysoft::ComponentSystem::allObjects().count()==2, "There should be one two objects in the registry.");;

        REQUIRE_MESSAGE (Nedrysoft::ComponentSystem::getObjects<QObject>().count()==2, "There should be two QObjects in the registry.");

        REQUIRE_MESSAGE (Nedrysoft::ComponentSystem::getObjects<TestObject>().count()==1, "There should be a TestObject in the registry.");
    }
}
