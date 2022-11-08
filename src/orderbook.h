#pragma once
#include <queue>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <iostream>
#include "order.h"
#include "types.h"

class OrderBook {
    private:
        std::string symbol;
        std::map<double, std::queue<uint32_t>> asks;
        std::map<double, std::queue<uint32_t>, std::greater<double>> bids;
        std::map<uint32_t, Order> resting_orders;
        std::queue<Order> filled_orders;


        // attempt to fil given order.
        // returns amount filled
        uint16_t attempt_fill(Order& order);
        bool order_fully_filled(std::queue<uint32_t> order_queue, Order& order);
        void print_book(std::queue<uint32_t> order_queue, results_t& res, bool is_ask);

    public:
        OrderBook(std::string symbol) : symbol{ symbol }{
        }

        OrderBook() {}
        
        results_t add_new_order(Order order);
        results_t cancel_order(uint32_t oid);
        results_t print_order_book();
};
