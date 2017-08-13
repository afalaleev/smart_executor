#ifndef COORDINATOR_RESUMABLE_HPP
#define COORDINATOR_RESUMABLE_HPP

#include <cstddef>
#include <cstdint>
#include "forward.hpp"

namespace smart_executor {
    class resumable {
    public:
        enum class resume_result : uint8_t {
            resume,
            awaiting,
            done,
            shutdown
        };

        enum class subtype_t : uint8_t {
            unspecified,
            work,
            work_io,
            function_object
        };

        virtual ~resumable() = default;

        virtual subtype_t subtype() const;

        virtual resume_result resume(execution_device *, std::size_t max_throughput) = 0;

    };
}
#endif //COORDINATOR_RESUMABLE_HPP
