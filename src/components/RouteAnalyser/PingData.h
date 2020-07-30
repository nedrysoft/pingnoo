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

#ifndef FIZZYADE_ROUTEANALYSER_PINGDATA_H
#define FIZZYADE_ROUTEANALYSER_PINGDATA_H

#include "Core/PingResult.h"
#include <cmath>
#include <QString>
#include <QPersistentModelIndex>
#include <QVariant>

class QCustomPlot;
class QTableView;
class QStandardItemModel;

namespace FizzyAde::RouteAnalyser
{
    class RouteItemTableDelegate;

    typedef std::chrono::duration<double, std::ratio<1,1000>> milliseconds_double;
    typedef std::chrono::duration<double, std::ratio<1,1>> seconds_double;

    /**
     * Route table data item
     *
     * Holds data about each hop and updates the route table
     * when the object is updated.
     */

    class PingData
    {
    public:
        /**
         * Column ID's (columns appear in this order)
         */
        enum PingDataFields
        {
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
         * Required for QVariant meta type
         *
         */
        PingData() = default;
        ~PingData() = default;
        PingData(const PingData &) = default;
        PingData &operator=(const PingData &) = default;

        /**
         * Constructor for a route table item
         *
         * Creates an entry in the given data table and initialises the row
         * with default information.
         *
         * @param[in]   tableModel          the table model
         * @param[in]   hop                 the hop number of this item
         * @param[in]   hopValid            true if the hop responds to ping, else false
         *
         */
        PingData(QStandardItemModel *tableModel, int hop, bool hopValid);

        /**
         * Sets the historical latency for this point
         *
         * This can optionally be drawn on the latency graph when hovering
         * over a chart.
         *
         * @param[in]   latency             the latency
         *
         */
        void setHistoricalLatency(std::chrono::duration<double> latency);

        /**
         * Updates the route table item with the result
         *
         * @param[in]   result              the ping result for this hop
         *
         */
        void updateItem(FizzyAde::Core::PingResult result);

        /**
         * Sets the hop number for this item
         *
         * @param[in]   hop                 the hop number
         *
         */
        void setHop(int hop);

        /**
         * Returns the hop number for this route item
         *
         * @return      the hop number
         *
         */
        int hop();

        /**
         * Sets the displayed ip address for this route item
         *
         * @param[in]   hostAddress         address for this hop
         *
         */
        void setHostAddress(QString hostAddress);

        /**
         * Returns the displayed ip address for this route item
         *
         * @return      the address for this hop
         *
         */
        QString hostAddress();

        /**
         * Sets the displayed host name for this route item
         *
         * @param[in]   hostName            the host name
         *
         */
        void setHostName(QString hostName);

        /**
         * Returns the displayed host name for this route item
         *
         * @return      the host name
         *
         */
        QString hostName();

        /**
         * Sets the location position
         *
         * @param[in]   location            the location
         *
         */
        void setLocation(const QString &location);

        /**
         * Returns the displayed location
         *
         * @return      the location
         *
         */
        QString location();

        /**
         * Sets the graph associated with this route item
         *
         * @param[in]   customPlot          the plot
         *
         */
        void setCustomPlot(QCustomPlot *customPlot);

        /**
         * Sets the jitter graph associated with this route item
         *
         * @param[in]   jitterPlot          the plot
         *
         */
        void setJitterPlot(QCustomPlot *jitterPlot);

        /**
         * Returns the graph associated with this route item
         *
         * @return      the plot
         *
         */
        QCustomPlot *customPlot();

        /**
         * Returns the jitter graph associated with this route item
         *
         * @return      the plot
         *
         */
        QCustomPlot *jitterPlot();

        /**
         * Returns whether this hop is valid
         *
         * @return      true if valid, else false
         *
         */
        bool hopValid();

        /**
         * Sets the valid state for this hop
         *
         * @param[in]   hopValid            whether the hop is valid
         *
         */
        void setHopValid(bool hopValid);

        /**
         * Returns the latency
         *
         * @param[in]   field which latency to retrieve
         *
         * @return      the latency
         *
         */
        double latency(int field);

        /**
         * Returns the packet loss %
         *
         * @return      the packet loss
         *
         */
        double packetLoss();
    protected:

        /**
         * Calculates a running average
         *
         * Used to calculate the running average of the latency, does
         * not store previous values.
         *
         * @param[in]   previousAverage     the current average
         * @param[in]   value               the new value to add to the average
         * @param[in]   n                   the number of values (including the new one) used
         *
         * @return      the plot
         *
         */
        static double runningAverage(double previousAverage, double value, double n);

        /**
         * Updates the model so that views refresh
         *
         */
        void updateModel();

        /**
         * Gets the table model associated with this item
         *
         * @return      table model
         *
         */
        QStandardItemModel *tableModel();

        /**
         * Returns the number of samples sent
         *
         * @return      sample count
         *
         */
        unsigned long count();

        friend class RouteTableItemDelegate;

    private:
        QStandardItemModel *m_tableModel;                   //! The table model
        QCustomPlot *m_customPlot;                          //! The graph
        QCustomPlot *m_jitterPlot;                          //! The graph
        QPersistentModelIndex m_modelIndex;                 //! Model index in the route talbe of this item

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
    };
}

Q_DECLARE_METATYPE(FizzyAde::RouteAnalyser::PingData)
Q_DECLARE_METATYPE(FizzyAde::RouteAnalyser::PingData *)

#endif // FIZZYADE_ROUTEANALYSER_PINGDATA_H
