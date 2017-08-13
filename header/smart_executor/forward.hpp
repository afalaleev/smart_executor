#ifndef COORDINATOR_FORWARD_HPP
#define COORDINATOR_FORWARD_HPP
namespace smart_executor {
    class resumable;

    class execution_device;

    namespace coordinator {
        class abstract_coordinator;

        template<typename Policy>
        class coordinator;
    }

}
#endif //COORDINATOR_FORWARD_HPP
