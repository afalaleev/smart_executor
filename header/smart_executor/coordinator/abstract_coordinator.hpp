#ifndef COORDINATOR_ABSTRACT_COORDINATOR_HPP
#define COORDINATOR_ABSTRACT_COORDINATOR_HPP

#include <cstddef>
#include <atomic>
#include <thread>
#include "smart_executor/forward.hpp"

namespace smart_executor {
    namespace coordinator {
        class abstract_coordinator {
        public:
            //abstract_coordinator() : next_worker_(0), max_throughput_(0), num_workers_(0) {}

            abstract_coordinator(std::size_t num_worker_threads, std::size_t max_throughput_param)
                    : max_throughput_(max_throughput_param),
                      num_workers_(((0 == num_worker_threads) ? std::thread::hardware_concurrency() * 2 - 1
                                                              : num_worker_threads)),
                      next_worker_(0) {
            };

            virtual void enqueue(resumable *what) = 0;

            std::size_t max_throughput() const {
                return max_throughput_;
            }

            std::size_t num_workers() const {
                return num_workers_;
            }

            virtual void start() {};

        protected:
            virtual ~abstract_coordinator() = default;

        private:

            std::atomic<std::size_t> next_worker_;

            std::size_t max_throughput_;

            std::size_t num_workers_;
        };
    }
}

#endif //ABSTRACT_COORDINATOR_HPP
