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

#include "test_componentsystem.h"
#include "catch.hpp"
#include "ComponentSystem/ComponentLoader.h"
#include "ComponentSystem/Component.h"
#include "ComponentSystem/IComponentManager.h"
#include "Core/CoreComponent.h"
#include <QString>
#include <QObject>

TEST_CASE("ComponentSystem Tests", "[app][libs][componentsystem]")
{
    SECTION("check core component") {
        FizzyAde::ComponentSystem::ComponentLoader componentLoader;
        QMap<QString, FizzyAde::ComponentSystem::Component *> loadedComponents;

        componentLoader.addComponents("Components");

        componentLoader.loadComponents();

        REQUIRE_MESSAGE(componentLoader.components().count()!=0, "No components were loaded.");

        for (auto component : componentLoader.components())
            loadedComponents[component->name()] = component;

        REQUIRE_MESSAGE(loadedComponents.contains("Core")==true, "Core component was not loaded.");
        REQUIRE_MESSAGE(loadedComponents["Core"]->loadError()==0, "Core component loaded with errors.");
    }

    SECTION("check component manager") {
        REQUIRE_MESSAGE(FizzyAde::ComponentSystem::allObjects().count()==0, "Objects found in the registry when there should be none.");

        FizzyAde::ComponentSystem::addObject(new QObject);

        REQUIRE_MESSAGE(FizzyAde::ComponentSystem::allObjects().count()==1, "There should be one object in the registry.");

        REQUIRE_MESSAGE(FizzyAde::ComponentSystem::getObjects<QObject>().count()==1, "There should be one QObject in the registry.");

        REQUIRE_MESSAGE(FizzyAde::ComponentSystem::getObject<QObject>()!=nullptr, "Unable to get QObject from the registry.");

        FizzyAde::ComponentSystem::addObject(new TestObject);

        REQUIRE_MESSAGE(FizzyAde::ComponentSystem::allObjects().count()==2, "There should be one two objects in the registry.");;

        REQUIRE_MESSAGE(FizzyAde::ComponentSystem::getObjects<QObject>().count()==2, "There should be two QObjects in the registry.");

        REQUIRE_MESSAGE(FizzyAde::ComponentSystem::getObjects<TestObject>().count()==1, "There should be a TestObject in the registry.");
    }
}
