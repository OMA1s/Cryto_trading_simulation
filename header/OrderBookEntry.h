#pragma once

#include <string>

enum class OrderBookType {bid = 1, ask, asksale, bidsale, unknown}; // Enum to stro bid/ask type

//Class to store our CSV entries
class OrderBookEntry{
    public:
        //Constructor for assignment
        OrderBookEntry(double _price, 
                       double _amount, 
                       std::string _timestamp, 
                       std::string _product, 
                       OrderBookType _orderType,
                       std::string username = "dataset");
        /** Convert string e.g. bid/ask to OrderBookType Enum*/
        static OrderBookType stringToOrderBookType(const std::string &s);
        static bool compareByTimestamp(const OrderBookEntry &e1, const OrderBookEntry &e2){
            return e1.timestamp < e2.timestamp;
        }

        static bool compareByPriceAsc(const OrderBookEntry &e1, const OrderBookEntry &e2){
            return e1.price < e2.price;
        }

        static bool compareByPriceDesc(const OrderBookEntry &e1, const OrderBookEntry &e2){
            return e1.price > e2.price;
        }

        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
        std::string username;
};