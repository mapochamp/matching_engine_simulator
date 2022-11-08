#include "orderbook.h"

results_t OrderBook::add_new_order(Order order)
{
    auto qty_left_to_fill = attempt_fill(order);
    if(qty_left_to_fill != 0) {
        // add to resting orders
        resting_orders[order.oid] = order;
        // add to asks if side is sell
        if(order.side == sell) {
            if(asks.count(order.px) > 0) {
                auto& ask_queue = asks.at(order.px);
                ask_queue.push(order.oid);
            } else {
                asks[order.px] = std::queue<uint32_t>({ order.oid });
            }
        } else { //order is buy
            //add to bids
            if(bids.count(order.px) > 0) {
                auto& bid_queue = bids.at(order.px);
                bid_queue.push(order.oid);
            } else {
                bids[order.px] = std::queue<uint32_t>({ order.oid });
            }
        }
    }
    
    // make appropriate results_t
    // loop through stack of filled orders
    results_t res;

    while(!filled_orders.empty()) {
        auto& most_recently_filled = filled_orders.front();
        char buff[13];

        std::sprintf(buff, "%7.5f", most_recently_filled.px);

        std::string filled = "F " + std::to_string(most_recently_filled.oid) + " " +
            most_recently_filled.symbol + " " +
            std::to_string(most_recently_filled.qty) + " " +
            std::string(buff);

        res.push_back(filled);
        filled_orders.pop();
    }

    return res;
}

results_t OrderBook::cancel_order(uint32_t oid)
{
    // cancel the order
    if(resting_orders.count(oid) > 0) {
        resting_orders.erase(oid);
        std::string res = "X " + std::to_string(oid);
        return results_t({ res });
    }
    // live order was filled so nothing to cancel
    std::string res = "E " + std::to_string(oid) +
        " no such cancellable order";
    return results_t({ res });
}

uint16_t OrderBook::attempt_fill(Order& order)
{
    //if there is a fill, access the resting orders map and change quantity
    //and remove filled orders accordingly
    // if its a sell, match with bids
    if(order.side == sell) {
        //check if bids are empty or matchable
        if(!bids.empty() && bids.begin()->first >= order.px) {
            for(auto& top_of_book : bids) {
                auto price = top_of_book.first;
                // only iterate if price is marketable
                if(price >= order.px) {
                    auto& bid_queue = top_of_book.second;
                    if(order_fully_filled(bid_queue, order)) {
                        return 0;
                    }
                }
            }
        }
    } else if(order.side == buy) {
        //check if asks are empty or matchable
        if(!asks.empty() && asks.begin()->first <= order.px) {
            for(auto& top_of_book : asks) {
                auto price = top_of_book.first;
                if(price <= order.px) {
                    auto& ask_queue = top_of_book.second;
                    if(order_fully_filled(ask_queue, order)) {
                        return 0;
                    }
                }
            }
        }
    } else {
        //should never reach here ... panic!
    }
    return order.qty;
}

bool OrderBook::order_fully_filled(std::queue<uint32_t> order_queue, Order& order)
{
    while(!order_queue.empty()) {
        auto& oid = order_queue.front();
        if(resting_orders.count(oid) > 0) { //check if order isn't cancelled
            auto& order_to_match = resting_orders.at(oid);
            if(order_to_match.qty > order.qty) {
                auto partial_fill = order_to_match;
                partial_fill.qty = order.qty;
                order.px = partial_fill.px;
                filled_orders.push(order);
                filled_orders.push(partial_fill);
                order_to_match.qty -= order.qty;
                return true;
            } else {
                auto partial_fill = order;
                partial_fill.qty = order_to_match.qty;
                partial_fill.px = order_to_match.px;
                filled_orders.push(partial_fill);
                filled_orders.push(order_to_match);
                order.qty -= order_to_match.qty;
                resting_orders.erase(oid);
            }
        }
        order_queue.pop();
    }
    return false;
}

results_t OrderBook::print_order_book()
{
    results_t res;
    auto bid_copy = bids;
    auto ask_copy = asks;

    for(auto& ask_book : asks) {
        auto ask_queue = ask_book.second;
        print_book(ask_queue, res, true);
    }

    for(auto& bid_book : bids) {
        auto bid_queue = bid_book.second;
        print_book(bid_queue, res, false);
    }

    return res;
}

void OrderBook::print_book(std::queue<uint32_t> order_queue, results_t& res, bool is_ask)
{
    while(!order_queue.empty()){
        auto oid = order_queue.front();
        if(resting_orders.count(oid)) {
            auto order = resting_orders.at(oid);

            std::string side;
            if(order.side == sell) {
                side = "S";
            } else {
                side = "B";
            }

            std::string order_to_print = "P " + std::to_string(order.oid) + " " +
                order.symbol + " " +
                side + " " +
                std::to_string(order.qty) + " " +
                std::to_string(order.px);
            if(is_ask) {
                res.push_front(order_to_print);
            } else { 
                res.push_back(order_to_print);
            }
        }
        order_queue.pop();
    }
}
