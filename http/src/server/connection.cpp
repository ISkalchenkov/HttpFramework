#include <unistd.h>
#include <fcntl.h>
#include <filesystem>
#include <sys/sendfile.h>

#include "server/connection.hpp"
#include "exception.hpp"

namespace http {

    Connection::Connection(int fd) : socket_(fd) {
    }

    Connection::Connection(Connection&& rhs) noexcept : socket_(std::move(rhs.socket_)) {
    }

    Connection& Connection::operator=(Connection&& rhs) {
        if (&rhs == this) {
            return *this;
        }

        close();
        socket_ = std::move(rhs.socket_);
        return *this;
    }

    void Connection::close() {
        socket_.close();
    }

    size_t Connection::write(const void* data, size_t len) {
        while(true) {
            ssize_t bytes_writen = ::write(socket_, data, len);
            if (bytes_writen == -1) {
                if (errno == EINTR)
                    continue;
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                    return 0;
                throw ServerError("Error writing to socket: ");
            }
            return bytes_writen;
        }
    }

    size_t Connection::send_file(std::string_view path, off_t offset, uintmax_t bytes_left) {
        utils::FileDescriptor file_fd;
        while (true) {
            file_fd = ::open(path.data(), O_RDONLY);
            if (file_fd == -1) {
                if (errno == EINTR)
                    continue;
                throw ServerError("Error opening file for sending: " + static_cast<std::string>(path) + ": ");
            }
            break;
        }

        uintmax_t file_size = std::filesystem::file_size(path);
        size_t total_bytes_writen = 0;

        while (total_bytes_writen != file_size) {
            ssize_t bytes_writen = ::sendfile(socket_, file_fd, &offset, bytes_left);
            if (bytes_writen < 0) {
                if (errno == EAGAIN) {
                    return total_bytes_writen;
                }
                throw ServerError("Error sending file to socket: ");
            }

            total_bytes_writen += bytes_writen;
            bytes_left -= bytes_writen;
            if (!bytes_left) {
                break;
            }
        }
        return total_bytes_writen;
    }

    size_t Connection::read(void* data, size_t len) {
        while (true) {
            ssize_t bytes_read = ::read(socket_, data, len);
            if (bytes_read == -1) {
                if (errno == EINTR)
                    continue;
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                    return 0;
                throw ServerError("Error reading from socket: ");
            }
            return bytes_read;
        }
    }

    std::string Connection::read(size_t limit) {
        std::string result;

        while (result.size() < limit) {
            std::string buff(1024, '\0');
            size_t bytes_read = read(buff.data(), buff.size());
            if (bytes_read < buff.size()) {
                if (result.empty()) {
                    buff.resize(bytes_read);
                    return buff;
                }
                buff.resize(bytes_read);
                result.append(buff);
                break;
            }
            result.append(buff);
        }
        return result;
    }

    int Connection::get_fd() {
        return socket_;
    }

} // namespace http
