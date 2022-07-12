#include <bits/stdc++.h>
#include "concurrency/value_or_exception.hpp"
#include "concurrency/future.hpp"
#include "concurrency/promise.hpp"

int get_int() {
    return 666;
}

int main() {
    concurrency::Promise<int> p;

    std::thread t1([&] {
        concurrency::Future<int> f = p.getFuture();
        try {
            std::cout << f.get() << std::endl;
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    
    try {
        throw std::out_of_range("you're out!");
    } catch (std::exception& e) {
        p.setException(std::current_exception());
    }

    t1.join();

    return 0;
}