#pragma once
#include <Prelude.hpp>
#include <Order.hpp>
#include <Utility.hpp>

class OrderBook {
    private:
        std::map<Price, std::vector<Order>, std::less<Price>> ask;
        std::map<Price, std::vector<Order>, std::greater<Price>> bid; 
    public:
        void insertOrder(Order);
        bool isPriceMatch(const Order&, Price) const;

        // for debugging
        friend std::ostream& operator<<(std::ostream&, const OrderBook&);

        // Currently for testing purposes
        std::optional<Price> getBestBidPrice() const;
        std::optional<Price> getBestAskPrice() const;
        std::vector<Order> getBidOrders() const;
        std::vector<Order> getAskOrders() const;

        template <typename IncomingOrderBookSide, typename OrderBookSide>
        void fillOrder(
            Order order, 
            IncomingOrderBookSide& incomingOrderBookSide, 
            OrderBookSide& orderBookSide
        ) {

            std::vector<Order> possibleFills;
            Quantity incomingOrderQuantity = order.qty;
            for (auto priceIt = orderBookSide.begin(); priceIt != orderBookSide.end() && incomingOrderQuantity > 0; priceIt++) {
                auto [ price, orders ] = *priceIt;
                if (!isPriceMatch(order, price)) break;
                for (auto it = orders.begin(); it != orders.end(); it++) {
                    possibleFills.push_back(*it);
                    incomingOrderQuantity = std::max(incomingOrderQuantity - it->qty, static_cast<Quantity>(0));
                    if (incomingOrderQuantity == 0) break;
                }
            }

            // how do we remove orders that have been filled from the orderbook?
            // remove first 0 <= n <= k - 1 orders.

            Quantity possibleFillsQtyTotal = getOrdersQty(possibleFills);
            size_t removeFirstOrders = (possibleFillsQtyTotal > order.qty) ? possibleFills.size() - 1 : possibleFills.size();
            Quantity partialFilledQty = std::max(possibleFillsQtyTotal - order.qty, static_cast<Quantity>(0));
            size_t k = 0;

            auto startEraseOrderBookSide = orderBookSide.begin();
            auto endEraseOrderBookSide = orderBookSide.begin();
            for (auto priceIt = orderBookSide.begin(); priceIt != orderBookSide.end() && k < removeFirstOrders; priceIt++) {
                auto& orders = priceIt->second;
                auto price = priceIt->first;
                size_t rm = std::min(removeFirstOrders - k, orders.size());
                // std::cout << "Removed rm orders from price index: " << removeFirstOrders << std::endl;
                orders.erase(orders.begin(), orders.begin() + rm);
                k += rm;

                // remove empty price indexes
                if (orders.size() == 0) endEraseOrderBookSide++;
            }

            // better way to do this?
            
            // This may not be that efficient?
            orderBookSide.erase(startEraseOrderBookSide, endEraseOrderBookSide);

            if (partialFilledQty > 0) {
                auto it = orderBookSide.begin();
                auto& orders = it->second;
                auto firstOrder = orders.begin();
                firstOrder->orderStatus = OrderStatus::PARTIAL_FILLED;
                firstOrder->qty = partialFilledQty;
                std::cout << "INFO: Inbook Order Partial Filled" << std::endl;
                std::cout << *firstOrder << std::endl;
            }

            if (incomingOrderQuantity > 0) {
                if (incomingOrderQuantity != order.qty) {
                    std::cout << "INFO: Incoming Order Partial Filled" << std::endl;
                    order.orderStatus = OrderStatus::PARTIAL_FILLED;
                    std::cout << order << std::endl;
                    // order.orderStatus = 
                }
                order.qty = incomingOrderQuantity;
                if (incomingOrderBookSide.find(order.price) == incomingOrderBookSide.cend()) {
                    incomingOrderBookSide.insert(std::pair<Price, std::vector<Order>>(order.price, { order }));
                } else {
                    incomingOrderBookSide.at(order.price).push_back(order);
                }
            }
        }
};

