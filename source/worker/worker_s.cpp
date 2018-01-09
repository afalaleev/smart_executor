#include "smart_executor/worker/worker_s.hpp"

namespace smart_executor {
    namespace worker {
        resumable::resume_result worker_s::resume(execution_device *e, std::size_t max_throughput) {
            if (e) {
                execution_device_ = e;
            }

            size_t handled_msgs = 0;
            auto task_list = msg_queue.get(max_throughput);
            for (auto& task: task_list) {
                task();
                ++handled_msgs;
            }

            if (handled_msgs < max_throughput) {
                return resume_result::awaiting;
            }
            return resume_result::done;
        };

        resumable::subtype_t worker_s::subtype() const {
            return subtype_t::work;
        };

        worker_s::worker_s(coordinator::abstract_coordinator *coordinator_) : coordinator_(coordinator_), execution_device_(nullptr) {
            assert(this->coordinator_ != nullptr);
        }
    }
}
