#include "server/server.hpp"

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

class HttpServer : public http::Server {
public:
    HttpServer(std::string_view address, uint16_t port, int max_conn, std::string_view document_root);

    http::HttpResponse on_request(const http::HttpRequest& request) override;

private:
    bool validate_method(std::string_view method);
    bool validate_path(std::string_view path);
    bool validate_version(std::string_view version);

    bool is_file_available(std::string_view file_path);

private:
    std::string document_root_;
    std::string_view response_status;
};

#endif // HTTPSERVER_HPP
