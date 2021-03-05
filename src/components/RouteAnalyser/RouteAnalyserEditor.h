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

#ifndef NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSEREDITOR_H
#define NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSEREDITOR_H

#include "ComponentSystem/IInterface.h"
#include "Core/Core.h"
#include "Core/IConfiguration.h"
#include "Core/IEditor.h"
#include "LatencyRibbonGroup.h"

#include <QObject>

namespace Nedrysoft::Core {
    class IPingEngineFactory;
}

namespace Nedrysoft::RouteAnalyser {
    class RouteAnalyserComponent;
    class RouteAnalyserWidget;

    /**
     * @brief       The RouteAnalyserEditor class provides the editor for a route analysis.
     */
    class RouteAnalyserEditor :
            public Nedrysoft::Core::IEditor,
            public Nedrysoft::Core::IConfiguration {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::Core::IEditor)

            Q_INTERFACES(Nedrysoft::Core::IConfiguration)

        public:
            /**
             * @brief       Constructs a new RouteAnalyserEditor.
             */
            RouteAnalyserEditor();

            /**
             * @brief       Destroys the RouteAnalyserEditor.
             */
            ~RouteAnalyserEditor();

            /**
             * @brief       Sets the ping engine to be used by this instance.
             *
             * @param[in]   pingEngineFactory the ping engine factory.
             */
            auto setPingEngine(Nedrysoft::Core::IPingEngineFactory *pingEngineFactory) -> void;

            /**
             * @brief       Sets the ping target.
             *
             * @param[in]   target the ping target.
             */
            auto setTarget(QString target) -> void;

            /**
             * @brief       Sets the IP Version used by this ping target.
             *
             * @param[in]   ipVersion the IP version.
             */
            auto setIPVersion(Nedrysoft::Core::IPVersion ipVersion) -> void;

            /**
             * @brief       Sets the ping interval used by this ping target.
             *
             * @param[in]   interval the interval time period.
             */
            auto setInterval(double interval) -> void;

        public:
            /**
             * @brief       Returns the widget for the editor.
             *
             * @see         Nedrysoft::Core::IEditor::widget
             *
             * @returns     the widget.
             */
            auto widget() -> QWidget * override;

            /**
             * @brief       Returns the display name for the editor.
             *
             * @details     Returns the user displayable name of the editor, this is usually used by the
             *              Nedrysoft::Core::IEditorManager to display the name of the widget in the title.
             *
             * @see         Nedrysoft::Core::IEditor::displayName
             *
             * @returns     the displayed name of the editor.
             */
            auto displayName() -> QString override;

            /**
             * @brief       The editor manager calls this method when an editor is activated. (i.e becomes the active
             *              editor)
             */
            auto activated() -> void override;

            /**
             * @brief       The editor manager calls this method when an editor is deactivated. (i.e focus is lost)
             *              editor)
             */
            auto deactivated() -> void override;

        public:
            /**
             * @brief       Saves the configuration to a JSON object.
             *
             * @see         Nedrysoft::Core::IConfiguration::saveConfiguration
             *
             * @returns     the JSON configuration.
             */
            auto saveConfiguration() -> QJsonObject override;

            /**
             * @brief       Loads the configuration.
             *
             * @see         Nedrysoft::Core::IConfiguration::loadConfiguration
             *
             * @param[in]   configuration the configuration as JSON object.
             *
             * @returns     true if loaded; otherwise false.
             */
            auto loadConfiguration(QJsonObject configuration) -> bool override;

            friend class RouteAnalyserWidget;

        private:
            /**
             * @brief       Called when the graph dataset has changed.
             *
             * @param[in]   start the epoch of the dataset.
             * @param[in]   end the end of the dataset.
             */
            void onDatasetChanged(double start, double end);

            /**
             * @brief       Called when the start and/or end of the ribbon viewport has changed.
             *
             * @param[in]   start the new start position.
             * @param[in]   end the new end position.
             */
            void onViewportChanged(double start, double end);

            /**
             * @brief       Called when the size of the ribbon viewport has changed.
             *
             * @param[in]   size the new size of the viewport.
             */
            void onViewportWindowChanged(double size);

            /**
             * @brief       Called when one of the latency values has been changed.
             *
             * @param[in]   type the type of latency value that was changed.
             * @param[in]   value the new value for the given latency type.
             */
            void onLatencyValueChanged(LatencyRibbonGroup::LatencyType type, double value);

        protected:

            int m_contextId;
            Nedrysoft::Core::IPingEngineFactory *m_pingEngineFactory;
            QString m_pingTarget;
            Nedrysoft::Core::IPVersion m_ipVersion;
            double m_interval;
            Nedrysoft::RouteAnalyser::RouteAnalyserWidget *m_editorWidget;
            double m_viewportStart;
            double m_viewportEnd;
            QMetaObject::Connection m_dataChangedConnection;
    };
}

#endif // NEDRYSOFT_ROUTEANALYSER_ROUTEANALYSEREDITOR_H
