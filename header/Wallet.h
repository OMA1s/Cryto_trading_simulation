#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet{
    public:
        Wallet();
        /** Insert currency into the wallet */
        void insertCurrency(const std::string &type, double amount);
        /** Check if wallet contains this much currency or more */
        bool containsCurrency(const std::string &type, double amount);
        /** Removes currency from Wallet */
        bool removeCurrency(const std::string &type, double amount);
        /** Checks if the wallet can fulfill ask or bid */
        bool canFulfillOrder(const OrderBookEntry &order);
        /** Update user wallet based on the sale made */
        void processSale(const OrderBookEntry& sale);
        /** Generate string representation of the wallet */
        std::string toString();

    private:
        std::map<std::string, double> currencies;

};