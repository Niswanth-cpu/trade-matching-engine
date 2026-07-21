
# Problem Statement: Low-Latency Software for Trading

## Background
You have been assigned to the Core Engineering team for a proprietary High-Frequency Trading (HFT) platform. Your mission is to build a compact analytics and execution utility that can accept live orders, execute matches based on strict financial rules, and write the executed transactions to a system log.

## Task: Live Matching Engine with Transaction Logging
You must build a central matching engine that accepts incoming limit orders, executes matches when conditions are met, and logs the transactions.

### Execution Rules
The matching engine must follow these standard exchange rules:
* A transaction executes when a buy order and sell order satisfy `Sell Price <= Buy Price`.
* The execution price is strictly the **seller's price**.
* The executed quantity is the minimum of the remaining buy and sell quantities; leftovers must remain in the order book.

### Matching Priority & Best Price Logic
* **BUY Orders:** An incoming BUY order matches against existing SELL orders for the same ticker, prioritizing the **lowest SELL price** first. If multiple SELL orders have the same price, use **time-priority**: match with the oldest (earliest entered) SELL order.
* **SELL Orders:** An incoming SELL order matches against existing BUY orders for the same ticker, prioritizing the **highest BUY price** first. If multiple BUY orders have the same price, use **time-priority**: match with the oldest (earliest entered) BUY order.
* **Logical Time:** Time starts at `0` and increases by `1` for each executed transaction.

### Input Data Format
Orders are submitted to the engine containing the following fields:
`(Order Type) (Trader Name) (Company Ticker) (Quantity) (Price)`

### Expected Output Log
The engine must export an executed trades log (`executed_trades.csv`) utilizing the following header and format:
`Ticker,Seller,Buyer,Qty,Price,Time`

*Note: All price values in the output CSV must be formatted to exactly 2 decimal places.*

### Example Execution Walkthrough
**Incoming Orders:**
1. `BUY Rajesh INFY 100 2450.00`
2. `SELL Priya INFY 60 2440.00`
3. `BUY Arjun TCS 50 3100.00`
4. `SELL Divya TCS 50 3090.00`

**Expected Output (`executed_trades.csv`):**
```csv
Ticker,Seller,Buyer,Qty,Price,Time
INFY,Priya,Rajesh,60,2440.00,0
TCS,Divya,Arjun,50,3090.00,1
