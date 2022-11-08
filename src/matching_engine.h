#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "orderbook.h"
#include "order.h"
#include "types.h"

class MatchingEngine {
    private:
        std::map<std::string, OrderBook> symbol_to_book;
        std::map<uint32_t, std::string> oid_to_symbol;

    public:
        results_t add_new_order(Order order);
        results_t print_all_books();
        results_t print_symbol(std::string symbol);
};
