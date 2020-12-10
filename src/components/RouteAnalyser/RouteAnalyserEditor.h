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

#ifndef NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSEREDITOR_H
#define NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSEREDITOR_H

#include "ComponentSystem/IInterface.h"
#include "Core/Core.h"
#include "Core/IConfiguration.h"
#include "Core/IEditor.h"

#include <QObject>

namespace Nedrysoft::Core {
    class IPingEngineFactory;
}

namespace Nedrysoft::RouteAnalyser {
    class RouteAnalyserComponent;
    class RouteAnalyserWidget;

    /**
     * @brief       Definition for the built in host masker
     *
     * @details     This host marker accepts a regular expression to match the host name
     *              or address and allows the masked output to be generated using capture
     *              groups
     */
    class RouteAnalyserEditor :
            public Nedrysoft::Core::IEditor,
            public Nedrysoft::Core::IConfiguration {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IEditor)

            Q_INTERFACES(Nedrysoft::Core::IConfiguration)

        public:
            RouteAnalyserEditor(int contextId);

            ~RouteAnalyserEditor();

            /**
             * @brief       Sets the ping engine to be used by this instance.
             *
             * @param[in]   pingEngineFactory the ping engine factory
             */
            void setPingEngine(Nedrysoft::Core::IPingEngineFactory *pingEngineFactory);

            /**
             * @brief       Sets the target.
             *
             * @param[in]   target the ping target
             *
             */
            void setTarget(QString target);

            /**
             * @brief       Sets the IP Version used by this ping target.
             *
             * @param[in]   ipVersion the IP version
             *
             */
            void setIPVersion(Nedrysoft::Core::IPVersion ipVersion);

            /**
             * @brief       Sets the ping interval used by this ping target.
             *
             * @param[in]   interval the interval time period
             */
            void setInterval(double interval);

            /**
             * @sa          IEditor
             */
            virtual QWidget *widget();

            virtual QString displayName();

            /**
             * @sa          IConfiguration
             */
            virtual QJsonObject saveConfiguration();

            virtual bool loadConfiguration(QJsonObject configuration);

            friend class RouteAnalyserWidget;

        protected:

            int m_contextId;
            Nedrysoft::Core::IPingEngineFactory *m_pingEngineFactory;
            QString m_pingTarget;
            Nedrysoft::Core::IPVersion m_ipVersion;
            double m_interval;
            Nedrysoft::RouteAnalyser::RouteAnalyserWidget *m_editorWidget;

    };
}

#endif // NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSEREDITOR_H
