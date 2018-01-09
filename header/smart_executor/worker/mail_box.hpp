#ifndef SMART_EXECUTOR_MAIL_BOX_HPP
#define SMART_EXECUTOR_MAIL_BOX_HPP

#include <mutex>
#include <condition_variable>
#include <forward_list>

namespace smart_executor {
    namespace worker {
        template<typename T>
        class mail_box {
        public:
            using storage_type=std::forward_list<T>;

            storage_type get(size_t max_throughput) {
                storage_type out;
                {
                    std::lock_guard<std::mutex>(this->mtx);
                    for (size_t i = 0; i < max_throughput; ++i) {
                        if (in.empty()) {
                            break;
                        }
                        out.emplace_front(std::move(in.front()));
                        in.pop_front();
                    }
                }
                return out;
            }

            void put(T &&element) {
                {
                    std::lock_guard<std::mutex>(this->mtx);
                    in.push_front(std::move(element));
                    cv.notify_one();
                }
            }

        private:

            /**--------------------------*/
            std::mutex mtx;
            std::condition_variable cv;
            /**--------------------------*/
            storage_type in;
        };
    }
}
#endif //SMART_EXECUTOR_MAIL_BOX_HPP
