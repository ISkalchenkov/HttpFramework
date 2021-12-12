#ifndef HTTP_EXCEPTION_HPP
#define HTTP_EXCEPTION_HPP

#include <exception>
#include <string>

namespace http {

    enum class ExType {
        NO_ERRNO,
        ERRNO
    };

    class Exception : public std::exception {
    public:
        explicit Exception(const std::string& msg, ExType type = ExType::ERRNO);

        ~Exception() noexcept override = default;

        [[nodiscard]] const char* what() const noexcept override;

    private:
        std::string msg_;
    };

    class FdError : public Exception {
    public:
        explicit FdError(const std::string& msg);
    };

    class ServerError : public Exception {
    public:
        explicit ServerError(const std::string& msg, ExType type = ExType::ERRNO);
    };

} // namespace http

#endif // HTTP_EXCEPTION_HPP
