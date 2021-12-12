#include <utility>
#include <unistd.h>
#include <cerrno>
#include <iostream>

#include "utils/file_descriptor.hpp"
#include "exception.hpp"

namespace http::utils {

    FileDescriptor::FileDescriptor() noexcept
        : fd_(-1) {
    }

    FileDescriptor::FileDescriptor(int fd) noexcept
        : fd_(fd) {
    }

    FileDescriptor::FileDescriptor(FileDescriptor&& rhs) noexcept
        : fd_(std::exchange(rhs.fd_, -1)) {
    }

    FileDescriptor& FileDescriptor::operator=(FileDescriptor&& rhs) {
        if (&rhs == this)
            return *this;

        std::swap(fd_, rhs.fd_);
        rhs.close();
        return *this;
    }

    FileDescriptor& FileDescriptor::operator=(int fd) {
        close();
        fd_ = fd;
        return *this;
    }

    FileDescriptor::operator int() const {
        return fd_;
    }

    FileDescriptor::~FileDescriptor() noexcept {
        try {
            close();
        } catch (const FdError& err) {
            std::cerr << err.what() << std::endl;
        }
    }

    bool FileDescriptor::is_opened() const {
        return fd_ != -1;
    }

    int FileDescriptor::get_fd() const {
        return fd_;
    }

    void FileDescriptor::set_fd(int fd) {
        close();
        fd_ = fd;
    }

    int FileDescriptor::extract() {
        return std::exchange(fd_, -1);
    }

    void FileDescriptor::close() {
        if (fd_ == -1) {
            return;
        }
        while (::close(fd_) != 0) {
            if (errno == EINTR)
                continue;
            throw FdError("Error closing file descriptor: ");
        }
        fd_ = -1;
    }

} // namespace http::utils
