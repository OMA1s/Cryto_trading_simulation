#include <iostream>
#include <limits>
#include "MerkelMain.h"
#include "CSVReader.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init(){
    int input = 0;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while(true){
        printMenu();
        try{
            input = getUserOption();    
        }
        catch(const std::exception &e){
            std::cout<<"MerkelMain::init() Bad Input. Try again."<<std::endl;
        }
        processUserOption(input);
    }
    
}

void MerkelMain::printMenu(){
    std::cout<<"1: Print help "<<std::endl;
    std::cout<<"2: Print exchange rates "<<std::endl;
    std::cout<<"3: Make an offer "<<std::endl;
    std::cout<<"4: Make a bid "<<std::endl;
    std::cout<<"5: Print wallet "<<std::endl;
    std::cout<<"6: Continue "<<std::endl;
    std::cout<<"================"<<std::endl;

    std::cout<<"Current time is: "<<currentTime<<std::endl;
}

int MerkelMain::getUserOption(){
    int userOption = 0;
    std::string line {};
    std::cout<<"Type in 1 - 6"<<std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }
    catch(const std::exception &e){
        std::cout<<"MerkelMain::getUserOption() Bad Input! "<<std::endl;
        throw e;
    }
    

    std::cout<<"You chose: "<<userOption<<std::endl;
    return userOption;
}


void MerkelMain::printHelp(){
    std::cout<<"Help - your aim is to make money. Analyse the market and make bids and offers."<<std::endl;
}

void MerkelMain::printMarketStats(){

    for (const std::string &p : orderBook.getKnownProducts())
    {
        std::cout<< "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);

        std::cout<<"Asks seen: "<<entries.size()<<std::endl;
        std::cout<<"Max ask: "<< OrderBook::getHighPrice(entries)<<std::endl;
        std::cout<<"Min ask: "<< OrderBook::getLowPrice(entries)<<std::endl;
        std::cout<<"Average ask price: "<< OrderBook::getAverage(entries)<<std::endl;
        std::cout<<"Standard deviation, or the spread of data from mean is: ";
        std::cout<<OrderBook::getStandardDeviation(entries)<<std::endl;
    }
}

void MerkelMain::enterAsk(){
    std::cout<<"Mark an ask - enter the amount. Format: Product/price/amount e.g. ETH/BTC, 0.02196165, 37.7"<<std::endl;
    std::string input {};
    std::getline(std::cin, input);
    std::cout<<"You typed: "<<input<<std::endl;

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3)
    {
        std::cout<<"MerkelMain::enterAsk() Bad Input! Not enough tokens. Tokens: "<<input<<std::endl;
    }
    else{
        try{
            OrderBookEntry obe = CSVReader::stringToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::ask);
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout<<"Wallet Looks good."<<std::endl;
                orderBook.insertOrder(obe);
            }
            else{
                std::cout<<"Wallet has insufficient funds."<<std::endl;
            }
        }
        catch(const std::exception &e){
            std::cout<<"MerkelMain::enterAsk() Bad Input! "<<std::endl;
        }
    }
}

void MerkelMain::enterBid(){
    std::cout<<"Make a bid - enter the amount."<<std::endl;
    std::cout<<"Format: Product(What u want vs what u offer)/ price (How much you offer per unit) / amount(How many u want?) \ne.g. ETH/BTC, 0.02196165, 37.7"<<std::endl;
    std::string input {};
    std::getline(std::cin, input);
    std::cout<<"You typed: "<<input<<std::endl;

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3)
    {
        std::cout<<"MerkelMain::enterBid() Bad Input! Not enough tokens. Tokens: "<<input<<std::endl;
    }
    else{
        try{
            OrderBookEntry obe = CSVReader::stringToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::bid);
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout<<"Wallet Looks good."<<std::endl;
                orderBook.insertOrder(obe);
            }
            else{
                std::cout<<"Wallet has insufficient funds."<<std::endl;
            }
        }
        catch(const std::exception &e){
            std::cout<<"MerkelMain::enterBid() Bad Input! "<<std::endl;
        }
    }
}

void MerkelMain::printWallet(){
    std::cout<<wallet.toString()<<std::endl;
    
}

void MerkelMain::gotoNextTimeFrame(){
    std::cout<<"Going to next time frame."<<std::endl;

    for (const std::string &p : orderBook.getKnownProducts())
    {
        std::cout<<"Matching "<<p<<std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout<<"Sales: "<<sales.size()<<std::endl;
        for (const auto &sale : sales)
        {
            std::cout<<"Sale amount: "<<sale.amount<<", Sale price: "<<sale.price<<std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
    }
    

    currentTime = orderBook.getNextTime(this->currentTime);
}
void MerkelMain::processUserOption(int userOption){
    switch (userOption)
        {
        case 0:
            std::cout<<"Might be cin fail trigger. Value is 0"<<std::endl;
            break;
        case 1:
            printHelp();
            break;
        case 2:
            printMarketStats();
            break;
        case 3:
            enterAsk();
            break;
        case 4:
            enterBid();
            break;
        case 5:
            printWallet();
            break;
        case 6:
            gotoNextTimeFrame();
            break;
        default:
            std::cout<<"Invalid or unknown option"<<std::endl;
            break;  
    }
}