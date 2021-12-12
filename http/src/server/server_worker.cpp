#include <sys/socket.h>
#include <sstream>
#include <chrono>

#include "server/server_worker.hpp"
#include "exception.hpp"

namespace http {

    ServerWorker::ServerWorker(const utils::FileDescriptor& server_fd, const WorkerContex& ctx, const bool& done)
        : server_fd_(server_fd)
        , ctx_(ctx)
        , done_(done) {

        create_epoll();
        add_epoll(server_fd_, EPOLLIN | EPOLLEXCLUSIVE);
    }

    void ServerWorker::run() {
        constexpr size_t EPOLL_SIZE = 128;
        epoll_event events[EPOLL_SIZE];

        TimeoutType epoll_timeout = std::min(ctx_.read_timeout, ctx_.write_timeout);

        while(!done_) {
            int nfds = epoll_wait(epoll_fd_, events, EPOLL_SIZE, epoll_timeout.count());

            if (nfds < 0) {
                if (errno == EINTR)
                    continue;
                throw ServerError("Error waiting epoll: ");
            }

            if (nfds == 0) {
                check_timeouts();
                continue;
            }

            handle_events(events, nfds);
        }

    }

    void ServerWorker::create_epoll() {
        epoll_fd_ = epoll_create(1);
        if (epoll_fd_ < 0) {
            throw ServerError("Error creating epoll: ");
        }
    }

    void ServerWorker::add_epoll(int fd, uint32_t events) {
        struct epoll_event event{};
        event.events = events;
        event.data.fd = fd;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
            throw ServerError("Error adding fd to epoll: ");
        }
    }

    void ServerWorker::accept_clients() {
        while (true) {
            int fd = accept4(server_fd_, nullptr, nullptr, SOCK_NONBLOCK);
            if (fd < 0) {
                if (errno == EINTR)
                    continue;
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                    return;
                throw ServerError("Error accepting connection: ");
            }

            add_epoll(fd, EPOLLIN);

            ClientActivity cl_act {std::chrono::system_clock::now(), Activity::READ, false};
            clients_acts_.emplace(fd, cl_act);
            return;
        }
    }

    void ServerWorker::handle_client(int fd, uint32_t events) {
        if (events & EPOLLIN) {
            try {
                ClientHandler handler(fd, epoll_fd_, ctx_.on_request, clients_acts_.find(fd)->second);
                handler.handle();
            } catch (...) {

            }
        } else {
            utils::FileDescriptor socket(fd); // closing fd when destroyed
        }

        clients_acts_.erase(fd);
        clients_.erase(fd);
    }

    void ServerWorker::handle_events(epoll_event* events, int nfds) {
        for (int n = 0; n < nfds; ++n) {
            int fd = events[n].data.fd;
            uint32_t event = events[n].events;

            if (fd == server_fd_) {
                accept_clients();
            } else {
                auto client = clients_.find(fd);
                if (client == clients_.end()) {
                    auto routine = Coroutine::create(&ServerWorker::handle_client, this, fd, event);
                    clients_.emplace(fd, routine);
                    Coroutine::resume(routine);
                } else {
                    Coroutine::resume(client->second);
                }
            }
        }
    }

    void ServerWorker::timeout_erase_client(int fd) {
        auto client = clients_.find(fd);
        if (client != clients_.end()) {
            clients_acts_.find(fd)->second.is_timed_out = true;
            Coroutine::resume(client->second); // handler.handle() should return
        } else {
            clients_acts_.erase(fd);
            utils::FileDescriptor socket(fd); // closing fd when destroyed
        }
    }

    bool ServerWorker::is_read_time_out(TimePoint last_resume) const {
        return (std::chrono::system_clock::now() - last_resume) > ctx_.read_timeout;
    }

    bool ServerWorker::is_write_time_out(TimePoint last_resume) const {
        return (std::chrono::system_clock::now() - last_resume) > ctx_.write_timeout;
    }

    void ServerWorker::check_timeouts() {
        for (auto& act : clients_acts_) {
            int fd = act.first;
            TimePoint last_resume = act.second.last_resume;

            if (act.second.activity == Activity::READ) {
                if (is_read_time_out(last_resume)) {
                    timeout_erase_client(fd);
                }
            }

            if (act.second.activity == Activity::WRITE) {
                if (is_write_time_out(last_resume)) {
                    timeout_erase_client(fd);
                }
            }
        }
    }

} // namespace http

