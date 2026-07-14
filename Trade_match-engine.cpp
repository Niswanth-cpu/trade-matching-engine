/**
 * matching_engine.cpp
 * * A simple Order Book and Trade Matching Engine.
 * Supports reading order data, summarizing statistics, tracking user 
 * order volumes, and running a live trade matching engine based on 
 * price-time priority.
 */

#include <bits/stdc++.h>
#include <fstream>
using namespace std;

// ─── Data Structures ────────────────────────────────────────────────────────

struct Order {
    string user;
    int qty;
    double price;
};

/*
 * Represents an Order Book for a specific financial instrument (ticker),
 * maintaining separate lists for open BUY and SELL orders.
 */
struct Book {
    string ticker;
    vector<Order> buyOrders;
    vector<Order> sellOrders;
    Book(string t) : ticker(t) {}
};

// ─── Comparators ────────────────────────────────────────────────────────────

// Sorts books by total order volume (descending), then by ticker (alphabetical)
bool byTicker(Book& a, Book& b) {
    if (a.ticker == b.ticker)
        return (a.buyOrders.size() + a.sellOrders.size()) > 
               (b.buyOrders.size() + b.sellOrders.size());
    return a.ticker < b.ticker;
}

bool byPriceAsc(Order& a, Order& b)  { return a.price < b.price; }
bool byPriceDesc(Order& a, Order& b) { return a.price > b.price; }

// ─── File Parsing ───────────────────────────────────────────────────────────

struct ParsedOrder {
    string type, user, ticker;
    int qty;
    double price;
};

// Parses a comma-separated (CSV) string into a ParsedOrder object
ParsedOrder parseCSVLine(const string& line) {
    stringstream ss(line);
    string type, user, ticker, qty_str, price_str;
    getline(ss, type,      ',');
    getline(ss, user,      ',');
    getline(ss, ticker,    ',');
    getline(ss, qty_str,   ',');
    getline(ss, price_str);
    return { type, user, ticker, stoi(qty_str), stod(price_str) };
}

// Parses a space-separated string into a ParsedOrder object
ParsedOrder parseSpaceLine(const string& line) {
    stringstream ss(line);
    string type, user, ticker, qty_str, price_str;
    getline(ss, type,      ' ');
    getline(ss, user,      ' ');
    getline(ss, ticker,    ' ');
    getline(ss, qty_str,   ' ');
    getline(ss, price_str);
    return { type, user, ticker, stoi(qty_str), stod(price_str) };
}

// ─── Order Book Helpers ─────────────────────────────────────────────────────

int findBook(vector<Book>& books, const string& ticker) {
    for (int i = 0; i < (int)books.size(); i++)
        if (books[i].ticker == ticker) return i;
    return -1;
}

/*
 * Loads a static order book state from a given CSV file path,
 * categorizing orders into their respective BUY or SELL queues.
 */
vector<Book> loadOrderBook(const string& filepath) {
    ifstream infile(filepath);
    if (!infile.is_open()) {
        cerr << "[ERROR] Could not open file: " << filepath << endl;
        return {};
    }

    vector<Book> books;
    string line;
    while (getline(infile, line)) {
        if (line.empty()) break;
        auto o = parseCSVLine(line);

        int idx = findBook(books, o.ticker);
        if (idx == -1) {
            books.push_back(Book(o.ticker));
            idx = books.size() - 1;
        }

        Order order = { o.user, o.qty, o.price };
        if (o.type == "BUY")       books[idx].buyOrders.push_back(order);
        else if (o.type == "SELL") books[idx].sellOrders.push_back(order);
    }
    return books;
}

// ─── Features ───────────────────────────────────────────────────────────────

/*
 * Feature 1 (P1): Order Book Summary
 * Prints the total number of unique order entries for each ticker, 
 * sorted alphabetically.
 */
void showOrderBookSummary() {
    string filepath;
    cin >> filepath;
    cin.ignore(1000, '\n');

    ifstream infile(filepath);
    if (!infile.is_open()) {
        cerr << "[ERROR] Could not open file: " << filepath << endl;
        return;
    }

    map<string, int> tickerCount;
    string line;
    while (getline(infile, line)) {
        if (line.empty()) break;
        auto o = parseCSVLine(line);
        tickerCount[o.ticker]++;
    }

    for (auto& [ticker, count] : tickerCount)
        cout << ticker << " " << count << "\n";
}

/*
 * Feature 2 (P2): User Order Volume
 * Calculates the total quantity of shares ordered by each user,
 * then answers interactive queries from standard input.
 */
void showUserOrderVolume() {
    string filepath;
    cin >> filepath;
    cin.ignore(1000, '\n');

    ifstream infile(filepath);
    if (!infile.is_open()) {
        cerr << "[ERROR] Could not open file: " << filepath << endl;
        return;
    }

    map<string, int> userQty;
    string line;
    while (getline(infile, line)) {
        if (line.empty()) break;
        auto o = parseCSVLine(line);
        userQty[o.user] += o.qty;
    }

    string user;
    while (cin >> user) {
        if (user.empty()) break;
        cout << user << " " << (userQty.count(user) ? userQty[user] : 0) << "\n";
    }
}

/*
 * Feature 3 (P3): Live Trade Matching Engine
 * Processes a continuous stream of orders dynamically using price-time priority. 
 * Executed trades are logged to a specified CSV report. Unmatched remainders
 * are stored in the order book.
 */
void runMatchingEngine(const string& outputPath) {
    ofstream out(outputPath);
    if (!out.is_open()) {
        cerr << "[ERROR] Could not open output file: " << outputPath << endl;
        return;
    }

    out << "Ticker,Seller,Buyer,Qty,Price,Time\n";

    vector<Book> books;
    int timestamp = 0;
    string line;

    while (getline(cin, line)) {
        if (line.empty()) break;

        auto o = parseSpaceLine(line);
        Order order = { o.user, o.qty, o.price };

        int idx = findBook(books, o.ticker);
        if (idx == -1) {
            books.push_back(Book(o.ticker));
            idx = books.size() - 1;
        }

        Book& book = books[idx];

        if (o.type == "SELL") {
            sort(book.buyOrders.begin(), book.buyOrders.end(), byPriceDesc);

            while (order.qty > 0 && !book.buyOrders.empty()) {
                if (book.buyOrders[0].price < order.price) break;

                int tradeQty = min(order.qty, book.buyOrders[0].qty);
                
                out << o.ticker << "," << order.user << ","
                    << book.buyOrders[0].user << "," << tradeQty << ","
                    << fixed << setprecision(2) << order.price << ","
                    << timestamp++ << "\n";

                order.qty             -= tradeQty;
                book.buyOrders[0].qty -= tradeQty;
                
                if (book.buyOrders[0].qty == 0)
                    book.buyOrders.erase(book.buyOrders.begin());
            }
            if (order.qty > 0) book.sellOrders.push_back(order);
        }

        else if (o.type == "BUY") {
            sort(book.sellOrders.begin(), book.sellOrders.end(), byPriceAsc);

            while (order.qty > 0 && !book.sellOrders.empty()) {
                if (book.sellOrders[0].price > order.price) break;

                int tradeQty = min(order.qty, book.sellOrders[0].qty);
                
                out << o.ticker << "," << book.sellOrders[0].user << ","
                    << order.user << "," << tradeQty << ","
                    << fixed << setprecision(2) << book.sellOrders[0].price << ","
                    << timestamp++ << "\n";

                order.qty               -= tradeQty;
                book.sellOrders[0].qty  -= tradeQty;
                
                if (book.sellOrders[0].qty == 0)
                    book.sellOrders.erase(book.sellOrders.begin());
            }
            if (order.qty > 0) book.buyOrders.push_back(order);
        }
    }

    out.close();
    cout << "[INFO] Trade report written to: " << outputPath << "\n";
}

// ─── Entry Point ─────────────────────────────────────────────────────────────

int main() {
    string mode;
    cin >> mode;

    if (mode == "P1") {
        showOrderBookSummary();

    } else if (mode == "P2") {
        showUserOrderVolume();

    } else if (mode == "P3") {
        string fileName;
        cin >> fileName;
        cin.ignore(1000, '\n');

        string outputPath = fileName;
        runMatchingEngine(outputPath);

    } else {
        cerr << "[ERROR] Unknown mode: " << mode << ". Use P1, P2, or P3.\n";
    }

    return 0;
}
