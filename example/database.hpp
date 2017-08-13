#ifndef SMART_EXECUTOR_DATABASE_HPP
#define SMART_EXECUTOR_DATABASE_HPP

#include "smart_executor/worker/worker_s.hpp"
#include "smart_executor/coordinator/coordinator.hpp"
#include "smart_executor/policy/work_sharing.hpp"
#include <iostream>

struct object {

    object(int number):number(number){}

    int number;
};


template<typename T>
class database final : public smart_executor::worker::worker_s {
public:
    explicit database(smart_executor::coordinator::abstract_coordinator *coordinator_) : worker_s(coordinator_) {}

    //write
    template<typename Object, typename Modifier>
    void insert(Object &&o, Modifier &&f, bool async = false) {
        if (async) {
            add(std::bind(f,o));
        } else {
            f(o);
        }
    }

    //write
    template<typename Selector, typename Modifier>
    void modify(Selector &&s, Modifier &&f, bool async = false) {
        if (async) {
            add(std::bind(f,std::bind(s)));
        } else {
            f(s());
        }
    }


    //read
    template<typename Selector, typename Modifier>
    void select(Selector &&s, Modifier &&f, bool async = false) {
        if (async) {
            add(std::bind(f,std::bind(s)));
        } else {
            f(s());
        }
    }


    ~database() = default;

public:
    std::vector<T> row;
};

#endif //SMART_EXECUTOR_DATABASE_HPP
