#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader(){

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string fileName){
    std::vector<OrderBookEntry> entries {};

    std::ifstream csvFile {fileName, std::ios::in};
    std::string line {};
        
    std::vector<std::string> tokens {};
    if (csvFile.is_open()){
        std::string line {};
        while(std::getline(csvFile, line)){
            try{
                entries.push_back(CSVReader::stringToOBE(tokenise(line, ',')));
            }
            catch(const std::exception &e){
                std::cout<<"Bad data..."<<std::endl;
            }
            
        }
        csvFile.close();
    }
    else{
        std::cout<< "Could not open file" <<std::endl;
    }
    std::cout<<"CSVReader::readCSV read "<<entries.size()<<" entries."<<std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator){
    std::vector<std::string> tokens {};
    signed int start {}, end {};
    start = csvLine.find_first_not_of(separator, 0);
    do
    {
        end = csvLine.find_first_of(separator, start); //separator index;
        std::string token {};
        if(start == csvLine.length() || start == end) //no token
            break;
        if(end >= 0) //token found
            token = csvLine.substr(start, end - start); //get string till separator
        else
            token = csvLine.substr(start, csvLine.length() - start); //no more separator, get string till end of line
        tokens.push_back(token);
        start = end + 1;
    } while (end != std::string::npos);
    
    return tokens;
}

OrderBookEntry CSVReader::stringToOBE(std::vector<std::string> tokens){
    // Upon manually inspecting CSV file lines:
    // token[0] = timestamp, token[1] = product, token[2] = bid or ask, token[3] = price, token[4] = amount

    double price {};
    double amount {};

    if(tokens.size() != 5){ //Check for 5 tokens
        std::cout << "bad line" <<std::endl;
        throw std::exception{};
    }
    
    try{
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch(const std::exception& e){
        std::cout<<"Bad float! " << tokens[3]<<std::endl;
        std::cout<<"Bad float! " << tokens[4]<<std::endl;
        throw std::exception{};
    }
            
    
    OrderBookEntry obe {price, amount, tokens[0], tokens[1], OrderBookEntry::stringToOrderBookType(tokens[2])};
    return obe;
}

OrderBookEntry CSVReader::stringToOBE(std::string priceString, 
                                      std::string amountString,
                                      std::string timestamp,
                                      std::string product,
                                      OrderBookType orderBookType)
{
    double price {};
    double amount {};

    try{
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch(const std::exception& e){
        std::cout<<"CSVReader::stringToOBE Bad float! " << priceString<<std::endl;
        std::cout<<"CSVReader::stringToOBE Bad float! " << amountString<<std::endl;
        throw std::exception{};
    }
            
    
    OrderBookEntry obe {price, amount, timestamp, product, orderBookType};
    return obe;
}