#ifndef COORDINATOR_SCHEDULER_POLICY_HPP
#define COORDINATOR_SCHEDULER_POLICY_HPP

#include "../forward.hpp"

namespace smart_executor {
    namespace policy {
        class scheduler_policy {
        public:

            struct coordinator_data {
                explicit coordinator_data(abstract_coordinator *);
            };

            struct worker_data {
                explicit worker_data(abstract_coordinator *);
            };

            template<class Coordinator>
            void central_enqueue(Coordinator *self, resumable *job);


            template<class Worker>
            void external_enqueue(Worker *self, resumable *job);

            template<class Worker>
            void internal_enqueue(Worker *self, resumable *job);

            template<class Worker>
            void resume_job_later(Worker *self, resumable *job);

            template<class Worker>
            resumable *dequeue(Worker *self);

            template<class Worker>
            void before_shutdown(Worker *self);

            template<class Worker>
            void before_resume(Worker *self, resumable *job);

            template<class Worker>
            void after_resume(Worker *self, resumable *job);


            template<class Worker>
            void after_completion(Worker *self, resumable *job);


            template<class Worker, typename UnaryFunction>
            void foreach_resumable(Worker *self, UnaryFunction f);

            template<class Coordinator, typename UnaryFunction>
            void foreach_central_resumable(Coordinator *self, UnaryFunction f);

        };
    }
}
#endif //COORDINATOR_SCHEDULER_POLICY_HPP
