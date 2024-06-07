#pragma once
#include <Prelude.hpp>

typedef __int128_t Price;
typedef __int128_t Quantity;
typedef std::string OrderId;

enum class OrderType {
    MARKET = 1,
    LIMIT = 2,
};

enum class Side {
    BUY = 1,
    SELL = 2
};

enum class OrderStatus {
    AVAILABLE = 1,
    PARTIAL_FILLED = 2,
    FILLED = 3,
};

struct Order {

    // If market order the price is determined by the order book, 
    OrderId orderId;
    Price price;
    Quantity qty;
    OrderType orderType;
    Side side;
    OrderStatus orderStatus;
    static Order createOrder(Price price, Quantity qty, OrderType orderType, Side side);
};
