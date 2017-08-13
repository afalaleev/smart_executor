#ifndef COORDINATOR_WORK_SHARING_HPP
#define COORDINATOR_WORK_SHARING_HPP

#include <list>
#include <mutex>
#include <cstddef>
#include <condition_variable>

#include "unprofiled.hpp"

namespace smart_executor {
    namespace policy {
        class work_sharing : public unprofiled {
        public:
            using queue_type = std::list<resumable *>;

            ~work_sharing() {}

            struct coordinator_data {
                inline explicit coordinator_data(coordinator::abstract_coordinator *) {
                }

                queue_type queue;
                std::mutex lock;
                std::condition_variable cv;
            };

            struct worker_data {
                inline explicit worker_data(coordinator::abstract_coordinator *) {
                }
            };

            template<class Coordinator>
            void enqueue(Coordinator *self, resumable *job) {
                queue_type l;
                l.push_back(job);
                std::unique_lock<std::mutex> guard(cast(self).lock);
                cast(self).queue.splice(cast(self).queue.end(), l);
                cast(self).cv.notify_one();
            }

            template<class Coordinator>
            void central_enqueue(Coordinator *self, resumable *job) {
                enqueue(self, job);
            }

            template<class Worker>
            void external_enqueue(Worker *self, resumable *job) {
                enqueue(self->parent(), job);
            }

            template<class Worker>
            void internal_enqueue(Worker *self, resumable *job) {
                enqueue(self->parent(), job);
            }

            template<class Worker>
            void resume_job_later(Worker *self, resumable *job) {
                enqueue(self->parent(), job);
            }

            template<class Worker>
            resumable *dequeue(Worker *self) {
                auto &parent_data = cast(self->parent());
                std::unique_lock<std::mutex> guard(parent_data.lock);
                parent_data.cv.wait(guard, [&] { return !parent_data.queue.empty(); });
                resumable *job = parent_data.queue.front();
                parent_data.queue.pop_front();
                return job;
            }

            template<class Worker, class UnaryFunction>
            void foreach_resumable(Worker *, UnaryFunction) {
            }

            template<class Coordinator, class UnaryFunction>
            void foreach_central_resumable(Coordinator *self, UnaryFunction f) {
                auto &queue = cast(self).queue;
                auto next = [&]() -> resumable * {
                    if (queue.empty()) {
                        return nullptr;
                    }
                    auto front = queue.front();
                    queue.pop_front();
                    return front;
                };
                std::unique_lock<std::mutex> guard(cast(self).lock);
                for (auto job = next(); job != nullptr; job = next()) {
                    f(job);
                }
            }
        };
    }
}
#endif //COORDINATOR_WORK_SHARING_HPP
