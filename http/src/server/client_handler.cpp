#include <thread>
#include <sys/epoll.h>
#include <filesystem>

#include "server/client_handler.hpp"
#include "coroutine/coroutine.hpp"
#include "exception.hpp"
#include "protocol/http.hpp"

namespace http {

    ClientHandler::ClientHandler(int fd, int epoll_fd, Callback on_request, ClientActivity& cl_act) noexcept
        : conn_(fd)
        , epoll_fd_(epoll_fd)
        , on_request_(std::move(on_request))
        , cl_act_(cl_act)
        , signed_event_(EPOLLIN) {
    }

    void ClientHandler::handle() {
        while (true) {
            if (signed_event_ == EPOLLOUT) {
                modify_epoll(EPOLLIN);
                signed_event_= EPOLLIN;
                cl_act_.activity = Activity::READ;

                Coroutine::yield();
                if (cl_act_.is_timed_out) {
                    return; // timed out
                }
            }

            std::string request_data = conn_.read(8192);

            // if (request_data.empty()) {
            //     cl_act_.last_resume = std::chrono::system_clock::now();
            //     Coroutine::yield();
            //     if (cl_act_.is_timed_out) {
            //         return; // timed out
            //     }
            //     continue;
            // }

            HttpRequest request(request_data);

            HttpResponse response = on_request_(request);

            cl_act_.activity = Activity::WRITE;

            if (!send_response(response)) {
                return; // timed out
            }

            if (request.method == http::version::V1_1) {
                auto connection_header = response.headers.find("Connection");
                if (connection_header != response.headers.end()) {
                    if (connection_header->second == "close") {
                        break;
                    }
                }
            }

            if (request.method == http::version::V1_0) {
                auto connection_header = response.headers.find("Connection");
                if (connection_header != response.headers.end()) {
                    if (connection_header->second == "Keep-Alive") {
                        continue;
                    }
                }
                break;
            }

        }
    }

    bool ClientHandler::send_response(HttpResponse& response) {
        std::string response_data = response.to_string();

        size_t total_bytes_written = 0;
        while (total_bytes_written != response_data.size()) {
            size_t bytes_written = conn_.write(response_data.data() + total_bytes_written,
                                               response_data.size() - total_bytes_written);
            if (bytes_written == 0) {
                if (signed_event_ != EPOLLOUT) {
                    modify_epoll(EPOLLOUT);
                    signed_event_ = EPOLLOUT;
                }
                cl_act_.last_resume = std::chrono::system_clock::now();
                Coroutine::yield();
                if (cl_act_.is_timed_out) {
                    return false;
                }
            }
            total_bytes_written += bytes_written;
        }


        while (!response.files.empty()) {
            std::string_view file_path = response.files.front();
            uintmax_t file_size = std::filesystem::file_size(file_path);
            total_bytes_written = 0;

            while (total_bytes_written != file_size) {
                size_t bytes_writen = conn_.send_file(file_path, total_bytes_written, file_size - total_bytes_written);
                total_bytes_written += bytes_writen;

                if (total_bytes_written != file_size) {
                    if (signed_event_ != EPOLLOUT) {
                        modify_epoll(EPOLLOUT);
                        signed_event_ = EPOLLOUT;
                    }
                    cl_act_.last_resume = std::chrono::system_clock::now();
                    Coroutine::yield();
                    if (cl_act_.is_timed_out) {
                        return false;
                    }
                }
            }
            response.files.pop();
        }
        return true;
    }

    void ClientHandler::modify_epoll(uint32_t events) {
        epoll_event event{};
        event.data.fd = conn_.get_fd();
        event.events = events;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, conn_.get_fd(), &event) < 0) {
            throw ServerError("Error modifying fd in epoll: ");
        }
    }

} // namespace http
