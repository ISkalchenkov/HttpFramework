#include <cstring>

#include "exception.hpp"

namespace http {

    Exception::Exception(const std::string& msg, ExType type) {
        if (type == ExType::ERRNO) {
            msg_ = msg + strerror(errno);
        } else {
            msg_ = msg;
        }
    }

    const char* Exception::what() const noexcept {
        return msg_.c_str();
    }

    FdError::FdError(const std::string& msg)
        : Exception(msg) {
    }

    ServerError::ServerError(const std::string& msg, ExType type)
        : Exception(msg, type) {
    }

} // namespace http
