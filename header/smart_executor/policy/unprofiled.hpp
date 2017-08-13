#ifndef COORDINATOR_UNPROFILED_HPP
#define COORDINATOR_UNPROFILED_HPP

#include "../coordinator/abstract_coordinator.hpp"
#include "../resumable.hpp"

namespace smart_executor {
    namespace policy {
        class unprofiled {
        public:
            virtual ~unprofiled() {}

            template<class Worker>
            void before_shutdown(Worker *) {
            }

            template<class Worker>
            void before_resume(Worker *, resumable *) {
            }


            template<class Worker>
            void after_resume(Worker *, resumable *) {
            }

            template<class Worker>
            void after_completion(Worker *, resumable *) {
            }

        protected:
            // Convenience function to access the data field.
            template<class WorkerOrCoordinator>
            static auto cast(WorkerOrCoordinator *self) -> decltype(self->data()) {
                    return self->data();
            }
        };
    }
}
#endif //COORDINATOR_UNPROFILED_HPP
