#include <Prelude.hpp>
#include <Utility.hpp>
#include <OrderBook.hpp>

int main() {
    OrderBook orderBook;
    orderBook.insertOrder(Order::createOrder(1000, 10, OrderType::LIMIT, Side::SELL));
    orderBook.insertOrder(Order::createOrder(1000, 5, OrderType::LIMIT, Side::SELL));
    orderBook.insertOrder(Order::createOrder(990, 10, OrderType::LIMIT, Side::SELL));
    orderBook.insertOrder(Order::createOrder(970, 10, OrderType::LIMIT, Side::BUY));
    orderBook.insertOrder(Order::createOrder(970, 5, OrderType::LIMIT, Side::BUY));
    orderBook.insertOrder(Order::createOrder(980, 10, OrderType::LIMIT, Side::BUY));

    // orderBook.insertOrder(Order::createOrder(10, 100, OrderType::LIMIT, Side::SELL));

    orderBook.insertOrder(Order::createOrder(1000, 12, OrderType::LIMIT, Side::BUY));
    // orderBook.insertOrder(Order::createOrder(990, 5, OrderType::LIMIT, Side::BUY));
    std::cout << orderBook << std::endl;
}