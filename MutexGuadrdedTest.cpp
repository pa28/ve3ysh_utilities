//
// Created by richard on 09/04/23.
//
#include "BetterMain/BMain.h"
#include "ysh/MutexGuarded.h"

using namespace ysh;

namespace better_main {
    [[nodiscard]] int start(std::span<const std::string_view>) noexcept {
        MutexGuarded<int> thing1{};
        thing1.with([](int &value) {
            std::cout << "thing1 was: " << value;
            value = 5;
            std::cout << ", is: " << value << '\n';
            return value;
        });

        MutexGuarded<int> thing2{7};
        thing2.with([](int &value) {
            std::cout << "thing2 was: " << value;
            value = 11;
            std::cout << ", is: " << value << '\n';
            return value;
        });

        const MutexGuarded<int> thing3{3};
        std::cout << "thing3 is: " << thing3.with([](const int &value){ return value; }) << '\n';

        if (auto result = thing2.try_with([](int &value) {
            value = 5;
            return value;
        }); result)
            std::cout << "try_with - thing2 is: " << result.value()  << '\n';
        else
            std::cout << "try_with - thing2 not locked.\n";
        return 0;
    }
}