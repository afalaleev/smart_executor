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

            T get(bool sync = false) {
                synchronize(sync);
                T tmp = std::move(local().front());
                local().pop_front();
                return tmp;
            }

            bool empty(bool sync = false) {
                synchronize(sync);
                return local().empty();

            }

            void put(T &&element) {
                {
                    std::lock_guard<std::mutex>(this->mtx);
                    in.push_front(std::move(element));
                    cv.notify_one();
                }
            }

        private:

            storage_type &local() {
                return out;
            }

            void synchronize(bool sync) {
                if (sync) {

                    {
                        std::lock_guard<std::mutex>(this->mtx);
                        for (auto &&i:in) {
                            out.emplace_front(std::move(i));
                        }
                        in.clear();
                        cv.notify_one();
                    }

                }
            }

            /**--------------------------*/
            std::mutex mtx;
            std::condition_variable cv;
            /**--------------------------*/
            storage_type in;
            storage_type out;

        };
    }
}
#endif //SMART_EXECUTOR_MAIL_BOX_HPP
