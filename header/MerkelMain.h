#pragma once

#include<vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

// Main application class to run the program
class MerkelMain{
    public:
        MerkelMain();
        /** Call this to start sim */
        void init();
    private:
        void printMenu(); //Print user Options Menu 1 - 6
        void printHelp(); // One of user options function
        void printMarketStats(); // One of user options function
        void enterAsk(); // One of user options function
        void enterBid(); // One of user options function
        void printWallet(); // One of user options function
        void gotoNextTimeFrame(); // One of user options function

        void processUserOption(int userOption); // Which function to call based on user input

        int getUserOption(); //get User input

        std::string currentTime;
        //OrderBook orderBook {"data.csv"};
        OrderBook orderBook {"data.csv"};
        Wallet wallet;
};