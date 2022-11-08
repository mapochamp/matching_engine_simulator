#include "parser.h"
Action Parser::parse(const std::string& line, Order& order) {
    int count = 0;
    size_t last = 0;
    size_t next = 0;
    std::string token;
    std::vector<std::string> action;

    while ((next = line.find(" ", last)) != std::string::npos) {
        token = line.substr(last, next-last);
        action.push_back(token);
        last = next + 1;
    }
    action.push_back(line.substr(last));

    switch(action.size()) {
        case 1:
            if(action[0] != "P") { 
                throw std::invalid_argument("Invalid action"); 
            }
            return print_action;
            break;
        case 2:
            if(action[0] != "X") { 
                throw std::invalid_argument("Invalid action"); 
            }
            order =  Order(X, std::stoi(action[1]));
            return order_action;
            break;
        case 6:
            if(action[0] != "O") { 
                throw std::invalid_argument("Invalid action"); 
            }

            Side side;
            if(action[3] == "B") {
                side = buy;
            } else if(action[3] == "S") {
                side = sell;
            } else {
                throw std::invalid_argument("Invalid order type"); 
            }

            if(action[2].length() > 8) {
                throw std::invalid_argument("Invalid symbol name");
            }

            order = Order(O, // ordertype
                    std::stoi(action[1]), // oid
                    action[2], // symbol
                    side, // side
                    std::stoi(action[4]), // qty
                    std::stoi(action[5])); // price

            return order_action;
            break;
        default:
            throw std::invalid_argument("Invalid argument");
    }
    throw std::invalid_argument("Invalid argument");
}
