//
// Created by antarctica on 04.10.2021.
//

#ifndef HTTP_FILE_DESCRIPTOR_HPP
#define HTTP_FILE_DESCRIPTOR_HPP

namespace http::utils {

    class FileDescriptor {
    public:
        FileDescriptor() noexcept;
        FileDescriptor(int fd) noexcept;

        FileDescriptor(const FileDescriptor& rhs) = delete;
        FileDescriptor& operator=(const FileDescriptor& rhs) = delete;

        FileDescriptor(FileDescriptor&& rhs) noexcept;
        FileDescriptor& operator=(FileDescriptor&& rhs);

        FileDescriptor& operator=(int fd);

        operator int() const;

        ~FileDescriptor() noexcept;

        [[nodiscard]] bool is_opened() const;
        [[nodiscard]] int get_fd() const;
        void set_fd(int fd);
        int extract();

        void close();

    private:
        int fd_;
    };

} // namespace http::utils

#endif // HTTP_FILE_DESCRIPTOR_HPP
