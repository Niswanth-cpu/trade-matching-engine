#include <bits/stdc++.h>

using namespace std;

// 1. Core Data Models
enum class OrderType { BUY, SELL };

struct Order {
    uint64_t orderId;
    string traderName;
    string ticker;
    OrderType type;
    int quantity;
    double price;
};

struct Trade {
    string ticker;
    string sellerName;
    string buyerName;
    int quantity;
    double price;
    uint64_t timestamp;
};

// 2. Single Ticker Order Book 
class OrderBook {
private:
    string ticker;
    
    // Bids (Buy orders): Sorted highest price first (greater)
    // The queue maintains time-priority for orders at the exact same price.
    map<double, queue<Order>, greater<double>> buySide;
    
    // Asks (Sell orders): Sorted lowest price first (default less)
    map<double, queue<Order>> sellSide;

public:
    explicit OrderBook(const string& symbol) : ticker(symbol) {}

    void addOrder(const Order& order, vector<Trade>& tradeLog, uint64_t& systemTime) {
        if (order.type == OrderType::BUY) {
            processBuyOrder(order, tradeLog, systemTime);
        } else {
            processSellOrder(order, tradeLog, systemTime);
        }
    }

private:
    void processBuyOrder(Order order, vector<Trade>& tradeLog, uint64_t& systemTime) {
        // Match with existing Sell orders where Sell Price <= Buy Price
        while (order.quantity > 0 && !sellSide.empty()) {
            auto bestSellIt = sellSide.begin();
            double bestSellPrice = bestSellIt->first;

            if (bestSellPrice > order.price) {
                break; // No matching price found
            }

            queue<Order>& bestSellQueue = bestSellIt->second;
            Order& restingSell = bestSellQueue.front();

            int tradeQty = min(order.quantity, restingSell.quantity);
            
            // Execute trade at the resting seller's price
            tradeLog.push_back({
                ticker, 
                restingSell.traderName, 
                order.traderName, 
                tradeQty, 
                bestSellPrice, 
                systemTime++
            });

            order.quantity -= tradeQty;
            restingSell.quantity -= tradeQty;

            if (restingSell.quantity == 0) {
                bestSellQueue.pop();
                if (bestSellQueue.empty()) {
                    sellSide.erase(bestSellIt);
                }
            }
        }

        // Add remaining quantity to the order book
        if (order.quantity > 0) {
            buySide[order.price].push(order);
        }
    }

    void processSellOrder(Order order, vector<Trade>& tradeLog, uint64_t& systemTime) {
        // Match with existing Buy orders where Buy Price >= Sell Price
        while (order.quantity > 0 && !buySide.empty()) {
            auto bestBuyIt = buySide.begin();
            double bestBuyPrice = bestBuyIt->first;

            if (bestBuyPrice < order.price) {
                break; // No matching price found
            }

            queue<Order>& bestBuyQueue = bestBuyIt->second;
            Order& restingBuy = bestBuyQueue.front();

            int tradeQty = min(order.quantity, restingBuy.quantity);
            
            // Execute trade at the resting buyer's price
            tradeLog.push_back({
                ticker, 
                order.traderName, 
                restingBuy.traderName, 
                tradeQty, 
                bestBuyPrice, 
                systemTime++
            });

            order.quantity -= tradeQty;
            restingBuy.quantity -= tradeQty;

            if (restingBuy.quantity == 0) {
                bestBuyQueue.pop();
                if (bestBuyQueue.empty()) {
                    buySide.erase(bestBuyIt);
                }
            }
        }

        // Add remaining quantity to the order book
        if (order.quantity > 0) {
            sellSide[order.price].push(order);
        }
    }
};

// 3. Central Matching Engine
class MatchingEngine {
private:
    unordered_map<string, OrderBook> orderBooks;
    vector<Trade> tradeLog;
    uint64_t systemTime = 0;
    uint64_t orderCounter = 0;

public:
    void submitOrder(const string& typeStr, const string& trader, 
                     const string& ticker, int qty, double price) {
        
        OrderType type = (typeStr == "BUY") ? OrderType::BUY : OrderType::SELL;
        Order newOrder = {++orderCounter, trader, ticker, type, qty, price};

        // Initialize order book for ticker if it doesn't exist
        if (orderBooks.find(ticker) == orderBooks.end()) {
            orderBooks.emplace(ticker, OrderBook(ticker));
        }

        orderBooks.at(ticker).addOrder(newOrder, tradeLog, systemTime);
    }

    void exportTradesToCSV(const string& filepath) const {
        ofstream out(filepath);
        if (!out.is_open()) {
            cerr << "Error: Could not open output file: " << filepath << "\n";
            return;
        }

        out << "Ticker,Seller,Buyer,Qty,Price,Time\n";
        for (const auto& trade : tradeLog) {
            out << trade.ticker << ","
                << trade.sellerName << ","
                << trade.buyerName << ","
                << trade.quantity << ","
                << fixed << setprecision(2) << trade.price << ","
                << trade.timestamp << "\n";
        }
        out.close();
        cout << "Successfully exported " << tradeLog.size() << " trades to " << filepath << "\n";
    }
};

// 4. Client / Application Layer
int main() {
    MatchingEngine exchange;

    cout << "Starting Matching Engine Simulation...\n";
    
    exchange.submitOrder("BUY", "Rajesh", "INFY", 100, 2450.00);
    exchange.submitOrder("SELL", "Priya", "INFY", 60, 2440.00);
    exchange.submitOrder("BUY", "Arjun", "TCS", 50, 3100.00);
    exchange.submitOrder("SELL", "Divya", "TCS", 50, 3090.00);

    exchange.exportTradesToCSV("executed_trades.csv");

    return 0;
}
