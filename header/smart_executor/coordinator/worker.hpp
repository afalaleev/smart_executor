#ifndef COORDINATOR_WORKER_HPP
#define COORDINATOR_WORKER_HPP

#include <thread>
#include <cassert>
#include "smart_executor/execution_device.hpp"
#include "smart_executor/resumable.hpp"

namespace smart_executor {
    namespace coordinator {
        template<typename Policy>
        class worker : public execution_device {
        public:
            using job_ptr = resumable *;
            using coordinator_ptr = coordinator <Policy> *;
            using policy_data = typename Policy::worker_data;


            worker(std::size_t worker_id, coordinator_ptr worker_parent, std::size_t throughput)
                    : max_throughput_(throughput),
                      id_(worker_id),
                      parent_(worker_parent),
                      data_(worker_parent) {}

            void start() {
                //    assert(this_thread_.get_id() == std::thread::id{});
                auto this_worker = this;
                this_thread_ = std::thread{[this_worker] {
                    this_worker->run();
                }};
            }

            worker(const worker &) = delete;

            worker &operator=(const worker &) = delete;


            void external_enqueue(job_ptr job) {
                assert(job != nullptr);
                policy_.external_enqueue(this, job);
            }


            void exec_later(job_ptr job) override {
                assert(job != nullptr);
                policy_.internal_enqueue(this, job);
            }

            coordinator_ptr parent() {
                return parent_;
            }

            std::size_t id() const {
                return id_;
            }

            std::thread &get_thread() {
                return this_thread_;
            }

            policy_data &data() {
                return data_;
            }

            std::size_t max_throughput() {
                return max_throughput_;
            }

        private:
            void run() {
                for (;;) {
                    auto job = policy_.dequeue(this);
                    assert(job != nullptr);
                    assert(job->subtype() != resumable::subtype_t::work_io);
                    policy_.before_resume(this, job);
                    auto res = job->resume(this, max_throughput_);
                    policy_.after_resume(this, job);
                    switch (res) {
                        case resumable::resume_result::resume: {
                            policy_.resume_job_later(this, job);
                            break;
                        }
                        case resumable::resume_result::done: {
                            policy_.after_completion(this, job);
                            break;
                        }
                        case resumable::resume_result::awaiting: {
                            break;
                        }
                        case resumable::resume_result::shutdown: {
                            policy_.after_completion(this, job);
                            policy_.before_shutdown(this);
                            return;
                        }
                    }
                }
            }

            std::size_t max_throughput_;
            std::thread this_thread_;
            std::size_t id_;
            coordinator_ptr parent_;
            policy_data data_;
            Policy policy_;
        };
    }
}

#endif //COORDINATOR_WORKER_HPP
