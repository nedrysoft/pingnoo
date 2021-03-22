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

#ifndef NEDRYSOFT_ROUTEANALYSER_PINGDATA_H
#define NEDRYSOFT_ROUTEANALYSER_PINGDATA_H

#include "Core/PingResult.h"

#include <QPersistentModelIndex>
#include <QString>
#include <QVariant>
#include <cmath>

class QCustomPlot;

class QTableView;

class QStandardItemModel;

namespace Nedrysoft::RouteAnalyser {
    class RouteItemTableDelegate;
    class IPlot;

    typedef std::chrono::duration<double, std::ratio<1, 1000>> milliseconds_double;
    typedef std::chrono::duration<double, std::ratio<1, 1>> seconds_double;

    /**
     * @brief       The PingData class is used to store data for a table model.
     *
     * @details     Holds data about each hop and updates the route table when the object is updated.
     */
    class PingData {
        public:
            /**
             * @brief       Column ID's. (columns appear in this order)
             */
            enum class Fields {
                Hop,
                Count,
                IP,
                HostName,
                Location,
                AverageLatency,
                MinimumLatency,
                MaximumLatency,
                CurrentLatency,
                PacketLoss,
                Graph,

                HistoricalLatency = 100
            };

        public:
            /**
             * @brief       Constructs a new PingData instance.
             */
            PingData() = default;

            /**
             * @brief       Destroys the PingData.
             */
            ~PingData() = default;

            /**
             * @brief       Constructs a new PingData instance copying another PingData.
             */
            PingData(const PingData &) = default;

            /**
             * @brief       Copy operator.
             */
            PingData &operator=(const PingData &) = default;

            /**
             * @brief       Constructs a new PingData and adds the item to a model.
             *
             * @details     Creates an entry in the given data table and initialises the row
             *              with default information.
             *
             * @param[in]   tableModel the table model.
             * @param[in]   hop the hop number of this item.
             * @param[in]   hopValid true if the hop responds to ping; otherwise false.
             */
            PingData(QStandardItemModel *tableModel, int hop, bool hopValid);

            /**
             * @brief       Sets the historical latency for this point.
             *
             * @details     This can optionally be drawn on the latency graph when hovering
             *              over a chart.
             *
             * @param[in]   latency the latency.
             */
            auto setHistoricalLatency(std::chrono::duration<double> latency) -> void;

            /**
             * @brief       Updates the route table item with the given result.
             *
             * @param[in]   result the ping result for this hop.
             */
            auto updateItem(Nedrysoft::Core::PingResult result) -> void;

            /**
             * @brief       Sets the hop number for this item.
             *
             * @param[in]   hop the hop number.
             */
            auto setHop(int hop) -> void;

            /**
             * @brief       Returns the hop number for this route item.
             *
             * @returns     the hop number.
             */
            auto hop() -> int;

            /**
             * @brief       Sets the displayed ip address for this route item.
             *
             * @param[in]   hostAddress address for this hop.
             */
            auto setHostAddress(const QString &hostAddress) -> void;

            /**
             * @brief       Returns the displayed ip address for this route item.
             *
             * @returns     the address for this hop.
             */
            auto hostAddress() -> QString;

            /**
             * @brief       Sets the displayed host name for this route item.
             *
             * @param[in]   hostName the host name.
             */
            auto setHostName(const QString &hostName) -> void;

            /**
             * @brief       Returns the displayed host name for this route item.
             *
             * @returns     the host name.
             */
            auto hostName() -> QString;

            /**
             * @brief       Sets the location position.
             *
             * @param[in]   location the location.
             */
            auto setLocation(const QString &location) -> void;

            /**
             * @brief       Returns the displayed location.
             *
             * @returns     the location.
             */
            auto location() -> QString;

            /**
             * @brief       Sets the graph associated with this route item.
             *
             * @param[in]   customPlot the plot.
             */
            auto setCustomPlot(QCustomPlot *customPlot) -> void;

            /**
             * @brief       Returns the graph associated with this route item.
             *
             * @returns     the plot.
             */
            auto customPlot() -> QCustomPlot *;

            /**
             * @brief       Returns whether this hop is valid.
             *
             * @returns     true if valid; otherwise false.
             */
            auto hopValid() -> bool;

            /**
             * @brief       Sets the valid state for this hop.
             *
             * @param[in]   hopValid whether the hop is valid.
             */
            auto setHopValid(bool hopValid) -> void;

            /**
             * @brief       Returns the latency.
             *
             * @param[in]   field which latency to retrieve.
             *
             * @returns     the latency.
             */
            auto latency(int field) -> double;

            /**
             * @brief       Returns the packet loss %.
             *
             * @returns     the packet loss.
             */
            auto packetLoss() -> double;

            /**
             * @brief       Sets the plots associated with this.
             *
             * @param[in]   plots the plots.
             */
            auto setPlots(QList<Nedrysoft::RouteAnalyser::IPlot *> plots) -> void;

        protected:
            /**
             * @brief       Calculates a running average.
             *
             * @details     Used to calculate the running average of the latency, does not store previous values.
             *
             * @param[in]   previousAverage the current average.
             * @param[in]   value the new value to add to the average.
             * @param[in]   n the number of values (including the new one) used.
             *
             * @returns     the average.
             */
            static auto runningAverage(double previousAverage, double value, double n) -> double;

            /**
             * @brief       Updates the model so that views refresh.
             */
            auto updateModel() -> void;

            /**
             * @brief       Returns the table model associated with this item.
             *
             * @returns     table model.
             */
            auto tableModel() -> QStandardItemModel *;

            /**
             * @brief       Returns the number of samples sent.
             *
             * @returns     sample count.
             */
            auto count() -> unsigned long ;

            friend class RouteTableItemDelegate;

        private:
            QStandardItemModel *m_tableModel;                   //! The table model
            QCustomPlot *m_customPlot;                          //! The graph
            QCustomPlot *m_jitterPlot;                          //! The graph
            QPersistentModelIndex m_modelIndex;                 //! Model index in the route table of this item

            unsigned long m_replyPacketCount;                   //! Total number of replies received
            unsigned long m_timeoutPacketCount;                 //! Total number of timeouts

            int m_hop;                                          //! The hop number
            bool m_hopValid;                                    //! Whether this hop is valid
            unsigned long m_count;                              //! Total number of samples sent

            QString m_hostAddress;                              //! The displayed ip address
            QString m_hostName;                                 //! The displayed host name
            QString m_location;                                 //! The Geo location of the host

            seconds_double m_currentLatency = {};               //! The current (last) received latency value (in seconds)
            seconds_double m_maximumLatency = {};               //! The maximum latency value (in seconds)
            seconds_double m_minimumLatency = {};               //! The minimum latency value (in seconds)
            seconds_double m_averageLatency = {};               //! The average latency value (in seconds)
            seconds_double m_historicalLatency = {};            //! The historical latency

            QList<Nedrysoft::RouteAnalyser::IPlot *> m_plots;   //! The additional plots
    };
}

Q_DECLARE_METATYPE(Nedrysoft::RouteAnalyser::PingData)
Q_DECLARE_METATYPE(Nedrysoft::RouteAnalyser::PingData *)

#endif // NEDRYSOFT_ROUTEANALYSER_PINGDATA_H
