#pragma once

#include "OrderBook.h"
#include "CSVReader.h"
#include <string>
#include <vector>

// A class to store and manage orderbook data
class OrderBook{
    public:
        /** Construct reading CSV data file, Load market data from CSV file into orders variable*/
        OrderBook(std::string fileName);
        /** return vector of all known products in dataset */
        std::vector<std::string> getKnownProducts();
        /** Return vector of orders based on select filters */
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                              std::string product,
                                              std::string timestamp);

        /**Returns earliest time in order book */
        std::string getEarliestTime();
        /**Returns next time after the sent time in the order book
         * if there is no next timestamp, wraps around to start
         */
        std::string getNextTime(const std::string&);

        void insertOrder(const OrderBookEntry &e);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry> const &orders);
        static double getLowPrice(std::vector<OrderBookEntry> const &orders);
        static double getAverage(std::vector<OrderBookEntry> const &orders);
        static double getVariance(std::vector<OrderBookEntry> const &orders);
        /**Check how far are data points from mean */
        static double getStandardDeviation(std::vector<OrderBookEntry> const &orders);
    private:
        std::vector<OrderBookEntry> orders;
};