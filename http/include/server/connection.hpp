#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <cstddef>
#include <string>

#include "utils/file_descriptor.hpp"

namespace http {

    class Connection {
    public:
        Connection(int fd);

        Connection(Connection&& rhs) noexcept;
        Connection& operator=(Connection&& rhs);

        Connection(const Connection& rhs) = delete;
        Connection& operator=(const Connection& rhs) = delete;

        ~Connection() noexcept = default;

        size_t write(const void* data, size_t len);
        size_t send_file(std::string_view path, off_t offset, uintmax_t bytes_left);

        size_t read(void* data, size_t len);
        std::string read(size_t limit);

        int get_fd();

        void close();

    private:
        utils::FileDescriptor socket_;
    };

} // namespace http

#endif // HTTP_CONNECTION_HPP
