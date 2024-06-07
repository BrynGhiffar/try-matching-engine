#include <Order.hpp>
#include <Utility.hpp>

Order Order::createOrder(Price price, Quantity qty, OrderType orderType, Side side) {
    OrderId orderId = uuid::generate_uuid_v4();
    OrderStatus orderStatus = OrderStatus::AVAILABLE;
    return Order { orderId, price, qty, orderType, side, orderStatus };
}
