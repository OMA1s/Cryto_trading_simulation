#pragma once

#include "OrderBookEntry.h"
#include <vector>

// Class to read and convert CSV data to OrderBookEntry objects
class CSVReader{
    public:
        CSVReader();
        /** Pass file string name, returns OrderBookEntry Vector with all Market data */
        static std::vector<OrderBookEntry> readCSV(std::string csvFile);
        /** Tokenise passed string line based on separator */
        static std::vector<std::string> tokenise(std::string csvLine, char separator);
        /** Convert string into an OrderBookEntry object */
        static OrderBookEntry stringToOBE(std::string price, 
                                          std::string amount,
                                          std::string timestamp,
                                          std::string product,
                                          OrderBookType orderBookType);
    private:
        /** Convert string into an OrderBookEntry object */
        static OrderBookEntry stringToOBE(std::vector<std::string> strings);
};