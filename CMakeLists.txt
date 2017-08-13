cmake_minimum_required(VERSION 2.8)

set(project smart_executor)
set(path header/smart_executor)
project(${project})


include_directories(header)

set(HEADER_FILES

        ${path}/coordinator/abstract_coordinator.hpp
        ${path}/coordinator/coordinator.hpp
        ${path}/coordinator/worker.hpp

        ${path}/policy/scheduler_policy.hpp
        ${path}/policy/unprofiled.hpp
        ${path}/policy/work_sharing.hpp

        ${path}/worker/worker_s.hpp
        ${path}/worker/mail_box.hpp

        ${path}/execution_device.hpp
        ${path}/forward.hpp
        ${path}/resumable.hpp
)


set(SOURCE_FILES
        source/worker/worker_s.cpp
        source/resumable.cpp
)

set(CMAKE_CXX_STANDARD 11)

add_library(${project} ${HEADER_FILES} ${SOURCE_FILES})
add_subdirectory(example)
