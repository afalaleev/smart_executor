#include "smart_executor/resumable.hpp"

namespace smart_executor {
    resumable::subtype_t resumable::subtype() const {
        return subtype_t::unspecified;
    }
}