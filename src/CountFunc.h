//
// Created by fulva on 12/2/19.
//

#ifndef OMNIMCU_Z_COUNTFUNC_H
#define OMNIMCU_Z_COUNTFUNC_H

#include <functional>
#include <utility>

class CountFunc {
public:
    inline void operator()() {
        ++count;
        if (count == limit) {
            count = 0;
            wrapFunc();
        }
    }

    CountFunc(std::function<void()> wrapFunc, int limit) : wrapFunc{std::move(wrapFunc)}, limit{limit} {}

private:
    int count{};
    int limit{};
    std::function<void()> wrapFunc;
};

#endif //OMNIMCU_Z_COUNTFUNC_H
