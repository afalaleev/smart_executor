#ifndef COORDINATOR_COORDINATOR_HPP
#define COORDINATOR_COORDINATOR_HPP

#include <cstddef>
#include <atomic>
#include <vector>
#include "abstract_coordinator.hpp"
#include "worker.hpp"

namespace smart_executor {
    namespace coordinator {
        template<typename Policy>
        class coordinator : public abstract_coordinator {
        public:
            coordinator(std::size_t num_worker_threads, std::size_t max_throughput_param) : abstract_coordinator(
                    num_worker_threads, max_throughput_param), data_(this) {
            }

            using super = abstract_coordinator;

            using policy_data = typename Policy::coordinator_data;

            using worker_type = worker<Policy>;

            worker_type *worker_by_id(std::size_t x) {
                return workers_[x].get();
            }

            policy_data &data() {
                return data_;
            }

        protected:
            virtual ~coordinator() = default;

            void start() override {
                auto num = num_workers();
                workers_.reserve(num);
                for (size_t i = 0; i < num; ++i) {
                    workers_.emplace_back(new worker_type(i, this, max_throughput()));
                }
                for (auto &w : workers_) {
                    w->start();
                }
                super::start();
            }

            void enqueue(resumable *ptr) override {
                policy_.central_enqueue(this, ptr);
            }

        private:
            std::vector<std::unique_ptr<worker_type>> workers_;
            policy_data data_;
            Policy policy_;
        };
    }
}
#endif //COORDINATOR_COORDINATOR_HPP
