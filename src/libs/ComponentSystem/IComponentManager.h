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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTMANAGER_H
#define NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTMANAGER_H

#include "ComponentSystemSpec.h"
#include "IComponentInterface.h"
#include <QObject>

namespace Nedrysoft::ComponentSystem {
    /**
     * @brief       IComponentManager
     *
     * @details     Singleton instance of the component manager, manages the storing of
     *              objects for any loaded component in a global "registry".
     *
     */
    class COMPONENT_SYSTEM_DLLSPEC IComponentManager :
            public QObject {
        Q_OBJECT

        private:
            /**
             * @brief       `Constructor
             *
             */
            IComponentManager() = default;

            /**
             * @brief       Destructor
             *
             */
            ~IComponentManager();

        public:

            /**
             * @brief       Add an object to the object registry
             *
             * @param[in]   object object to store
             *
             */
            void addObject(QObject *object);

            /**
             * @brief       Removes an object to the object registry
             *
             * @param[in]   object object to store
             *
             */
            void removeObject(QObject *object);

            /**
             * @brief       Return all objects
             *
             * @returns     returns a list of all objects
             *
             */
            QList<QObject *> allObjects();

            /**
             * @brief       Returns the singleton instance to the ComponentManager object
             *
             * @returns     the singleton instance
             *
             */
            static IComponentManager *getInstance();

        private:
            QList<QObject *> m_objectList;                              //! The list of objects
    };
}

/**
 * @brief       Convenience functions to manipulate the object registry
 *
 * @example     Nedrysoft::ComponentSystem:addObject(object);
 *
 *              QList<QObject *> objectList = Nedrysoft::ComponentSystem:allObjects();
 *
 *              auto object = Nedrysoft::ComponentSystem:getObject<IInterface>();
 *
 *              QList<IInterface *> objectList = Nedrysoft::ComponentSystem:getObjects<IInterface>();
 *
 */
namespace Nedrysoft::ComponentSystem {
    /**
     * @brief       Adds an object to the registry
     *
     * @param[in]   object      the object
     *
     */
    inline void addObject(QObject *object) {
        return IComponentManager::getInstance()->addObject(object);
    }

    /**
     * @brief       Removes an object to the registry
     *
     * @param[in]   object      the object
     *
     */
    inline void removeObject(QObject *object) {
        return IComponentManager::getInstance()->removeObject(object);
    }

    /**
     * @brief       Return all registered objects
     *
     * @return      the list of objects
     *
     */
    inline QList<QObject *> allObjects() {
        return IComponentManager::getInstance()->allObjects();
    }

    /**
     * @brief       Return the first matching object of type T
     *
     * @return      the object of type T
     *
     */
    template<typename T>
    inline T *getObject() {
        for (auto object : IComponentManager::getInstance()->allObjects()) {

            auto castObject = qobject_cast<T *>(object);

            if (castObject)
                return castObject;
        }

        return nullptr;
    }

    /**
     * @brief       Returns all objects that implement type T
     *
     * @return      the list of objects implementing type T
     *
     */
    template<typename T>
    inline QList<T *> getObjects() {
        QList<T *> objectList;

        for (auto object : IComponentManager::getInstance()->allObjects()) {

            auto castObject = qobject_cast<T *>(object);

            if (castObject)
                objectList.append(castObject);
        }

        return objectList;
    }
}

#endif // NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTMANAGER_H
