#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

#include "server/server.hpp"
#include "exception.hpp"

namespace http {

    Server::Server(std::string_view address, uint16_t port, int max_conn) {
        open(address, port);
        listen(max_conn);
    }

    void Server::open(std::string_view address, uint16_t port) {
        server_fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

        if (server_fd_ < 0) {
            throw ServerError("Error creating socket: ");
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (inet_aton(address.data(), &addr.sin_addr) == 0) {
            throw ServerError("Ip address is not valid, ip = " + std::string(address),
                              ExType::NO_ERRNO);
        }

        if (bind(server_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            throw ServerError("Error binding socket: ");
        }
    }

    void Server::listen(int max_conn) {
        if (::listen(server_fd_, max_conn) < 0) {
            throw ServerError("Error socket listening: ");
        }
    }

    void Server::close() {
        server_fd_.close();
    }

    void Server::join_threads() {
        if (done_)
            return;

        done_ = true;
        for (auto& worker : workers_) {
            worker.join();
        }
    }

    bool Server::is_opened() const noexcept {
        return server_fd_.is_opened();
    }

    void Server::run(TimeoutType read_timeout, TimeoutType write_timeout, size_t thread_count) {
        done_ = false;

        WorkerContex ctx{
            read_timeout,
            write_timeout,
            [this](const HttpRequest& request) {
                return on_request(request);
            }
        };


        workers_.reserve(thread_count - 1);
        for (size_t i = 0; i < thread_count - 1; ++i) {
            workers_.emplace_back([this, &ctx]() {
                ServerWorker worker(server_fd_, ctx, done_);
                worker.run();
            });
        }
        ServerWorker worker(server_fd_, ctx, done_);
        worker.run();


        // workers_.reserve(thread_count - 1);
        // for (size_t i = 0; i < thread_count - 1; ++i) {
        //     workers_.emplace_back([this, &ctx]() {
        //         ServerWorker worker(server_fd_, ctx, done_);
        //         worker.run();
        //     });
        // }
        // ServerWorker worker(server_fd_, ctx, done_);
        // worker.run();
    }

    void Server::signal_handler() {
        join_threads();
        close();
    }

} // namespace http
