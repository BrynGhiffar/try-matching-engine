#pragma once
#include <Prelude.hpp>
#include <Order.hpp>

namespace uuid {
    std::string generate_uuid_v4();
}

Quantity getOrdersQty(const std::vector<Order>&);

// Operator overloads to debug
std::ostream& operator<<(std::ostream&, __int128_t);
std::ostream& operator<<(std::ostream&, const Order&);