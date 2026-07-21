# C++ High-Frequency Order Book Matching Engine

A low-latency, modular order book matching engine simulating a financial exchange. It processes incoming limit orders, executes trades using strict **Price-Time Priority**, and logs completed transactions.

## ⚙️ Architecture & Core Logic

To optimize for high-frequency trading constraints, the core logic avoids naive $O(N \log N)$ sorting per order. Instead, it maintains a continuous state using specialized standard template library (STL) containers:

* **Price Priority (`std::map`)**: Bids are sorted in descending order (`std::greater<double>`) and asks are sorted in ascending order. This ensures $O(\log U)$ insertion time (where $U$ is the number of unique price levels), allowing instantaneous access to the best available market price.
* **Time Priority (`std::queue`)**: Orders placed at the exact same price level are queued. This ensures absolute $O(1)$ time-priority execution without requiring timestamp sorting or complex struct comparisons.
* **Exchange Routing (`std::unordered_map`)**: The central engine routes incoming orders to their specific ticker's order book in $O(1)$ time using a hash map.

## 🚀 Build and Run Instructions

This project is contained in a single, easy-to-compile C++ file.

### Prerequisites
* A C++ compiler (GCC/g++, Clang, etc.)

### Build & Execution
```bash
# Clone the repository
git clone [https://github.com/YourUsername/matching-engine.git](https://github.com/YourUsername/matching-engine.git)
cd matching-engine

# Compile the code
g++ main.cpp -o matching_engine -std=c++17

# Run the simulation
./matching_engine
