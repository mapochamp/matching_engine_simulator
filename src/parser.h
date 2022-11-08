#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "order.h"

enum Action { print_action, order_action };

class Parser {
    private:
        std::string action;

    public:
        Parser() {
        }

        Action parse(const std::string& line, Order& order);
};
