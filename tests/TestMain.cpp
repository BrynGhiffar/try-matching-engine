#include <OrderBook.hpp>

void printSuccessFailed(int passes, int expectedPasses) {
    if (passes == expectedPasses) {
        std::cout << "SUCCESS: " << passes << "/" << expectedPasses 
            << " Cases Passed";
    } else {
        std::cout << "FAILED: " << passes << "/" << expectedPasses 
            << " Cases Passed";
    }
    std::cout << std::endl << std::endl;
}

void testOrderInserted() {
    int passes = 0, expectedPassess = 2;

    std::cout << "Test: testOrderInserted" << std::endl;
    OrderBook orderBook;
    orderBook.insertOrder(Order::createOrder(1000, 5, OrderType::LIMIT, Side::SELL));
    auto price = orderBook.getBestAskPrice();
    if (price != 1000) {
        std::cout << "FAIL: Price is not 1000" << std::endl;
    } else {
        passes += 1;
    }

    auto orders = orderBook.getAskOrders();
    if (orders.size() != 1) {
        std::cout << "FAIL: Number of orders is not 1, got: " << orders.size() << std::endl;
    } else {
        passes += 1;
    }

    printSuccessFailed(passes, expectedPassess);
}


void testOrderPerfectMatch() {
    int passes = 0, expectedPassess = 2;
    std::cout << "Test: testOrderPerfectMatch" << std::endl;
    OrderBook orderBook;
    orderBook.insertOrder(Order::createOrder(1000, 5, OrderType::LIMIT, Side::SELL));
    orderBook.insertOrder(Order::createOrder(1000, 5, OrderType::LIMIT, Side::BUY));

    auto asks = orderBook.getAskOrders();
    auto bids = orderBook.getBidOrders();

    if (asks.size() == 0) { 
        passes += 1;
    } else {
        std::cout << "FAIL: After perfect match there should not exist any asks" << std::endl;
    }

    if (bids.size() == 0) {
        passes += 1;
    } else {
        std::cout << "FAIL: After perfect match there should not be any bids" << std::endl;
    }

    printSuccessFailed(passes, expectedPassess);
}

// void testBuyPartialMatch() {
//     int passes = 0, expectedPasses = 2;
// }

int main() {
    testOrderInserted();
    testOrderPerfectMatch();
}