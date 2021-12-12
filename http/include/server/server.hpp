#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <string_view>
#include <thread>
#include <vector>

#include "server/server_worker.hpp"
#include "protocol/http_request.hpp"
#include "protocol/http_response.hpp"
#include "utils/file_descriptor.hpp"

namespace http {

    class Server {
    public:
        Server(std::string_view address, uint16_t port, int max_conn);
        virtual ~Server() noexcept = default;

        Server(const Server& rhs) = delete;
        Server& operator=(const Server& rhs) = delete;
        Server(Server&& rhs) = delete;
        Server& operator=(Server&& rhs) = delete;

        void open(std::string_view address, uint16_t port);
        void listen(int max_conn);
        void close();
        void join_threads();

        [[nodiscard]] bool is_opened() const noexcept;

        void run(TimeoutType read_timeout = TimeoutType(-1), TimeoutType write_timeout = TimeoutType(-1),
                 size_t thread_count = std::thread::hardware_concurrency());

        virtual HttpResponse on_request(const HttpRequest& r) = 0;

        virtual void signal_handler();

    private:
        utils::FileDescriptor server_fd_;

        std::vector<std::thread> workers_;
        bool done_;
    };

} // namespace http

#endif // HTTP_SERVER_HPP
