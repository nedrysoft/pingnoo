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

#ifndef FIZZYADE_ROUTEANALYSER_ROUTEANALYSEREDITOR_H
#define FIZZYADE_ROUTEANALYSER_ROUTEANALYSEREDITOR_H

#include "ComponentSystem/IInterface.h"
#include "Core/IEditor.h"
#include "Core/IConfiguration.h"
#include "Core/Core.h"
#include <QObject>

namespace FizzyAde::Core
{
    class IPingEngineFactory;
}

namespace FizzyAde::RouteAnalyser
{
    class RouteAnalyserComponent;

    /**
     * Definition for the built in host masker
     *
     * This host marker accepts a regular expression to match the host name
     * or address and allows the masked output to be generated using capture
     * groups
     *
     */

    class RouteAnalyserEditor :
        public FizzyAde::Core::IEditor,
        public FizzyAde::Core::IConfiguration
    {
        Q_OBJECT

        Q_INTERFACES(FizzyAde::Core::IEditor)
        Q_INTERFACES(FizzyAde::Core::IConfiguration)

    public:
        RouteAnalyserEditor(int contextId);

        /**
         * Sets the ping engine to be used by this instance.
         *
         * @param[in]   pingEngineFactory       the ping engine factory
         *
         */
        void setPingEngine(FizzyAde::Core::IPingEngineFactory *pingEngineFactory);

        /**
         * Sets the target.
         *
         * @param[in]   target                  the ping target
         *
         */
        void setTarget(QString target);

        /**
         * Sets the IP Version used by this ping target.
         *
         * @param[in]   ipVersion               the IP version
         *
         */
        void setIPVersion(FizzyAde::Core::IPVersion ipVersion);

        /**
         * Sets the ping interval used by this ping target.
         *
         * @param[in]   interval                the interval time period
         *
         */
        void setInterval(double interval);

        /**
         * @sa IEditor
         *
         */
        virtual QWidget *widget();
        virtual QString displayName();

        /**
         * @sa IConfiguration
         *
         */
        virtual QJsonObject saveConfiguration();
        virtual bool loadConfiguration(QJsonObject configuration);

        friend class RouteAnalyserWidget;

    protected:

        int m_contextId;
        FizzyAde::Core::IPingEngineFactory *m_pingEngineFactory;
        QString m_pingTarget;
        FizzyAde::Core::IPVersion m_ipVersion;
        double m_interval;

    };
}

#endif // FIZZYADE_ROUTEANALYSER_ROUTEANALYSEREDITOR_H
