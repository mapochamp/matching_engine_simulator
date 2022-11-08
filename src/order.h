#pragma once
#include <string>
/*
    ACTION [OID [SYMBOL SIDE QTY PX]]

    ACTION: single character value with the following definitions
    O - place order, requires OID, SYMBOL, SIDE, QTY, PX
    X - cancel order, requires OID
    P - print sorted book (see example below)

#NOTES: map
    OID: positive 32-bit integer value which must be unique for all orders

#NOTES: map
    SYMBOL: alpha-numeric string value. Maximum length of 8.

    SIDE: single character value with the following definitions
    B - buy
    S - sell

    QTY: positive 16-bit integer value

    PX: positive double precision value (7.5 format)
*/

enum OrderType { O, X };
enum Side { buy, sell };

struct Order {
    OrderType order_type;
    uint32_t oid;
    std::string symbol;
    Side side;
    uint16_t qty;
    double px;
    
    Order(OrderType order_type, uint32_t oid, std::string symbol,
            Side side, uint16_t qty, double px) :
        order_type { order_type },
        oid { oid },
        symbol { symbol },
        side { side },
        qty { qty },
        px { px }
    { }
    
    Order(OrderType order_type, uint32_t oid) :
        order_type { order_type },
        oid { oid }
    { }

    Order()
    { }

};
