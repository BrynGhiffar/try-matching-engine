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

            // Worst case time complexity here is number of orders in the orderbook, this can occur for example
            // When the incoming order is able to match all orders in the orderbook.
            for (const auto& [ price, orders ]: orderBookSide) {
                // If the current price does not match the order index, this means
                // that subsequent orders in the order book also won't match.
                // Therefore, we can break outside of the loop
                if (!isPriceMatch(order, price)) break;

                for (const auto& order: orders) {
                    possibleFills.push_back(order);
                    incomingOrderQuantity = std::max(incomingOrderQuantity - order.qty, static_cast<Quantity>(0));

                    // if incomingOrderQuantity is empty this means we have all the orders that we want to fill
                    if (incomingOrderQuantity == 0) break;
                }
            }

            Quantity possibleFillsQtyTotal = getOrdersQty(possibleFills); // Total qty of possible orders.
            size_t removeFirstOrders = (possibleFillsQtyTotal > order.qty) ? possibleFills.size() - 1 : possibleFills.size();
            Quantity partialFilledQty = std::max(possibleFillsQtyTotal - order.qty, static_cast<Quantity>(0));
            size_t k = 0;

            auto startEmptyPriceIndex = orderBookSide.begin();
            auto endEmptyPriceIndex = orderBookSide.begin();

            // Worst case time complexity here is the number of price indexes in the orderbook
            for (auto& [ price, orders ]: orderBookSide) {
                if (k >= removeFirstOrders) break;
                size_t rm = std::min(removeFirstOrders - k, orders.size());

                // Orders that are removed from the orderbook can become trades, since they have matched with the incoming order.
                // This is not yet implemented.
                orders.erase(orders.begin(), orders.begin() + rm);
                k += rm;

                if (orders.size() == 0) endEmptyPriceIndex++;
            }
            
            // remove empty price indexes
            orderBookSide.erase(startEmptyPriceIndex, endEmptyPriceIndex);

            // If the last order to be filled still have left overs, then it is partially filled
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
                if (incomingOrderQuantity < order.qty) {
                    // If there are still left overs in incoming order
                    std::cout << "INFO: Incoming Order Partial Filled" << std::endl;
                    order.orderStatus = OrderStatus::PARTIAL_FILLED;
                    std::cout << order << std::endl;
                }
                order.qty = incomingOrderQuantity;

                // If first order in price index, create a new price index
                if (incomingOrderBookSide.find(order.price) == incomingOrderBookSide.cend()) {
                    incomingOrderBookSide.insert(std::pair<Price, std::vector<Order>>(order.price, { order }));
                } else {
                    incomingOrderBookSide.at(order.price).push_back(order);
                }
            }
        }
};