#include <OrderBook.hpp>
int main() {
    OrderBook orderBook;

    orderBook.insertOrder(Order::createOrder(1000, 5, OrderType::LIMIT, Side::SELL));
}