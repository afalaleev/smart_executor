#include "smart_executor/worker/worker_s.hpp"

namespace smart_executor {
    namespace worker {
        resumable::resume_result worker_s::resume(execution_device *e, std::size_t max_throughput) {
            if (e) {
                execution_device_ = e;
            }

            for (size_t handled_msgs = 0; handled_msgs < max_throughput;) {

                if (msg_queue.empty(true)) {
                    return resume_result::awaiting;
                }
                auto task = std::move(msg_queue.get());
                task();
                ++handled_msgs;
            }

            return resume_result::resume;

        };

        resumable::subtype_t worker_s::subtype() const {
            return subtype_t::work;
        };

        worker_s::worker_s(coordinator::abstract_coordinator *coordinator_) : coordinator_(coordinator_) {
            assert(this->coordinator_ != nullptr);
        }
    }
}