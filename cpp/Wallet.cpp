#include "Wallet.h"
#include "CSVReader.h"
#include <vector>
#include <iostream>

Wallet::Wallet()
{

}

void Wallet::insertCurrency(const std::string &type, double amount)
{
    if(amount < 0)
        throw std::exception{};
    double balance {};
    if (currencies.count(type) == 0)
    {
        balance = 0;
    }
    else{
        balance = currencies[type];
    }
    balance += amount;
    currencies[type] = balance;
}

bool Wallet::removeCurrency(const std::string &type, double amount)
{
    if(amount < 0)
        throw std::exception{};
    if (currencies.count(type) == 0)
    {
        return false;
    }
    else{ // is there enough currency 
        if(containsCurrency(type, amount))
        {
            currencies[type] -= amount;
            return true;
        }
        return false; // not enough currency
    }
}

bool Wallet::containsCurrency(const std::string &type, double amount)
{
    if (currencies.count(type) == 0)
        return false;
    return currencies[type] >= amount;
}

bool Wallet::canFulfillOrder(const OrderBookEntry &order)
{
    // Split currency by separator '/' as in csv file
    std::vector<std::string> currency = CSVReader::tokenise(order.product, '/');
    
    //ask
    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;
        std::cout<<"Wallet::canFulfillOrder() "<<currency[0]<<" : "<<amount<<std::endl;
        return containsCurrency(currency[0], amount);
    }
    //bid
    else if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        std::cout<<"Wallet::canFulfillOrder() "<<currency[1]<<" : "<<amount<<std::endl;
        return containsCurrency(currency[1], amount);
    }
    else{
        std::cout<<"Wallet::canFulfillOrder() --- Likely Unknown Order type."<<std::endl;
        return false;
    }
}

void Wallet::processSale(const OrderBookEntry& sale)
{
    // Split currency by separator '/' as in csv file
    std::vector<std::string> currency = CSVReader::tokenise(sale.product, '/');
    
    //ask
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currency[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currency[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
    //bid
    if (sale.orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currency[0];
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currency[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}

std::string Wallet::toString()
{
    std::string s {};
    for(std::pair<const std::string &, double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    return s;
}