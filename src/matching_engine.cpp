#include "matching_engine.h"

results_t MatchingEngine::add_new_order(Order order)
{
    if(order.order_type == O) {
        if(oid_to_symbol.count(order.oid) > 0) { // duplicate order
            std::string res = "E " + std::to_string(order.oid) +  " Duplicate order id";
            return results_t({ res });
        }

        oid_to_symbol[order.oid] = order.symbol; //associate oid to symbol

        if(symbol_to_book.count(order.symbol) == 0) { // orderbook for symbol does not exists
            OrderBook orderbook(order.symbol);
            symbol_to_book[order.symbol] = orderbook;
        }
        auto &orderbook = symbol_to_book.at(order.symbol); //get orderbook for symbol
        return orderbook.add_new_order(order);

    } else if(order.order_type == X) {
        //check if a cancellable order exists
        if(oid_to_symbol.count(order.oid) > 0) {
            auto symbol = oid_to_symbol.at(order.oid);
            auto& orderbook = symbol_to_book.at(symbol);
            return orderbook.cancel_order(order.oid);
        } else { // error invalid oid to cancel
            std::string res = "E " + std::to_string(order.oid) +
                " no such cancellable order";
            return results_t({ res });
        }
    } else {
        // should never get here... panic!
    }
    return results_t();
}


results_t MatchingEngine::print_all_books()
{
    results_t res;
    for(auto& i : symbol_to_book) {
        res.merge(i.second.print_order_book());
    }

    return res;
}
