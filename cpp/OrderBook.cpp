#include "OrderBook.h"
#include <map>
#include <math.h>
#include <algorithm>

OrderBook::OrderBook(std::string _fileName){
    this->orders = CSVReader::readCSV(_fileName);
}

std::vector<std::string> OrderBook::getKnownProducts(){
    std::vector<std::string> prooducts {};

    std::map<std::string, bool> prodMap {};

    //Populate map with all the unique products e.g. ETH/BTC, DOGE/BTC
    for (const OrderBookEntry &order : orders){
        prodMap[order.product] = true;
    }

    // flatten map to vector
    for (auto const &itr : prodMap){
        prooducts.push_back(itr.first);
    }
    return prooducts;
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);

    std::vector<OrderBookEntry> sales {};
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    for (OrderBookEntry &ask : asks)
    {
        for (OrderBookEntry &bid : bids)
        {
            if (bid.price >= ask.price)
            {
                OrderBookEntry sale {ask.price, 0, timestamp, product, OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }
                
                if (bid.amount == ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }
                if (bid.amount > ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount -= ask.amount;
                    break;
                }
                if (bid.amount < ask.amount && bid.amount > 0)
                {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount -= bid.amount;
                    bid.amount = 0;
                    continue;
                }
            }
        }
    }
    return sales;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> const &orders)
{
    if (orders.empty())
        return 0;
    double max = orders[0].price;
    for(const OrderBookEntry &order : orders)
    {
        if (order.price > max)
            max = order.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> const &orders)
{
    if (orders.empty())
        return 0;    
    double min = orders[0].price;
    for(const OrderBookEntry &order : orders)
    {
        if (order.price < min)
            min = order.price;
    }
    return min;
}

void OrderBook::insertOrder(const OrderBookEntry &order){
    orders.push_back(order);
    
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

double OrderBook::getAverage(std::vector<OrderBookEntry> const &orders)
{
    if (orders.empty())
        return 0;
    double avg {};
    for(const OrderBookEntry &order : orders)
    {
        avg += order.price;
    }
    return avg / (orders.size());
}

double OrderBook::getVariance(std::vector<OrderBookEntry> const &orders)
{
    double avg = getAverage(orders);
    double variance {};
    for(const OrderBookEntry &order : orders)
    {
        variance += pow((order.price - avg), 2);
    }
    return variance / (orders.size());
}

double OrderBook::getStandardDeviation(std::vector<OrderBookEntry> const &orders)
{
    if (orders.empty())
        return 0;
    return sqrt(getVariance(orders));
}

std::string OrderBook::getEarliestTime()
{
    //Assumption is order is sorted
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(const std::string &timestamp)
{
    std::string nextTimestamp {};
    for (const OrderBookEntry &order : orders){
        if (order.timestamp > timestamp){
            nextTimestamp = order.timestamp;
            break;
        }
    }
    if (nextTimestamp.empty()){
        nextTimestamp = orders[0].timestamp;
    }
    return nextTimestamp;
}

std::vector<OrderBookEntry> OrderBook::getOrders(
    OrderBookType type,
    std::string product,
    std::string timestamp)
    {
        std::vector<OrderBookEntry> entries {};

        //Populate entries based on filters and return it
        for (const OrderBookEntry &order : orders)
        {
            if (order.orderType == type &&
                order.product == product &&
                order.timestamp == timestamp)
                {
                    entries.push_back(order);
                }
                
        }
        return entries;
                                            
}

