#include <OrderBook.hpp>

void OrderBook::insertOrder(Order order) {
    if (order.side == Side::SELL) {
        fillOrder(std::move(order), ask, bid);
    } else if (order.side == Side::BUY) {
        fillOrder(std::move(order), bid, ask);
    }
}

bool OrderBook::isPriceMatch(const Order& order, Price price) const {
    if (order.side == Side::BUY) return order.price >= price;
    if (order.side == Side::SELL) return order.price <= price;
    return false;
}

std::ostream& operator<<(std::ostream& out, const OrderBook& orderBook) {
    out << "-------------" << std::endl;
    out << "OrderBook" << std::endl;
    out << "ask" << std::endl;
    auto askStart = std::reverse_iterator(orderBook.ask.end());
    auto askEnd = std::reverse_iterator(orderBook.ask.begin());
    auto getQty = [](const Order& order) { return order.qty; };
    for (auto it = askStart; it != askEnd; it++) {
        auto [ price, orders ] = *it;
        Quantity totalQty = getOrdersQty(orders);
        out << static_cast<Price>(price) << ":" << totalQty << std::endl;
    }

    out << "bid" << std::endl;
    for (auto it = orderBook.bid.begin(); it != orderBook.bid.end(); it++) {
        auto [ price, orders ] = *it;
        Quantity totalQty = getOrdersQty(orders);
        out << static_cast<Price>(price) << ":" << totalQty << std::endl;
    }
    out << "-------------" << std::endl;
    return out;
}

std::optional<Price> OrderBook::getBestBidPrice() const {
    if (bid.size() >= 1) {
        auto it = bid.begin();
        return it->first;
    }
    return std::nullopt;
}

std::optional<Price> OrderBook::getBestAskPrice() const {
    if (ask.size() >= 1) {
        auto it = ask.begin();
        return it->first;
    }
    return std::nullopt;
}

std::vector<Order> OrderBook::getBidOrders() const {
    std::vector<Order> resBids;
    for (const auto &[price, orders]: bid) {
        for (const auto &order: orders) {
            resBids.push_back(order);
        }
    }
    return resBids;
}

std::vector<Order> OrderBook::getAskOrders() const {
    std::vector<Order> resAsks;
    for (const auto &[price, orders]: ask) {
        for (const auto& order: orders) {
            resAsks.push_back(order);
        }
    }
    return resAsks;
}