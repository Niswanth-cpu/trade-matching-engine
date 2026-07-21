# Order Book Matching Engine

A console-based C++ application simulating a stock exchange order book system with trade matching, order tracking, and CSV report generation.

---

## Overview

This system processes BUY and SELL orders for multiple stock tickers, matches compatible trades using price-priority logic, and outputs trade execution reports. Built as a lab assignment demonstrating file handling, STL containers, and order matching algorithms.

---

## Features

- **Part 1 — Order Book Summary**: Reads orders from a CSV file and outputs the number of orders per ticker, sorted alphabetically.
- **Part 2 — User Order Totals**: Aggregates total quantity ordered per user across all orders.
- **Part 3 — Trade Matching Engine**: Matches BUY and SELL orders in real-time using price-priority, outputs executed trades to a CSV file.

---

## How It Works

### Trade Matching Logic (Part 3)
- A **SELL** order matches against the highest-priced BUY orders first (descending price).
- A **BUY** order matches against the lowest-priced SELL orders first (ascending price).
- A trade executes only when `buy_price >= sell_price`.
- Unmatched remainder stays in the order book for future matching.
- Each trade is timestamped incrementally.

---

## Input Format

### Part 1 & 2 — CSV File
TYPE,USERNAME,TICKER,QUANTITY,PRICE

BUY,Alice,AAPL,10,150.00

SELL,Bob,AAPL,5,148.00

### Part 3 — stdin (space-separated)
BUY Alice AAPL 10 150.00

SELL Bob AAPL 5 148.00

## Output Format

### Part 1
AAPL 3
GOOG 2

### Part 2
Alice 20
Bob 15

### Part 3 — CSV written to `./actual_output/Q1/CSV/<ROLL_NUMBER>/<filename>`
Ticker,Seller,Buyer,Qty,Price,Time
AAPL,Bob,Alice,5,148.00,0

## Tech Stack

- **C++17**
- **STL** — `vector`, `map`, `sort`
- **File Handling** — `ifstream`, `ofstream`
- **String Parsing** — `stringstream`, `getline`
.
├── main.cpp
├── README.md
└── actual_output/
└── Q1/
└── CSV/
└── <ROLL_NUMBER>/

---

## Author

**Niswanth** — IIT Madras, CS24B044
