#ifndef COORDINATOR_EXECUTION_UNIT_HPP
#define COORDINATOR_EXECUTION_UNIT_HPP

#include "forward.hpp"

namespace smart_executor {
    struct execution_device {

        execution_device(execution_device &&) = delete;

        execution_device(const execution_device &) = delete;

        execution_device() = default;

        virtual ~execution_device() {};

        virtual void exec_later(resumable *ptr) = 0;
    };
}
#endif //COORDINATOR_EXECUTION_UNIT_HPP
