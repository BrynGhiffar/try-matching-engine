#!/usr/bin/sh

clang++ -Iinclude ./tests/TestMain.cpp ./src/Utility.cpp ./src/OrderBook.cpp ./src/Order.cpp -std=c++20 -o ./build/test