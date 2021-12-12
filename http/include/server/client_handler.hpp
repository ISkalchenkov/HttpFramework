#ifndef HTTP_CLIENT_HANDLER_HPP
#define HTTP_CLIENT_HANDLER_HPP

#include <chrono>
#include <functional>

#include "server/connection.hpp"
#include "protocol/http_request.hpp"
#include "protocol/http_response.hpp"
#include "server/server_worker.hpp"

namespace http {

    struct ClientActivity;

    class ClientHandler {
    public:
        using Callback = std::function<HttpResponse(const HttpRequest&)>;

    public:
        ClientHandler(int fd, int epoll_fd, Callback on_request, ClientActivity& cl_act) noexcept;

        void handle();

    private:
        bool send_response(HttpResponse& response);

        void modify_epoll(uint32_t events);

    private:
        Connection conn_;
        const int epoll_fd_;
        uint32_t signed_event_;

        const Callback on_request_;
        ClientActivity& cl_act_;
    };

} // namespace http

#endif // HTTP_CLIENT_HANDLER_HPP
