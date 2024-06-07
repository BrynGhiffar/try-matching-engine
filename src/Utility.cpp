#include <Prelude.hpp>
#include <Utility.hpp>
#include <Order.hpp>

namespace uuid {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);
}

std::string uuid::generate_uuid_v4() {
    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    return ss.str();
}

Quantity getOrdersQty(const std::vector<Order>& orders) {
    return std::accumulate(orders.cbegin(), orders.cend(), 0, [](Quantity qty, const Order& order) {
        return qty + order.qty;
    });
}


std::ostream& operator<<( std::ostream& dest, __int128_t value )
{
    std::ostream::sentry s( dest );
    if ( s ) {
        __uint128_t tmp = value < 0 ? -value : value;
        char buffer[ 128 ];
        char* d = std::end( buffer );
        do
        {
            -- d;
            *d = "0123456789"[ tmp % 10 ];
            tmp /= 10;
        } while ( tmp != 0 );
        if ( value < 0 ) {
            -- d;
            *d = '-';
        }
        int len = std::end( buffer ) - d;
        if ( dest.rdbuf()->sputn( d, len ) != len ) {
            dest.setstate( std::ios_base::badbit );
        }
    }
    return dest;
}

std::ostream& operator<<(std::ostream& out, const Order& order) {
    out << "Order { ";
    out << "id = " << order.orderId;
    out << ", price = " << order.price;
    out << ", qty = " << order.qty;
    out << ", orderType = ";
    if (order.orderType == OrderType::LIMIT) {
        out << "LIMIT";
    } else if (order.orderType == OrderType::MARKET) {
        out << "MARKET";
    }
    out << ", side = ";
    if (order.side == Side::SELL) {
        out << "SELL";
    } else {
        out << "BUY";
    }
    out << ", orderStatus = ";
    if (order.orderStatus == OrderStatus::AVAILABLE) {
        out << "AVAILABLE";
    } else if (order.orderStatus == OrderStatus::FILLED) {
        out << "FILLED";
    } else if (order.orderStatus == OrderStatus::PARTIAL_FILLED) {
        out << "PARTIAL_FILLED";
    }
    out << " }";
    return out;
}