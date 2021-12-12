#ifndef HTTP_SERVER_WORKER_HPP
#define HTTP_SERVER_WORKER_HPP

#include <unordered_map>
#include <sys/epoll.h>

#include "utils/file_descriptor.hpp"
#include "coroutine/coroutine.hpp"
#include "server/client_handler.hpp"

namespace http {

    using Callback = std::function<HttpResponse(const HttpRequest&)>;
    using TimeoutType = std::chrono::milliseconds;
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

    struct WorkerContex {
        TimeoutType read_timeout;
        TimeoutType write_timeout;
        Callback on_request;
    };

    enum class Activity {
        READ,
        WRITE
    };

    struct ClientActivity {
        TimePoint last_resume;
        Activity activity;
        bool is_timed_out;
    };

    class ServerWorker {
    public:
        ServerWorker(const utils::FileDescriptor& server_fd, const WorkerContex& ctx, const bool& done);
        void run();

    private:
        void create_epoll();
        void add_epoll(int fd, uint32_t events);
        void handle_events(epoll_event* events, int nfds);
        void accept_clients();
        void handle_client(int fd, uint32_t events);
        void check_timeouts();

        [[nodiscard]] bool is_read_time_out(TimePoint last_resume) const;
        [[nodiscard]] bool is_write_time_out(TimePoint last_resume) const;

        void timeout_erase_client(int fd);

    private:
        utils::FileDescriptor epoll_fd_;
        const utils::FileDescriptor& server_fd_;

        std::unordered_map<int, Coroutine::routine_t> clients_;
        std::unordered_map<int, ClientActivity> clients_acts_;

        const WorkerContex ctx_;
        const bool& done_;
    };

} // namespace http

#endif // HTTP_SERVER_WORKER_HPP
