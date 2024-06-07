#!/usr/bin/sh

clang++ -Iinclude ./src/Main.cpp ./src/Utility.cpp ./src/OrderBook.cpp ./src/Order.cpp -std=c++20 -o ./build/main