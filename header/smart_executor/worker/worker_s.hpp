#ifndef COORDINATOR_WORKER_S_HPP
#define COORDINATOR_WORKER_S_HPP

#include "smart_executor/resumable.hpp"
#include "smart_executor/execution_device.hpp"
#include "smart_executor/coordinator/abstract_coordinator.hpp"
#include "mail_box.hpp"
#include <functional>
#include <cassert>


namespace smart_executor {
    namespace worker {

        class worker_s : public resumable{
        public:
            using queue_type = mail_box<std::function<void()>>;

            worker_s(coordinator::abstract_coordinator *coordinator_);

            subtype_t subtype() const override final;

            resume_result resume(execution_device *e, std::size_t max_throughput) override final;

            virtual ~worker_s() = default;

        protected:
            template<typename F>
            void add(F &&f) {
                msg_queue.put(std::move(f));
                if (execution_device_ != nullptr) {
                    execution_device_->exec_later(this);
                } else if (coordinator_ != nullptr) {
                    coordinator_->enqueue(this);
                }
            }

        private:
            queue_type msg_queue;
            execution_device *execution_device_;
            coordinator::abstract_coordinator *coordinator_;
        };
    }
}

#endif
