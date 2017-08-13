
#include <condition_variable>
#include <mutex>

#include "database.hpp"


int main() {
    smart_executor::coordinator::abstract_coordinator *coordinator_ = new smart_executor::coordinator::coordinator<smart_executor::policy::work_sharing>(1, 10);
    coordinator_->start();

    database<object> fff(coordinator_);
    auto *tmp = new object(12);

    fff.insert(
            *tmp,
            [&](object &o) {
                std::cerr << o.number << std::endl;
            },
            true
    );

    fff.modify(
            [&]() -> std::vector<object> {
                std::vector<object> tmp;
                tmp.emplace_back(34);
                return tmp;
            },
            [&](std::vector<object> o) {
                std::cerr << o[0].number << std::endl;
            },
            true
    );

    std::mutex mtx;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait_for(lck, std::chrono::seconds(10000));
    return 0;
}